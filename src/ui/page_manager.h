#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "lvgl/lvgl.h"

// 定义页面状态
typedef enum {
    PAGE_SPLASH,    // 启动动画
    PAGE_LOGIN,     // 登录页面
    PAGE_HOME       // 主页面
} page_state_t;

// 获取当前页面状态
page_state_t get_current_page(void);

// 切换到指定页面
void switch_to_page(page_state_t page);

// 初始化页面管理器
void page_manager_init(void);

// 获取页面对象
lv_obj_t* get_page_obj(page_state_t page);

// 声明外部可访问的页面对象
//extern lv_obj_t *login_screen;

#endif // PAGE_MANAGER_H
