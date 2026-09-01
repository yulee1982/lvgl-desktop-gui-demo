#include <stdlib.h>
#include "page_ctrl_page.h"

ctrl_page_t* g_ctrl_page = NULL;

// 前向声明
static void ctrl_page_close_handler(void *page);

lv_obj_t* ctrl_page_create(lv_obj_t * parent,close_cb_t close_cb) {
    // 分配内存
    ctrl_page_t *page = (ctrl_page_t *)lv_malloc(sizeof(ctrl_page_t));
    if (page == NULL) {
        return NULL;
    }
    lv_memset(page, 0, sizeof(ctrl_page_t));

    g_ctrl_page = page;

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
    titlebar_set_title(page->titlebar, "Smart Home Control");
    
    // 添加标题栏关闭按钮的事件处理
    titlebar_set_close_cb(page->titlebar, ctrl_page_close_handler, page);
    
    // 创建控制卡片的容器
    page->container = lv_obj_create(page->obj);
    lv_obj_set_size(page->container, 440, 180);
    lv_obj_align(page->container, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_flex_flow(page->container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(page->container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(page->container, 10, 0);
    lv_obj_set_style_pad_row(page->container, 10, 0);
    lv_obj_set_style_pad_column(page->container, 10, 0);
    
    // 灯光控制卡片
    page->light.card = lv_obj_create(page->container);
    lv_obj_set_size(page->light.card, 130, 160);
    lv_obj_set_style_radius(page->light.card, 10, 0);
    lv_obj_set_style_bg_color(page->light.card, lv_color_hex(0x2196F3), 0);
    lv_obj_set_style_bg_opa(page->light.card, LV_OPA_COVER, 0);
    
    page->light.icon = lv_label_create(page->light.card);
    lv_label_set_text(page->light.icon, LV_SYMBOL_POWER);
    lv_obj_align(page->light.icon, LV_ALIGN_TOP_MID, 0, 20);
    
    page->light.label = lv_label_create(page->light.card);
    lv_label_set_text(page->light.label, "Lighting");
    lv_obj_align(page->light.label, LV_ALIGN_TOP_MID, 0, 50);
    
    page->light.switch_obj = lv_switch_create(page->light.card);
    lv_obj_align(page->light.switch_obj, LV_ALIGN_BOTTOM_MID, 0, -20);
    page->light.state = false;
    
    // 温度控制卡片
    page->temp.card = lv_obj_create(page->container);
    lv_obj_set_size(page->temp.card, 130, 160);
    lv_obj_set_style_radius(page->temp.card, 10, 0);
    lv_obj_set_style_bg_color(page->temp.card, lv_color_hex(0xFF4081), 0);
    lv_obj_set_style_bg_opa(page->temp.card, LV_OPA_COVER, 0);
    
    page->temp.icon = lv_label_create(page->temp.card);
    lv_label_set_text(page->temp.icon, LV_SYMBOL_SETTINGS);
    lv_obj_align(page->temp.icon, LV_ALIGN_TOP_MID, 0, 20);
    
    page->temp.label = lv_label_create(page->temp.card);
    lv_label_set_text(page->temp.label, "Temperature");
    lv_obj_align(page->temp.label, LV_ALIGN_TOP_MID, 0, 50);
    
    page->temp.value = lv_label_create(page->temp.card);
    lv_label_set_text(page->temp.value, "25°C");
    lv_obj_align(page->temp.value, LV_ALIGN_CENTER, 0, 10);
    page->temp.temperature = 25;
    
    // 窗帘控制卡片
    page->curtain.card = lv_obj_create(page->container);
    lv_obj_set_size(page->curtain.card, 130, 160);
    lv_obj_set_style_radius(page->curtain.card, 10, 0);
    lv_obj_set_style_bg_color(page->curtain.card, lv_color_hex(0x4CAF50), 0);
    lv_obj_set_style_bg_opa(page->curtain.card, LV_OPA_COVER, 0);
    
    page->curtain.icon = lv_label_create(page->curtain.card);
    lv_label_set_text(page->curtain.icon, LV_SYMBOL_IMAGE);
    lv_obj_align(page->curtain.icon, LV_ALIGN_TOP_MID, 0, 20);
    
    page->curtain.label = lv_label_create(page->curtain.card);
    lv_label_set_text(page->curtain.label, "Curtain");
    lv_obj_align(page->curtain.label, LV_ALIGN_TOP_MID, 0, 50);
    
    page->curtain.slider = lv_slider_create(page->curtain.card);
    lv_obj_set_size(page->curtain.slider, 80, 10);
    lv_obj_align(page->curtain.slider, LV_ALIGN_BOTTOM_MID, 0, -20);
    page->curtain.position = 0;
    
    return page->obj;
}

// 设置关闭回调函数
void ctrl_page_set_close_cb(ctrl_page_t *page, void (*cb)(void)) {
    if (page) {
        page->close_cb = cb;
    }
}

// 关闭事件处理函数
static void ctrl_page_close_handler(void *user) {
    ctrl_page_t *page  = (ctrl_page_t *)user;
    if (page && page->close_cb) {
        page->close_cb();
    }
}
