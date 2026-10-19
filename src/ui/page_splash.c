#include "page_splash.h"
#include "page_logo_icon.h"
#include "page_manager.h"

// 声明外部变量
extern lv_obj_t *login_screen;

// 启动动画回调
static void splash_timer_cb(lv_timer_t *timer) {
    //switch_to_page(PAGE_LOGIN);
    switch_to_page(PAGE_HOME);
}

void create_splash_screen(lv_obj_t * parent) {
    // 显示logo动画
    show_logo_animation(parent);
    
    // 3秒后自动切换到登录页面，只触发一次
    lv_timer_t * timer = lv_timer_create(splash_timer_cb, 3000, NULL);
    lv_timer_set_repeat_count(timer, 1);
}
