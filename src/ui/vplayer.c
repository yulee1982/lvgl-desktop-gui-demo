#include "vplayer.h"
#include "lv_app_conf.h"
#include <stdlib.h>
/*
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
*/
// 按钮事件处理函数
static void play_btn_event_cb(lv_event_t *e)
{
    vplayer_t *vplayer = (vplayer_t *)lv_event_get_user_data(e);
    if (!vplayer->is_playing) {
        vplayer_play(vplayer);
    } else {
        vplayer_pause(vplayer);
    }
}

static void stop_btn_event_cb(lv_event_t *e)
{
    vplayer_t *vplayer = (vplayer_t *)lv_event_get_user_data(e);
    vplayer_stop(vplayer);
}

static void fullscreen_btn_event_cb(lv_event_t *e)
{
    vplayer_t *vplayer = (vplayer_t *)lv_event_get_user_data(e);
    vplayer_toggle_fullscreen(vplayer);
}

static void player_click_event_cb(lv_event_t *e)
{
    vplayer_t *vplayer = (vplayer_t *)lv_event_get_user_data(e);
    LV_LOG_USER("Player clicked, current fullscreen state: %s", vplayer->is_fullscreen ? "true" : "false");
    if (vplayer->is_fullscreen) {
        LV_LOG_USER("Exiting fullscreen mode");
        vplayer_toggle_fullscreen(vplayer);
    }
    LV_LOG_USER("Player click event handled");
}

static void vplayer_close_cb_handler(void *user_data)
{
    vplayer_t *vplayer = (vplayer_t *)user_data;
    if (vplayer){
        vplayer_stop(vplayer);
        if(vplayer->close_cb) {
            vplayer->close_cb();
        }
    }
}

vplayer_t *vplayer_create(lv_obj_t *parent)
{
    vplayer_t *vplayer = lv_malloc(sizeof(vplayer_t));
    if (vplayer == NULL) return NULL;

    lv_memset(vplayer, 0, sizeof(vplayer_t));

    // 初始化状态
    vplayer->is_playing = false;
    vplayer->video_width = 0;
    vplayer->video_height = 0;
    vplayer->video_path = NULL;
    vplayer->is_fullscreen = false;
    vplayer->close_cb = NULL;

    // 禁用父容器的滚动功能
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);

    // 创建标题栏
    vplayer->titlebar = titlebar_create(parent);
    lv_obj_set_size(vplayer->titlebar->cont, LV_HOR_RES, 20);
    lv_obj_align(vplayer->titlebar->cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(vplayer->titlebar->cont, lv_color_hex(0x808080), 0);
    lv_obj_set_style_bg_opa(vplayer->titlebar->cont, LV_OPA_50, 0);
    titlebar_set_title(vplayer->titlebar, "视频播放器");
    titlebar_set_close_cb(vplayer->titlebar, vplayer_close_cb_handler, vplayer);

    // 创建视频播放器容器
    vplayer->player = lv_obj_create(parent);
    lv_obj_set_size(vplayer->player, LV_HOR_RES, LV_VER_RES-70);
    lv_obj_align_to(vplayer->player, vplayer->titlebar->cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(vplayer->player, player_click_event_cb, LV_EVENT_CLICKED, vplayer);
    lv_obj_add_flag(vplayer->player, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(vplayer->player, 0, 0);
    lv_obj_set_style_border_width(vplayer->player, 0, 0);
    lv_obj_set_style_pad_all(vplayer->player, 0, 0);
    lv_obj_set_style_bg_color(vplayer->player, lv_color_black(), 0);

    // 创建实际显示视频的对象
    vplayer->video_obj = lv_ffmpeg_player_create(vplayer->player);
    lv_obj_center(vplayer->video_obj);
    lv_obj_set_size(vplayer->video_obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_radius(vplayer->video_obj, 0, 0);
    lv_obj_set_style_border_width(vplayer->video_obj, 0, 0);
    lv_obj_set_style_pad_all(vplayer->video_obj, 0, 0);

    // 创建工具栏
    vplayer->toolbar = lv_obj_create(parent);
    lv_obj_set_size(vplayer->toolbar, LV_HOR_RES, 50);
    lv_obj_align_to(vplayer->toolbar, vplayer->player, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(vplayer->toolbar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(vplayer->toolbar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(vplayer->toolbar, lv_color_hex(0x808080), 0);
    lv_obj_set_style_bg_opa(vplayer->toolbar, LV_OPA_50, 0);

    // 创建播放/暂停按钮
    vplayer->play_btn = lv_btn_create(vplayer->toolbar);
    lv_obj_set_size(vplayer->play_btn, 40, 40);
    lv_obj_add_event_cb(vplayer->play_btn, play_btn_event_cb, LV_EVENT_CLICKED, vplayer);
    lv_obj_t *play_label = lv_label_create(vplayer->play_btn);
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
    lv_obj_center(play_label);

    // 创建停止按钮
    vplayer->stop_btn = lv_btn_create(vplayer->toolbar);
    lv_obj_set_size(vplayer->stop_btn, 40, 40);
    lv_obj_add_event_cb(vplayer->stop_btn, stop_btn_event_cb, LV_EVENT_CLICKED, vplayer);
    lv_obj_t *stop_label = lv_label_create(vplayer->stop_btn);
    lv_label_set_text(stop_label, LV_SYMBOL_STOP);
    lv_obj_center(stop_label);

    // 创建全屏按钮
    vplayer->fullscreen_btn = lv_btn_create(vplayer->toolbar);
    lv_obj_set_size(vplayer->fullscreen_btn, 40, 40);
    lv_obj_add_event_cb(vplayer->fullscreen_btn, fullscreen_btn_event_cb, LV_EVENT_CLICKED, vplayer);
    lv_obj_t *fullscreen_label = lv_label_create(vplayer->fullscreen_btn);
    lv_label_set_text(fullscreen_label, LV_SYMBOL_PLUS);
    lv_obj_center(fullscreen_label);

    return vplayer;
}

lv_res_t vplayer_set_src(vplayer_t *vplayer, const char *path)
{
    lv_res_t res = LV_RES_INV;

    if (vplayer->video_path) {
        free(vplayer->video_path);
        vplayer->video_path = NULL;
    }

    vplayer->video_path = strdup(path);
    if (!vplayer->video_path) {
        LV_LOG_ERROR("Failed to allocate memory for video path");
        return LV_RES_INV;
    }

    res = lv_ffmpeg_player_set_src(vplayer->video_obj, path);
    if (res != LV_RES_OK) {
        free(vplayer->video_path);
        vplayer->video_path = NULL;
        LV_LOG_ERROR("Failed to set video path");
        return res;
    }
    lv_ffmpeg_player_set_auto_restart(vplayer->video_obj, true);
/*
    // 从ffmpeg播放器对象中获取视频原始宽高
    lv_image_dsc_t * video_imgdsc = lv_get_video_imgdsc_from_ffmpeg();
    if (video_imgdsc) {
        vplayer->video_width = video_imgdsc->header.w;
        vplayer->video_height = video_imgdsc->header.h;
        LV_LOG_USER("video_src_width: %d, video_src_height: %d", vplayer->video_width, vplayer->video_height);
    } else {
        LV_LOG_ERROR("Failed to get video dimensions");
        free(vplayer->video_path);
        vplayer->video_path = NULL;
        return LV_RES_INV;
    }

    // 更新布局以确保获取正确的容器尺寸
    lv_obj_update_layout(vplayer->player);

    // 根据视频宽高比调整显示大小
    lv_coord_t container_w = lv_obj_get_width(vplayer->player);
    lv_coord_t container_h = lv_obj_get_height(vplayer->player);
    lv_coord_t video_w = 0, video_h = 0;

    if (vplayer->video_width > 0 && vplayer->video_height > 0) {
        if (container_w * vplayer->video_height > container_h * vplayer->video_width) {
            // 以高度为基准
            video_h = container_h;
            video_w = (container_h * vplayer->video_width) / vplayer->video_height;
            // 确保不超过原始尺寸
            if (video_h > vplayer->video_height) {
                video_h = vplayer->video_height;
                video_w = vplayer->video_width;
            }
        } else {
            // 以宽度为基准
            video_w = container_w;
            video_h = (container_w * vplayer->video_height) / vplayer->video_width;
            // 确保不超过原始尺寸
            if (video_w > vplayer->video_width) {
                video_w = vplayer->video_width;
                video_h = vplayer->video_height;
            }
        }
    }

    lv_obj_set_size(vplayer->video_obj, video_w, video_h);
*/
    lv_obj_center(vplayer->video_obj);

    //LV_LOG_USER("vide obj,video_width: %d, video_height: %d", video_w,video_h);
 
    return res;
}

void vplayer_play(vplayer_t *vplayer)
{
    lv_ffmpeg_player_set_cmd(vplayer->video_obj, LV_FFMPEG_PLAYER_CMD_START);
    vplayer->is_playing = true;
    lv_obj_t *play_label = lv_obj_get_child(vplayer->play_btn, 0);
    lv_label_set_text(play_label, LV_SYMBOL_PAUSE);
}

void vplayer_pause(vplayer_t *vplayer)
{
    lv_ffmpeg_player_set_cmd(vplayer->video_obj, LV_FFMPEG_PLAYER_CMD_PAUSE);
    vplayer->is_playing = false;
    lv_obj_t *play_label = lv_obj_get_child(vplayer->play_btn, 0);
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
}

void vplayer_stop(vplayer_t *vplayer)
{
    lv_ffmpeg_player_set_cmd(vplayer->video_obj, LV_FFMPEG_PLAYER_CMD_STOP);
    vplayer->is_playing = false;
    lv_obj_t *play_label = lv_obj_get_child(vplayer->play_btn, 0);
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);
}

void vplayer_toggle_fullscreen(vplayer_t *vplayer)
{
    if (!vplayer->is_fullscreen) {
        // 进入全屏模式
        lv_obj_set_size(vplayer->player, LV_HOR_RES, LV_VER_RES);
        lv_obj_align(vplayer->player, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_flag(vplayer->toolbar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(vplayer->titlebar->cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_t *fullscreen_label = lv_obj_get_child(vplayer->fullscreen_btn, 0);
        lv_label_set_text(fullscreen_label, LV_SYMBOL_MINUS);
    } else {
        // 退出全屏模式
        lv_obj_set_size(vplayer->player, LV_HOR_RES, LV_VER_RES-90);
        lv_obj_align_to(vplayer->player, vplayer->titlebar->cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_clear_flag(vplayer->toolbar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(vplayer->titlebar->cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_t *fullscreen_label = lv_obj_get_child(vplayer->fullscreen_btn, 0);
        lv_label_set_text(fullscreen_label, LV_SYMBOL_PLUS);
    }
    vplayer->is_fullscreen = !vplayer->is_fullscreen;

    // 更新布局以确保获取正确的容器尺寸
    lv_obj_update_layout(vplayer->player);
    
    // 根据视频宽高比调整显示大小
    lv_coord_t container_w = lv_obj_get_width(vplayer->player);
    lv_coord_t container_h = lv_obj_get_height(vplayer->player);
    lv_coord_t video_w = 0, video_h = 0;

    if (vplayer->video_width > 0 && vplayer->video_height > 0) {
        if (container_w * vplayer->video_height > container_h * vplayer->video_width) {
            // 以高度为基准
            video_h = container_h;
            video_w = (container_h * vplayer->video_width) / vplayer->video_height;
            // 确保不超过原始尺寸
            if (video_h > vplayer->video_height) {
                video_h = vplayer->video_height;
                video_w = vplayer->video_width;
            }
        } else {
            // 以宽度为基准
            video_w = container_w;
            video_h = (container_w * vplayer->video_height) / vplayer->video_width;
            // 确保不超过原始尺寸
            if (video_w > vplayer->video_width) {
                video_w = vplayer->video_width;
                video_h = vplayer->video_height;
            }
        }
    }
    
    lv_obj_set_size(vplayer->video_obj, video_w, video_h);
    lv_obj_center(vplayer->video_obj);

    LV_LOG_USER("video obj size in %s mode: width=%d, height=%d", 
                vplayer->is_fullscreen ? "fullscreen" : "normal", 
                video_w, video_h);
}

void vplayer_set_close_cb(vplayer_t *vplayer, vplayer_close_cb_t cb)
{
    if (vplayer) {
        vplayer->close_cb = cb;
    }
}
