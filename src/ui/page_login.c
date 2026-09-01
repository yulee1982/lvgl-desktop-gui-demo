#include "page_login.h"
#include "page_home.h"
#include "page_manager.h"
//#include "virt_kb.h"
#include "desktop_usermgr.h"

extern lv_obj_t *home_screen;
//extern virt_kb_t *g_virt_kb;


// 全局变量，用于存储输入框对象的引用
static lv_obj_t *g_username_input;
static lv_obj_t *g_password_input;

static void ta_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = lv_event_get_user_data(e);
    
    if (code == LV_EVENT_FOCUSED) {
        LV_LOG_USER("文本框获得焦点，显示虚拟键盘");
        //if (kb)
        //    virt_kb_show(kb, ta);
        if (lv_indev_get_type(lv_indev_active()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES / 2, 0);
            lv_obj_update_layout(get_page_obj(PAGE_LOGIN));   /*Be sure the sizes are recalculated*/
            //lv_obj_set_height(get_page_obj(PAGE_LOGIN), LV_VER_RES - lv_obj_get_height(kb));
            //lv_obj_update_layout(kb);   /*Be sure the sizes are recalculated*/
            //lv_obj_set_height(kb, LV_VER_RES - lv_obj_get_height(kb));
            lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
            lv_indev_wait_release(lv_event_get_param(e)); //阻塞当前任务，等待输入设备释放，确保用户完成输入
        }

    }
    else if (code == LV_EVENT_DEFOCUSED) {
        LV_LOG_USER("文本框失去焦点，隐藏虚拟键盘");
        //if (kb) 
        //    virt_kb_hide(kb);
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_set_height(get_page_obj(PAGE_LOGIN), LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);
    }
    else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_set_height(get_page_obj(PAGE_LOGIN), LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}

// 验证用户名和密码
static bool verify_user(const char *username, const char *password)
{
    if (!username || !password) {
        return false;
    }
    
    return usermgr_verify_password(username, password);
}

// 显示错误消息
static void show_error_message(const char *message)
{
    //lv_obj_t *mbox = lv_msgbox_create(NULL, "ERROR", message, NULL, true);
    //lv_obj_center(mbox);

    lv_obj_t* mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "ERROR");
    lv_msgbox_add_text(mbox, message);
    //lv_msgbox_add_footer_button(mbox, "Apply");
    //lv_msgbox_add_footer_button(mbox, "Close");
    //lv_msgbox_add_header_button(mbox, LV_SYMBOL_AUDIO);
    lv_msgbox_add_close_button(mbox);
    lv_obj_center(mbox);
}

void create_login_screen(lv_obj_t * parent) {
    // 创建滚动容器
    lv_obj_t *scroll_container = lv_obj_create(parent);
    lv_obj_set_size(scroll_container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_border_width(scroll_container, 0, 0);  // 设置边框宽度为0，使外框不可见
    lv_obj_set_style_pad_all(scroll_container, 30, 0);
    lv_obj_set_style_pad_top(scroll_container, 20, 0);  // 增加顶部内边距
    lv_obj_set_scrollbar_mode(scroll_container, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_scroll_dir(scroll_container, LV_DIR_VER);
    lv_obj_set_flex_flow(scroll_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scroll_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scroll_container, 30, 0);  // 设置垂直方向上的间距为30
    lv_obj_clear_flag(scroll_container, LV_OBJ_FLAG_SCROLL_CHAIN_HOR);
    lv_obj_add_flag(scroll_container, LV_OBJ_FLAG_SCROLL_ONE);
    
    // 创建用户名输入区域的容器
    lv_obj_t *username_container = lv_obj_create(scroll_container);
    lv_obj_set_size(username_container, 300, 40);
    lv_obj_set_style_pad_all(username_container, 0, 0);
    lv_obj_set_style_border_width(username_container, 0, 0);
    lv_obj_set_flex_flow(username_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(username_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /*Create a keyboard*/
    lv_obj_t* kb = lv_keyboard_create(lv_screen_active());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    // 创建用户名标签
    lv_obj_t *username_label = lv_label_create(username_container);
    lv_label_set_text(username_label, "User name");
    lv_obj_set_style_min_width(username_label, 80, 0);
    
    // 创建用户名输入框
    g_username_input = lv_textarea_create(username_container);
    //lv_textarea_set_placeholder_text(g_username_input, "Username");
    lv_textarea_set_text(g_username_input, "admin");
    lv_obj_set_size(g_username_input, 200, 40);
    lv_obj_add_event_cb(g_username_input, ta_event_cb, LV_EVENT_ALL, kb); //g_virt_kb
    
    // 创建密码输入区域的容器
    lv_obj_t *password_container = lv_obj_create(scroll_container);
    lv_obj_set_size(password_container, 300, 40);
    lv_obj_set_style_pad_all(password_container, 0, 0);
    lv_obj_set_style_border_width(password_container, 0, 0);
    lv_obj_set_flex_flow(password_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(password_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // 创建密码标签
    lv_obj_t *password_label = lv_label_create(password_container);
    lv_label_set_text(password_label, "Password");
    lv_obj_set_style_min_width(password_label, 80, 0);
    
    // 创建密码输入框
    g_password_input = lv_textarea_create(password_container);
    lv_textarea_set_placeholder_text(g_password_input, "Password");
    lv_textarea_set_password_mode(g_password_input, true);
    lv_textarea_set_one_line(g_password_input, true);
    lv_obj_set_size(g_password_input, 200, 40);
    lv_obj_add_event_cb(g_password_input, ta_event_cb, LV_EVENT_ALL, kb);//g_virt_kb
    
    // 创建登录按钮
    lv_obj_t *btn = lv_btn_create(scroll_container);
    lv_obj_set_size(btn, 120, 40);
    lv_obj_add_event_cb(btn, login_btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Login");
    lv_obj_center(btn_label);
}

void login_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        const char *username = lv_textarea_get_text(g_username_input);
        const char *password = lv_textarea_get_text(g_password_input);

        if (verify_user(username, password)) {
            switch_to_page(PAGE_HOME);
        } else {
            show_error_message("username or password error !");
        }
    }
}
