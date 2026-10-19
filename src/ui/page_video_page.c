#include "page_video_page.h"
#include "lv_app_conf.h"
#include "vplayer.h"

video_page_t *g_video_page = NULL;
static void vplayer_close_cb(void)
{
    if (g_video_page && g_video_page->close_cb) {
        g_video_page->close_cb();
    }
}

lv_obj_t *video_page_create(lv_obj_t * parent,close_cb_t close_cb) {
    // 分配视频页面结构体内存
    video_page_t *video_page = (video_page_t *)lv_malloc(sizeof(video_page_t));
    if (video_page == NULL) {
        return NULL;
    }
    lv_memset(video_page, 0, sizeof(video_page_t));
    g_video_page = video_page;

    // 初始化结构体成员
    video_page->obj = lv_obj_create(parent);
    lv_obj_set_size(video_page->obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_border_width(video_page->obj, 0, 0);
    lv_obj_set_style_pad_all(video_page->obj, 0, 0);
    
    video_page->parent = parent;
    video_page->close_cb = close_cb;
    
    // 创建视频播放器组件
    video_page->vplayer = vplayer_create(video_page->obj);
    if (video_page->vplayer == NULL) {
        lv_free(video_page);
        return NULL;
    }
    
    // 设置视频源
    lv_res_t res = vplayer_set_src(video_page->vplayer, VIDEO_TEST_FILE_PATH);
    
    // 如果视频加载失败，显示错误信息
    if (res != LV_RES_OK) {
        lv_obj_t *error_label = lv_label_create(video_page->obj);
        lv_label_set_text(error_label, "Error: Cannot load video file");
        lv_obj_align(error_label, LV_ALIGN_CENTER, 0, 0);
        //vplayer_delete(video_page->vplayer);
        lv_free(video_page);
        return NULL;
    }

    // 注册关闭回调函数
    vplayer_set_close_cb(video_page->vplayer, vplayer_close_cb);
    
    return video_page->obj;
}

void video_page_set_close_cb(video_page_t *video_page, close_cb_t close_cb) {
    if (video_page) {
        video_page->close_cb = close_cb;
    }
}

void video_page_show(void) {

}
