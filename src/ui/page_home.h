#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include "lvgl/lvgl.h"
#include "desktop.h"
#include "page_ctrl_page.h"
#include "page_video_page.h"
#include "page_subpages.h"

typedef lv_obj_t* (*ctrl_page_create_t)(lv_obj_t * parent, close_cb_t close_cb);
/**
 * @brief 应用页面ID枚举
 */
typedef enum {
    APP_PAGE_ID_CTRL,      /**< 控制页面ID */
    APP_PAGE_ID_VIDEO,    /**< 视频页面ID */
    APP_PAGE_ID_FILE_EXPLORER,  /**< 文件浏览器页面ID */
    MAX_APP_PAGE_COUNT
} app_page_id_t;
/**
 * @brief 应用页面信息结构体
 */
typedef struct {
    lv_obj_t *obj;        /**< 页面对象 */
    app_page_id_t app_id;  /**< 应用页面ID */
    const char *title;     /**< 页面标题 */
    const char *icon_path;        /**< 页面图标 */
    ctrl_page_create_t page_create_cb;  /**< 创建页面回调函数 */
} app_page_info_t;

/**
 * @brief 主页面结构体
 */
typedef struct {
    lv_obj_t *parent;     /**< 父对象 */
    desktop_t *desktop;    /**< 桌面组件 */
    ctrl_page_t *ctrl_page; /**< 控制页面 */
    video_page_t *video_page; /**< 视频页面 */
} home_page_t;

/**
 * @brief 创建主页面
 * @param parent 父对象
 * @return 返回主页面结构体指针，失败返回NULL
 */
home_page_t *create_home_screen(lv_obj_t * parent);

#endif // HOME_PAGE_H
