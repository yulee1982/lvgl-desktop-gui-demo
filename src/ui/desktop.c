#include "desktop.h"
#include "lv_app_conf.h"
#include "desktop_btn.h"
#include "lvgl/lvgl.h"

static void app_btn_event_cb(lv_event_t *e);

// 全局桌面实例指针定义
desktop_t *g_desktop = NULL;

desktop_t *desktop_create(lv_obj_t *parent) {
    // 分配内存
    desktop_t *desktop = lv_malloc(sizeof(desktop_t));
    if (desktop == NULL) {
        return NULL;
    }
    lv_memset(desktop, 0, sizeof(desktop_t));
    
    // 创建桌面对象
    desktop->obj = lv_obj_create(parent);
    if (desktop->obj == NULL) {
        lv_free(desktop);
        return NULL;
    }
    
    // 设置桌面样式
    lv_obj_set_size(desktop->obj, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(desktop->obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(desktop->obj, 0, 0);
    lv_obj_set_style_border_width(desktop->obj, 0, 0);

    // 设置背景图片
    lv_obj_set_style_bg_img_src(desktop->obj, DESKTOP_BG_PATH, 0);
    lv_obj_set_style_bg_image_opa(desktop->obj, LV_OPA_COVER, 0); //LV_OPA_30
    
    // 创建状态栏
    desktop->status_bar = lv_obj_create(desktop->obj);
    lv_obj_set_size(desktop->status_bar, LV_PCT(100), 30);
    lv_obj_align(desktop->status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(desktop->status_bar, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(desktop->status_bar, LV_OPA_20, 0);
    lv_obj_set_style_radius(desktop->status_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT); //移除圆角
    lv_obj_clear_flag(desktop->status_bar, LV_OBJ_FLAG_SCROLLABLE);//移除滚动条
    lv_obj_set_style_border_width(desktop->status_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT); //移除边框
    // 创建WiFi图标
    desktop->wifi_icon = lv_label_create(desktop->status_bar);
    lv_label_set_text(desktop->wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_align(desktop->wifi_icon, LV_ALIGN_LEFT_MID, 10, 0);  
    // 创建电池图标
    desktop->battery_icon = lv_label_create(desktop->status_bar);
    lv_label_set_text(desktop->battery_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(desktop->battery_icon, LV_ALIGN_RIGHT_MID, -10, 0);
    // 创建通知图标
    desktop->notify_icon = lv_label_create(desktop->status_bar);
    lv_label_set_text(desktop->notify_icon, LV_SYMBOL_BELL);
    lv_obj_align(desktop->notify_icon, LV_ALIGN_RIGHT_MID, -40, 0);

    // 创建应用按钮容器
    desktop->app_btn_container = lv_obj_create(desktop->obj);
    //lv_obj_set_size(desktop->app_btn_container, LV_PCT(100), LV_PCT(90));
    lv_obj_set_size(desktop->app_btn_container, LV_PCT(100), LV_VER_RES - 30); //LV_VER_RES
    lv_obj_align(desktop->app_btn_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(desktop->app_btn_container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(desktop->app_btn_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT); //移除边框
    lv_obj_clear_flag(desktop->app_btn_container, LV_OBJ_FLAG_SCROLLABLE);

    // 创建页面容器
    desktop->page_container = lv_obj_create(desktop->obj);
    lv_obj_set_size(desktop->page_container, LV_PCT(100), LV_PCT(100));
    lv_obj_align(desktop->page_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(desktop->page_container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(desktop->page_container, 0, 0);
    lv_obj_set_style_pad_all(desktop->page_container, 0, 0);
    lv_obj_clear_flag(desktop->page_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(desktop->page_container, LV_OBJ_FLAG_HIDDEN);
    
    // 初始化计数器
    desktop->app_count = 0;
    desktop->page_count = 0;
    
    return desktop;
}

void desktop_add_app(desktop_t *desktop, const char *title, const char *icon_path,int app_id) {
    if (desktop == NULL ||  desktop->app_count >= MAX_APP_COUNT) {
        return;
    }
    
    // 保存页面到pages数组
    desktop->pages[desktop->page_count].page = NULL;
    desktop->pages[desktop->page_count].app_id = app_id;
    
    // 创建应用按钮
    desktop_btn_t *btn = desktop_btn_create(desktop->app_btn_container, title, 
                                          icon_path ? icon_path : DESKTOP_ICON_DIR"/default.png");
    if (btn == NULL) {
        return;
    }
    
    // 计算并设置按钮位置
    lv_coord_t x = (desktop->app_count % 6) * 68 + 20;
    lv_coord_t y = (desktop->app_count / 6) * 84 + 20;
    desktop_btn_set_pos(btn, x, y);

    // 注册按钮事件回调
    desktop_btn_add_event_cb(btn, app_btn_event_cb, desktop);
    
    // 保存按钮对象
    desktop->apps_btn[desktop->app_count] = btn;
    
    // 更新计数器
    desktop->app_count++;
    desktop->page_count++;

    
}

void desktop_add_page(desktop_t *desktop, lv_obj_t *page,int app_id)
{
    if (desktop == NULL || page == NULL) {
        return;
    }

    // 将页面的父对象设置为页面容器
    lv_obj_set_parent(page, desktop->page_container);
    lv_obj_add_flag(page, LV_OBJ_FLAG_HIDDEN);

    int idx = -1;
    int i = 0;
    // 查找页面在pages数组中的索引
    for (i = 0; i < MAX_APP_COUNT; i++) {
        if (desktop->pages[i].app_id == app_id) {
            idx = i;
            break;
        }
    }

    // 如果页面不在pages数组中，则返回
    if (idx == -1) {
        return;
    }

    // 更新pages数组
    desktop->pages[idx].page = page;
}

void desktop_set_page_layout(desktop_t *desktop, lv_obj_t *page, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height) {
    if (desktop == NULL || page == NULL) {
        return;
    }
    
    // 设置页面位置和大小
    lv_obj_set_pos(page, x, y);
    lv_obj_set_size(page, width, height);
    lv_obj_set_style_border_width(page, 0, 0);
    lv_obj_set_style_pad_all(page, 0, 0);
}

static void app_btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    desktop_t *desktop = lv_event_get_user_data(e);
    uint8_t i = 0;
    
    // 查找按钮在apps_btn数组中的索引
    int idx = -1;
    for (i = 0; i < MAX_APP_COUNT; i++) {
        if(desktop_btn_recognize_btn(desktop->apps_btn[i], btn)) {
            idx = i;
            break;
        }
    }

    // 如果按钮不在apps_btn数组中，则返回
    if (idx == -1) {
        LV_LOG_ERROR("按钮不存在");
        return;
    }
    if(!desktop->pages[idx].page) {
        if (desktop->create_page_cb) {
            desktop->create_page_cb(desktop->page_container, desktop->pages[idx].app_id);
        } else {
            LV_LOG_ERROR("未注册创建页面回调函数");
            return;
        }
    }
    
    // 隐藏所有页面
    for(i = 0; i < desktop->page_count; i++) {
        if(desktop->pages[i].page) {
            lv_obj_add_flag(desktop->pages[i].page, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    if(desktop->pages[idx].page) {
        LV_LOG_USER("display app_id:%d", desktop->pages[idx].app_id);
        // 显示被点击的页面
        lv_obj_clear_flag(desktop->pages[idx].page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(desktop->page_container, LV_OBJ_FLAG_HIDDEN);
    } else {
        LV_LOG_ERROR("页面不存在,请先注册页面,app_id:%d", desktop->pages[idx].app_id);
    }
}

void desktop_close_all_pages(desktop_t *desktop) {
    if (desktop == NULL) {
        return;
    }
    
    LV_LOG_USER("Desktop: Closing all pages\n");
    
    // 隐藏所有页面
    uint8_t i = 0;
    for(i = 0; i < desktop->page_count; i++) {
        if(desktop->pages[i].page) {
            lv_obj_add_flag(desktop->pages[i].page, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    // 隐藏页面容器
    lv_obj_add_flag(desktop->page_container, LV_OBJ_FLAG_HIDDEN);
}

void desktop_register_create_page_cb(desktop_t *desktop, create_page_cb_t cb) {
    if (desktop == NULL) {
        return;
    }

    desktop->create_page_cb = cb;
}

