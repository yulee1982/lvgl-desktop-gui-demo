#include "page_home.h"
#include "desktop.h"
#include "page_ctrl_page.h"
#include "page_video_page.h"
#include "page_file_explorer_page.h"
#include "lv_app_conf.h"

home_page_t *g_home_page = NULL;

/*
* 子页面信息
* app_id: 应用ID
* title: 页面标题
* icon_path: 图标路径
* page_create_cb: 创建页面的回调函数
*/
static app_page_info_t app_pages_info[MAX_APP_PAGE_COUNT] = {
    {NULL, APP_PAGE_ID_CTRL, "ctrl", DESKTOP_ICON_DIR"/ctrl_page.png",ctrl_page_create},
    {NULL, APP_PAGE_ID_VIDEO, "video", DESKTOP_ICON_DIR"/video_page.png",video_page_create},
    {NULL, APP_PAGE_ID_FILE_EXPLORER, "file explorer", DESKTOP_ICON_DIR"/file_explorer_page.png",file_explorer_page_create},
};

static void home_sub_page_closed_cb(void)
{
    if (g_home_page == NULL)
        return;

    desktop_close_all_pages(g_home_page->desktop);
}

static void home_create_sub_page(lv_obj_t *parent, int app_id)
{
    if (g_home_page == NULL) {
        LV_LOG_ERROR("g_home_page is NULL");
        return;
    }
    if (app_id < 0 || app_id >= MAX_APP_PAGE_COUNT) {
        LV_LOG_ERROR("Invalid app_id: %d", app_id);
        return;
    }

    app_page_info_t *page_info = &app_pages_info[app_id];
    if (page_info->obj != NULL) {
        LV_LOG_ERROR("Page already exists for app_id: %d", app_id);
        return;
    }
    // 创建子页面
    LV_LOG_USER("create sub page for app_id: %d", app_id);
    page_info->obj = page_info->page_create_cb(parent, home_sub_page_closed_cb);
    if (page_info->obj == NULL) {
        LV_LOG_ERROR("Failed to create page for app_id: %d", app_id);
        return;
    }
    // 将子页面添加到桌面
    desktop_add_page(g_home_page->desktop, page_info->obj, app_id);
}

home_page_t *create_home_screen(lv_obj_t * parent)
{
    // 分配主页面结构体内存
    home_page_t *home_page = (home_page_t *)lv_malloc(sizeof(home_page_t));
    if (home_page == NULL) {
        return NULL;
    }
    lv_memset(home_page, 0, sizeof(home_page_t));

    // 将主页面结构体指针赋值给全局变量
    g_home_page = home_page;
    
    // 初始化结构体成员
    home_page->parent = parent;
    
    // 创建桌面组件
    home_page->desktop = desktop_create(parent);
    if (home_page->desktop == NULL) {
        lv_free(home_page);
        return NULL;
    }
    desktop_register_create_page_cb(home_page->desktop,home_create_sub_page);
    
    // 添加标题和图标到模拟桌面
    int i = 0;
    for (i = 0; i < MAX_APP_PAGE_COUNT; i++) {
        app_page_info_t *page_info = &app_pages_info[i];
        desktop_add_app(home_page->desktop, page_info->title, page_info->icon_path, page_info->app_id);
    }
    
    return home_page;
}

