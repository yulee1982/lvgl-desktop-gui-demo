#ifndef VIDEO_PAGE_H
#define VIDEO_PAGE_H

#include "lvgl/lvgl.h"
#include "vplayer.h"
#include "page_subpages.h"

/**
 * @brief 视频页面结构体
 */
typedef struct {
    lv_obj_t *obj;        /**< 本页面对象 */
    lv_obj_t *parent;     /**< 父对象 */
    vplayer_t *vplayer;   /**< 视频播放器组件 */
    close_cb_t close_cb; /**< 关闭回调函数 */
} video_page_t;

/**
 * @brief 创建视频页面
 * @param parent 父对象
 * @param close_cb 关闭回调函数
 * @return 视频页面对象
 */
lv_obj_t *video_page_create(lv_obj_t * parent,close_cb_t close_cb);

/**
 * @brief 设置视频页面关闭回调函数
 * @param video_page 视频页面结构体指针
 * @param close_cb 关闭回调函数
 */
void video_page_set_close_cb(video_page_t *video_page, close_cb_t close_cb);

/**
 * @brief 显示视频页面
 */
void video_page_show(void);

#endif // VIDEO_PAGE_H
