#ifndef FILE_EXPLORER_PAGE_H
#define FILE_EXPLORER_PAGE_H

#include "lvgl.h"
#include "desktop_titlebar.h"
#include "page_subpages.h"

/**
 * @brief 视频页面结构体
 */
typedef struct {
    lv_obj_t* obj;        /**< 本页面对象 */
    lv_obj_t* parent;     /**< 父对象 */
    titlebar_t* titlebar;
    lv_obj_t* explorer;
    close_cb_t close_cb; /**< 关闭回调函数 */
} file_explorer_page_t;

lv_obj_t* file_explorer_page_create(lv_obj_t * parent,close_cb_t close_cb);

void file_explorer_page_set_close_cb(file_explorer_page_t * video_page, close_cb_t close_cb);

#endif // CTRL_PAGE_H
