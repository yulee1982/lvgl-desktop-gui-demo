#ifndef VPLAYER_H
#define VPLAYER_H

#include "lvgl/lvgl.h"
#include "ui.h"
#include "desktop_titlebar.h"

// 视频播放器关闭回调函数类型
typedef void (*vplayer_close_cb_t)(void);

// 视频播放器组件结构体
typedef struct {
    lv_obj_t *player;         // 视频播放器容器对象
    lv_obj_t *video_obj;      // 实际显示视频的对象
    lv_obj_t *toolbar;        // 工具栏对象
    lv_obj_t *play_btn;       // 播放/暂停按钮
    lv_obj_t *stop_btn;       // 停止按钮
    lv_obj_t *fullscreen_btn; // 全屏按钮
    titlebar_t *titlebar;     // 标题栏对象
    bool is_playing;          // 播放状态
    uint16_t video_width;     // 视频原始宽度
    uint16_t video_height;    // 视频原始高度
    char *video_path;         // 视频源路径
    bool is_fullscreen;       // 全屏状态
    vplayer_close_cb_t close_cb; // 关闭回调函数
} vplayer_t;

/**
 * 创建视频播放器组件
 * @param parent 父对象
 * @return 返回视频播放器组件对象
 */
vplayer_t *vplayer_create(lv_obj_t *parent);

/**
 * 设置视频源
 * @param vplayer 视频播放器组件对象
 * @param path 视频文件路径
 * @return 返回设置结果
 */
lv_res_t vplayer_set_src(vplayer_t *vplayer, const char *path);

/**
 * 开始播放视频
 * @param vplayer 视频播放器组件对象
 */
void vplayer_play(vplayer_t *vplayer);

/**
 * 暂停播放视频
 * @param vplayer 视频播放器组件对象
 */
void vplayer_pause(vplayer_t *vplayer);

/**
 * 停止播放视频
 * @param vplayer 视频播放器组件对象
 */
void vplayer_stop(vplayer_t *vplayer);

/**
 * 切换全屏状态
 * @param vplayer 视频播放器组件对象
 */
void vplayer_toggle_fullscreen(vplayer_t *vplayer);

/**
 * 设置视频播放器关闭回调函数
 * @param vplayer 视频播放器组件对象
 * @param cb 关闭回调函数
 */
void vplayer_set_close_cb(vplayer_t *vplayer, vplayer_close_cb_t cb);

#endif // VPLAYER_H
