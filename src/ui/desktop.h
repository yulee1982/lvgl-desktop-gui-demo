#ifndef DESKTOP_H
#define DESKTOP_H

#include "lvgl/lvgl.h"
#include "desktop_btn.h"

// 最大应用数量
#define MAX_APP_COUNT 8

typedef void (*create_page_cb_t)(lv_obj_t *parent, int app_id);

// 页面信息结构体
typedef struct {
    lv_obj_t *page;           // 页面对象
    int app_id;             // 页面ID
} desktop_app_t;

// 桌面组件结构体
typedef struct {
    lv_obj_t *obj;              // 桌面对象
    lv_obj_t *status_bar;       // 状态栏
    lv_obj_t *wifi_icon;        // WiFi图标
    lv_obj_t *battery_icon;     // 电池图标
    lv_obj_t *notify_icon;      // 通知图标
    lv_obj_t *app_btn_container;    // 应用按钮容器
    lv_obj_t *page_container;   // 页面容器
    desktop_btn_t *apps_btn[MAX_APP_COUNT];  // 应用按钮数组
    desktop_app_t pages[MAX_APP_COUNT]; // 子应用页面数组
    uint8_t app_count;          // 应用数量
    uint8_t page_count;         // 页面数量
    create_page_cb_t create_page_cb; // 创建页面的回调函数;
} desktop_t;

// 全局桌面实例指针
extern desktop_t *g_desktop;

/**
 * @brief 创建桌面组件
 * @param parent 父对象
 * @return 桌面组件指针，失败返回NULL
 */
desktop_t *desktop_create(lv_obj_t *parent);

/**
 * @brief 添加APP图标到桌面
 * @param desktop 桌面组件指针
 * @param page 页面对象
 * @param title 页面标题
 * @param icon_path 图标路径
 */
void desktop_add_app(desktop_t *desktop, const char *title, const char *icon_path,int app_id);


/**
 * @brief 添加页面到桌面
 * @param desktop 桌面组件指针
 * @param page 页面对象
 * @param title 页面标题
 * @param icon_path 图标路径
 */
void desktop_add_page(desktop_t *desktop, lv_obj_t *page,int app_id);

/**
 * @brief 设置页面布局
 * @param desktop 桌面组件指针
 * @param page 页面对象
 * @param x 页面x坐标
 * @param y 页面y坐标
 * @param width 页面宽度
 * @param height 页面高度
 */
void desktop_set_page_layout(desktop_t *desktop, lv_obj_t *page, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height);

/**
 * @brief 关闭所有显示的页面
 * @param desktop 桌面组件指针
 */
void desktop_close_all_pages(desktop_t *desktop);

/**
 * @brief 注册创建页面的回调函数
 * @param desktop 桌面组件指针
 * @param cb 创建页面的回调函数
 */
void desktop_register_create_page_cb(desktop_t *desktop, create_page_cb_t cb);

#endif // DESKTOP_H
