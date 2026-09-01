#include <stdlib.h>
#include <string.h>
#include "page_file_explorer_page.h"

file_explorer_page_t * file_explorer_page = NULL;

// 前向声明
static void file_explorer_page_close_handler(void *page);
static void file_explorer_event_handler(lv_event_t* e);
#if LV_FILE_EXPLORER_QUICK_ACCESS
static void btn_event_handler(lv_event_t* e);
static void dd_event_handler(lv_event_t* e);
#endif

lv_obj_t* file_explorer_page_create(lv_obj_t * parent,close_cb_t close_cb)
{
    // 分配内存
    file_explorer_page_t * page = (file_explorer_page_t *)lv_malloc(sizeof(file_explorer_page_t));
    if (page == NULL) {
        return NULL;
    }
    lv_memset(page, 0, sizeof(file_explorer_page_t));

    file_explorer_page = page;

    page->obj = lv_obj_create(parent);
    lv_obj_set_size(page->obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_border_width(page->obj, 0, 0);
    lv_obj_set_style_pad_all(page->obj, 0, 0);

    page->parent = parent;
    page->close_cb = close_cb;

    // 创建标题栏
    page->titlebar = titlebar_create(page->obj);
    if (page->titlebar == NULL) {
        lv_free(page);
        return NULL;
    }
    titlebar_set_title(page->titlebar, "file explorer");
    // 添加标题栏关闭按钮的事件处理
    titlebar_set_close_cb(page->titlebar, file_explorer_page_close_handler, page);

    page->explorer = lv_obj_create(page->obj);
    lv_obj_set_size(page->explorer, LV_HOR_RES, LV_VER_RES - 40);
    lv_obj_align(page->explorer, LV_ALIGN_TOP_MID, 0, 40);


    lv_obj_t* file_explorer = lv_file_explorer_create(page->explorer);

#if LV_USE_FS_WIN32
    lv_file_explorer_open_dir(file_explorer, "C:C:/");
#if LV_FILE_EXPLORER_QUICK_ACCESS
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_HOME_DIR, "C:C:/Users/Public/Desktop");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_VIDEO_DIR, "C:C:/Users/Public/Videos");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_PICTURES_DIR, "C:C:/Users/Public/Pictures");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_MUSIC_DIR, "C:C:/Users/Public/Music");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_DOCS_DIR, "C:C:/Users/Public/Documents");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_FS_DIR, "C:C:/");
#endif

#else
    /* linux */
    lv_file_explorer_open_dir(file_explorer, "A:/");
#if LV_FILE_EXPLORER_QUICK_ACCESS
    const char* envvar = "HOME";
    char home_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(home_dir, "A:");
    /* get the user's home directory from the HOME environment variable*/
    strcat(home_dir, getenv(envvar));
    LV_LOG_USER("home_dir: %s\n", home_dir);
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_HOME_DIR, home_dir);
    char video_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(video_dir, home_dir);
    strcat(video_dir, "/Videos");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_VIDEO_DIR, video_dir);
    char picture_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(picture_dir, home_dir);
    strcat(picture_dir, "/Pictures");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_PICTURES_DIR, picture_dir);
    char music_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(music_dir, home_dir);
    strcat(music_dir, "/Music");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_MUSIC_DIR, music_dir);
    char document_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(document_dir, home_dir);
    strcat(document_dir, "/Documents");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_DOCS_DIR, document_dir);

    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_FS_DIR, "A:/");
#endif
#endif

    lv_obj_add_event_cb(file_explorer, file_explorer_event_handler, LV_EVENT_ALL, NULL);

#if LV_FILE_EXPLORER_QUICK_ACCESS
    /*Quick access status control button*/
    lv_obj_t* fe_quick_access_obj = lv_file_explorer_get_quick_access_area(file_explorer);
    lv_obj_t* fe_header_obj = lv_file_explorer_get_header(file_explorer);
    lv_obj_t* btn = lv_button_create(fe_header_obj);
    lv_obj_set_style_radius(btn, 2, 0);
    lv_obj_set_style_pad_all(btn, 4, 0);
    lv_obj_align(btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_LIST);
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, fe_quick_access_obj);

    /*Sort control*/
    static const char* opts = "NONE\n"
        "KIND";

    lv_obj_t* dd = lv_dropdown_create(fe_header_obj);
    lv_obj_set_style_radius(dd, 4, 0);
    lv_obj_set_style_pad_all(dd, 0, 0);
    lv_obj_set_size(dd, LV_PCT(30), LV_SIZE_CONTENT);
    lv_dropdown_set_options_static(dd, opts);
    lv_obj_align(dd, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_add_event_cb(dd, dd_event_handler, LV_EVENT_VALUE_CHANGED, file_explorer);
#endif

    return page->obj;
}

// 设置关闭回调函数
void file_explorer_page_set_close_cb(file_explorer_page_t *page, void (*cb)(void))
{
    if (page) {
        page->close_cb = cb;
    }
}

// 关闭事件处理函数
static void file_explorer_page_close_handler(void *user)
{
    file_explorer_page_t *page  = (file_explorer_page_t *)user;
    if (page && page->close_cb) {
        page->close_cb();
    }
}

static void file_explorer_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target_obj(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        const char* cur_path = lv_file_explorer_get_current_path(obj);
        const char* sel_fn = lv_file_explorer_get_selected_file_name(obj);

        LV_LOG_USER("%s%s", cur_path, sel_fn);
    }
}

#if LV_FILE_EXPLORER_QUICK_ACCESS
static void btn_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target_obj(e);
    lv_obj_t* file_explorer = (lv_obj_t*)lv_event_get_user_data(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(btn, LV_STATE_CHECKED))
            lv_obj_add_flag(file_explorer, LV_OBJ_FLAG_HIDDEN);
        else
            lv_obj_remove_flag(file_explorer, LV_OBJ_FLAG_HIDDEN);
    }
}

static void dd_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* dd = lv_event_get_target_obj(e);
    lv_obj_t* fe_quick_access_obj = (lv_obj_t*)lv_event_get_user_data(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_dropdown_get_selected_str(dd, buf, sizeof(buf));
        if (strcmp(buf, "NONE") == 0) {
            lv_file_explorer_set_sort(fe_quick_access_obj, LV_EXPLORER_SORT_NONE);
        }
        else if (strcmp(buf, "KIND") == 0) {
            lv_file_explorer_set_sort(fe_quick_access_obj, LV_EXPLORER_SORT_KIND);
        }
    }
}
#endif
