#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "lvgl/lvgl.h"

/**
 * @brief 标题栏组件结构体
 */
typedef struct {
    lv_obj_t *cont;          // 容器对象
    lv_obj_t *title;         // 标题文本
    lv_obj_t *close_btn;     // 关闭按钮
    lv_obj_t *icon;          // 左侧图标（可选）
    void (*close_cb)(void *); // 关闭按钮回调函数
    void *user_data;         // 用户数据指针
} titlebar_t;

/**
 * @brief 创建标题栏
 * @param parent 父对象
 * @return 标题栏对象指针，失败返回NULL
 */
titlebar_t *titlebar_create(lv_obj_t *parent);

/**
 * @brief 设置标题文本
 * @param titlebar 标题栏对象
 * @param text 标题文本
 */
void titlebar_set_title(titlebar_t *titlebar, const char *text);

/**
 * @brief 设置左侧图标
 * @param titlebar 标题栏对象
 * @param img_src 图标源文件路径
 */
void titlebar_set_icon(titlebar_t *titlebar, const char *img_src);

/**
 * @brief 设置关闭按钮回调函数
 * @param titlebar 标题栏对象
 * @param cb 回调函数
 */
void titlebar_set_close_cb(titlebar_t *titlebar, void (*cb)(void *), void *user_data);

#endif // TITLEBAR_H
