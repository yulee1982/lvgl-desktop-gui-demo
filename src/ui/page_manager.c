#include "page_manager.h"
#include "page_splash.h"
#include "page_login.h"
#include "page_home.h"

static size_t display_screen_load_start = 0;
static size_t display_screen_loaded = 0;
static size_t display_screen_unload_start = 0;
static size_t display_screen_unloaded = 0;

// 当前页面状态
static page_state_t current_page = -1;

// 主容器和页面对象
static lv_obj_t *main_container = NULL;
static lv_obj_t *splash_page = NULL;
lv_obj_t *login_page = NULL;
static lv_obj_t *home_page = NULL;

// 获取当前页面状态
page_state_t get_current_page(void)
{
    return current_page;
}

// 切换到指定页面
void switch_to_page(page_state_t page)
{
    lv_obj_t *target_page = get_page_obj(page);
    lv_obj_t *current_obj = get_page_obj(current_page);
    
    if (target_page && target_page != current_obj) {
        // 触发当前页面的卸载事件
        if (current_obj) {
            lv_obj_send_event(current_obj, LV_EVENT_SCREEN_UNLOAD_START, NULL);
            lv_obj_add_flag(current_obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_send_event(current_obj, LV_EVENT_SCREEN_UNLOADED, NULL);
        }
        
        // 触发目标页面的加载事件
        lv_obj_send_event(target_page, LV_EVENT_SCREEN_LOAD_START, NULL);
        lv_obj_clear_flag(target_page, LV_OBJ_FLAG_HIDDEN);
        
        // 更新当前页面状态
        current_page = page;
        
        // 触发页面加载完成事件
        lv_obj_send_event(target_page, LV_EVENT_SCREEN_LOADED, NULL);
        
        // 确保页面切换后不会被其他操作干扰
        lv_timer_handler();
    }
}

// 创建主容器
static void create_main_container(void)
{
    main_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(main_container, 0, 0);
    lv_obj_set_style_border_width(main_container, 0, 0);
}

// 创建所有页面
static void create_pages(void)
{
    // 创建所有页面
    splash_page = lv_obj_create(main_container);
    login_page = lv_obj_create(main_container);
    home_page = lv_obj_create(main_container);

    // 设置页面的样式
    lv_obj_set_style_pad_all(splash_page, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(splash_page, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(login_page, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(login_page, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(home_page, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(home_page, 0, LV_PART_MAIN);

    
    // 设置页面大小为100%
    lv_obj_set_size(splash_page, LV_PCT(100), LV_PCT(100));
    lv_obj_set_size(login_page, LV_PCT(100), LV_PCT(100));
    lv_obj_set_size(home_page, LV_PCT(100), LV_PCT(100));
    
    // 清除页面容器的内边距和滚动属性
    lv_obj_set_style_pad_all(splash_page, 0, 0);
    lv_obj_set_style_pad_all(login_page, 0, 0);
    lv_obj_set_style_pad_all(home_page, 0, 0);
    
    lv_obj_clear_flag(splash_page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(login_page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(home_page, LV_OBJ_FLAG_SCROLLABLE);
    
    // 初始隐藏所有页面
    lv_obj_add_flag(splash_page, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(login_page, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(home_page, LV_OBJ_FLAG_HIDDEN);
    
    // 初始化各个页面的内容
    create_splash_screen(splash_page);
    create_login_screen(login_page);
    create_home_screen(home_page);
}

// 初始化页面管理器
void page_manager_init(void)
{
    // 创建主容器
    create_main_container();

    // 创建所有页面
    create_pages();
    
    // 显示启动页面
    switch_to_page(PAGE_SPLASH);
}

// 获取页面对象
lv_obj_t* get_page_obj(page_state_t page)
{
    switch (page) {
        case PAGE_SPLASH:
            return splash_page;
        case PAGE_LOGIN:
            return login_page;
        case PAGE_HOME:
            return home_page;
        default:
            return NULL;
    }
}
