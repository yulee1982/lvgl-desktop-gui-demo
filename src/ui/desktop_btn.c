#include "desktop_btn.h"

desktop_btn_t *desktop_btn_create(lv_obj_t *parent, const char *title, const char *icon_path)
{
    // 分配内存
    desktop_btn_t *btn = lv_malloc(sizeof(desktop_btn_t));
    if (btn == NULL) {
        return NULL;
    }
    
    // 创建按钮对象
    btn->btn = lv_btn_create(parent);
    if (btn->btn == NULL) {
        lv_free(btn);
        return NULL;
    }
    
    // 设置按钮样式
    lv_obj_set_size(btn->btn, 52, 68);  //(btn->btn, 52, 64)
    lv_obj_set_style_pad_all(btn->btn, 2, 0);
    lv_obj_set_style_bg_opa(btn->btn, LV_OPA_0, 0);  // 设置按钮背景完全透明

    // 创建图标
    btn->icon = lv_img_create(btn->btn);
    lv_img_set_src(btn->icon, icon_path);
    lv_obj_set_size(btn->icon, 48, 48);

    // 创建标题标签
    btn->label = lv_label_create(btn->btn);
    lv_label_set_text(btn->label, title);
    lv_obj_set_style_text_color(btn->label, lv_color_black(), 0);
    lv_obj_set_style_pad_all(btn->label, 0, 0);
    lv_obj_align(btn->label, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_align(btn->label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(btn->label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(btn->label, 48);
    // 保存标题
    btn->title = title;
    
    return btn;
}

void desktop_btn_set_pos(desktop_btn_t *btn, lv_coord_t x, lv_coord_t y) {
    if (btn && btn->btn) {
        lv_obj_set_pos(btn->btn, x, y);
    }
}

void desktop_btn_add_event_cb(desktop_btn_t *btn, lv_event_cb_t event_cb, void *user_data) {
    if (btn && btn->btn) {
        lv_obj_add_event_cb(btn->btn, event_cb, LV_EVENT_CLICKED, user_data);
    }
}

bool desktop_btn_recognize_btn(desktop_btn_t *btn,lv_obj_t *obj) {
    if (btn && btn->btn && obj) {
        return (btn->btn == obj);
    }
    return false;
}
