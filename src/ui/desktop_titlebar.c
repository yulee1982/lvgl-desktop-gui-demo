#include "desktop_titlebar.h"

// 关闭按钮点击事件回调
static void close_btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        titlebar_t *titlebar = (titlebar_t *)lv_event_get_user_data(e);
        if (titlebar && titlebar->close_cb) {
            titlebar->close_cb(titlebar->user_data);
        }
    }
}

titlebar_t *titlebar_create(lv_obj_t *parent) {
    // 分配内存
    titlebar_t *titlebar = (titlebar_t *)lv_malloc(sizeof(titlebar_t));
    if (titlebar == NULL) {
        return NULL;
    }

    // 创建容器
    titlebar->cont = lv_obj_create(parent);
    if (titlebar->cont == NULL) {
        lv_free(titlebar);
        return NULL;
    }

    // 设置容器样式
    lv_obj_set_size(titlebar->cont, LV_PCT(100), 40);
    lv_obj_set_style_bg_color(titlebar->cont, lv_color_hex(0x2196F3), 0);
    lv_obj_set_style_bg_opa(titlebar->cont, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(titlebar->cont, 5, 0);

    // 创建标题文本
    titlebar->title = lv_label_create(titlebar->cont);
    lv_obj_align(titlebar->title, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(titlebar->title, lv_color_white(), 0);

    // 创建关闭按钮
    titlebar->close_btn = lv_btn_create(titlebar->cont);
    lv_obj_set_size(titlebar->close_btn, 30, 30);
    lv_obj_align(titlebar->close_btn, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_style_bg_color(titlebar->close_btn, lv_color_hex(0xFF4081), 0);
    lv_obj_add_event_cb(titlebar->close_btn, close_btn_event_cb, LV_EVENT_CLICKED, titlebar);

    // 创建关闭按钮图标
    lv_obj_t *close_label = lv_label_create(titlebar->close_btn);
    lv_label_set_text(close_label, LV_SYMBOL_CLOSE);
    lv_obj_center(close_label);

    // 初始化其他成员
    titlebar->icon = NULL;
    titlebar->close_cb = NULL;

    return titlebar;
}

void titlebar_set_title(titlebar_t *titlebar, const char *text) {
    if (titlebar && titlebar->title) {
        lv_label_set_text(titlebar->title, text);
    }
}

void titlebar_set_icon(titlebar_t *titlebar, const char *img_src) {
    if (titlebar == NULL) return;

    // 如果已存在图标，先删除
    if (titlebar->icon) {
        lv_obj_del(titlebar->icon);
        titlebar->icon = NULL;
    }

    // 创建新图标
    if (img_src) {
        titlebar->icon = lv_img_create(titlebar->cont);
        lv_img_set_src(titlebar->icon, img_src);
        lv_obj_align(titlebar->icon, LV_ALIGN_LEFT_MID, 5, 0);
    }
}

void titlebar_set_close_cb(titlebar_t *titlebar, void (*cb)(void *), void *user_data) {
    if (titlebar) {
        titlebar->close_cb = cb;
        titlebar->user_data = user_data;
    }
}
