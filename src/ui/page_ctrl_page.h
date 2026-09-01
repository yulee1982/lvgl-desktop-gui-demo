#ifndef CTRL_PAGE_H
#define CTRL_PAGE_H

#include "lvgl.h"
#include "desktop_titlebar.h"
#include "page_subpages.h"

// 控制页面结构体
typedef struct {
    lv_obj_t *card;
    lv_obj_t *icon;
    lv_obj_t *label;
    lv_obj_t *switch_obj;
    bool state;
} light_ctrl_t;

typedef struct {
    lv_obj_t *card;
    lv_obj_t *icon;
    lv_obj_t *label;
    lv_obj_t *value;
    int temperature;
} temp_ctrl_t;

typedef struct {
    lv_obj_t *card;
    lv_obj_t *icon;
    lv_obj_t *label;
    lv_obj_t *slider;
    int position;
} curtain_ctrl_t;

typedef struct {
    lv_obj_t *obj;         /**< 本页面对象 */
    lv_obj_t *parent;     /**< 父对象 */
    titlebar_t *titlebar;
    lv_obj_t *container;
    light_ctrl_t light;
    temp_ctrl_t temp;
    curtain_ctrl_t curtain;
    close_cb_t close_cb;  // 关闭回调函数指针
} ctrl_page_t;

/**
 * @brief 创建控制页面
 * @param parent 父对象
 * @param close_cb 关闭回调函数
 * @return 控制页面对象指针
 */
lv_obj_t* ctrl_page_create(lv_obj_t * parent,close_cb_t close_cb);

/**
 * @brief 设置关闭回调函数
 * @param page 控制页面对象
 * @param cb 回调函数
 */
void ctrl_page_set_close_cb(ctrl_page_t *page, void (*cb)(void));

#endif // CTRL_PAGE_H
