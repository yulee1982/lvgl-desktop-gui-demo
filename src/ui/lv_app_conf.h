#ifndef LV_APP_CONF_H
#define LV_APP_CONF_H
#include "lv_conf.h"


/* 根目录定义 */
#define LV_ROOT_RESOURCES_PATH  "A:./deskgui_res" //"A:../resource"
#define ROOT_RESOURCES_PATH "/z_lvgl/deskgui_res" //"../resource"

/* 数据库文件路径定义 */
#define DB_FILE_PATH ROOT_RESOURCES_PATH"/test.db"

/* 用户数据库文件路径定义 */
#define DB_USER_FILE_PATH ROOT_RESOURCES_PATH"/user.db"

#if LV_USE_FFMPEG && !LV_FFMPEG_PLAYER_USE_LV_FS
/* Logo图标文件路径定义 */
#define LOGO_ICON_PATH ROOT_RESOURCES_PATH"/images/logo_icon.png"

/* mp4视频文件路径定义 */
#define VIDEO_TEST_FILE_PATH ROOT_RESOURCES_PATH"/video/birds.mp4"

/* 桌面背景和图标 */
#define DESKTOP_BG_PATH ROOT_RESOURCES_PATH"/images/desktop/desktop_bg.png"
#define DESKTOP_ICON_DIR ROOT_RESOURCES_PATH"/images/desktop"

#else
/* Logo图标文件路径定义 */
#define LOGO_ICON_PATH LV_ROOT_RESOURCES_PATH"/images/logo_icon.png"

/* mp4视频文件路径定义 */
#define VIDEO_TEST_FILE_PATH LV_ROOT_RESOURCES_PATH"/video/birds.mp4"

/* 桌面背景和图标 */
#define DESKTOP_BG_PATH LV_ROOT_RESOURCES_PATH"/images/desktop/desktop_bg.png"
#define DESKTOP_ICON_DIR LV_ROOT_RESOURCES_PATH"/images/desktop"

#endif


#endif /* LV_APP_CONF_H */
