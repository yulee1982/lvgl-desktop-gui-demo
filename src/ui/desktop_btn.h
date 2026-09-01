#ifndef DESKTOP_BTN_H
#define DESKTOP_BTN_H

#include "lvgl/lvgl.h"

// 桌面按钮结构体定义
typedef struct {
    lv_obj_t *btn;      // 主按钮对象
    lv_obj_t *icon;     // 图标对象
    lv_obj_t *label;    // 标题标签对象
    const char *title;  // 标题文本
} desktop_btn_t;

/**
 * @brief 创建一个桌面按钮
 * @param parent 父对象
 * @param title 按钮标题
 * @param icon_path 图标路径
 * @return 返回创建的桌面按钮对象指针，失败返回NULL
 */
desktop_btn_t *desktop_btn_create(lv_obj_t *parent, const char *title, const char *icon_path);

/**
 * @brief 设置桌面按钮的位置
 * @param btn 桌面按钮对象
 * @param x X坐标
 * @param y Y坐标
 */
void desktop_btn_set_pos(desktop_btn_t *btn, lv_coord_t x, lv_coord_t y);

/**
 * @brief 为桌面按钮添加点击事件回调
 * @param btn 桌面按钮对象
 * @param event_cb 事件回调函数
 * @param user_data 用户数据
 */
void desktop_btn_add_event_cb(desktop_btn_t *btn, lv_event_cb_t event_cb, void *user_data);

/**
 * @brief 识别按钮OBJ
 * @param btn 桌面按钮对象
 */
bool desktop_btn_recognize_btn(desktop_btn_t *btn,lv_obj_t *obj);

#endif // DESKTOP_BTN_H
