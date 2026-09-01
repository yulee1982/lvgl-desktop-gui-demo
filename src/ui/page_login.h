#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "lvgl/lvgl.h"

/**
 * @brief 创建登录页面
 * @param parent 父对象
 */
void create_login_screen(lv_obj_t * parent);

/**
 * @brief 登录按钮事件回调
 * @param e 事件对象
 */
void login_btn_event_cb(lv_event_t *e);

#endif // LOGIN_PAGE_H
