#include "lvgl.h"
#include "page_logo_icon.h"
#include "lv_app_conf.h"

static lv_obj_t *logo_img = NULL;
static lv_anim_t logo_anim;
static lv_anim_t scale_anim;  // 将scale_anim定义为全局变量
static lv_coord_t orig_width = 0;  // 保存原始宽度
static lv_coord_t orig_height = 0; // 保存原始高度

static void logo_fade_anim(void * obj, int32_t value)
{
    int32_t opa = (value * 255) / 256;  // 将0-256的值映射到0-255的透明度范围
    //LV_LOG_INFO("淡入动画值: %d, 透明度: %d", value, opa);
    lv_obj_set_style_img_opa(obj, opa, 0);
}

static void logo_scale_anim(void * obj, int32_t value)
{
    int32_t size = (value * 100) / 256;  // 将0-256的值映射到0-100的百分比
    lv_coord_t width = (orig_width * size) / 100;
    lv_coord_t height = (orig_height * size) / 100;
    //LV_LOG_INFO("缩放动画值: %d, 当前尺寸: %dx%d", value, width, height);
    lv_obj_set_size(obj, width, height);
    lv_obj_center(obj);  // 保持居中
}

static void logo_anim_ready_cb(lv_anim_t * a)
{
    LV_LOG_USER("动画完成，准备删除Logo");
    // 动画完成后的回调，可以在这里切换到主界面
    if(logo_img) {
        lv_obj_del(logo_img);
        logo_img = NULL;
        LV_LOG_USER("Logo已删除");
    }
}

void show_logo_animation(lv_obj_t * parent)
{
    LV_LOG_USER("开始创建Logo动画");
    logo_img = lv_img_create(parent);
    lv_img_set_src(logo_img, LOGO_ICON_PATH);
    
    // 检查图片是否加载成功
    const void *src = lv_img_get_src(logo_img);
    if(src == NULL) {
        LV_LOG_ERROR("Logo图片加载失败");
        return;
    }
    
    lv_obj_center(logo_img);
    lv_obj_set_style_img_opa(logo_img, 0, 0);
    
    // 获取原始尺寸
    lv_obj_update_layout(logo_img); /*Be sure the object's size is calculated*/
    orig_width = lv_obj_get_width(logo_img);
    orig_height = lv_obj_get_height(logo_img);
    
    // 检查尺寸是否有效
    if(orig_width == 0 || orig_height == 0) {
        LV_LOG_ERROR("无法获取Logo有效尺寸，使用默认尺寸");
        orig_width = 100;  // 设置默认宽度
        orig_height = 100; // 设置默认高度
    }
    
    // 设置基本样式属性
    lv_obj_set_size(logo_img, orig_width, orig_height);
    lv_obj_set_style_width(logo_img, orig_width, 0);
    lv_obj_set_style_height(logo_img, orig_height, 0);
    
    LV_LOG_USER("Logo原始尺寸: %dx%d", orig_width, orig_height);
    
    // 初始设置为最小尺寸
    lv_obj_set_size(logo_img, 1, 1);
    LV_LOG_USER("Logo初始化为最小尺寸");
    
    // 设置淡入动画
    LV_LOG_USER("配置淡入动画");
    lv_anim_init(&logo_anim);
    lv_anim_set_var(&logo_anim, logo_img);
    lv_anim_set_exec_cb(&logo_anim, logo_fade_anim);
    lv_anim_set_values(&logo_anim, 0, 256);  // 修改为0-256范围以匹配映射逻辑
    lv_anim_set_time(&logo_anim, 2000);  // 缩短动画时间
    lv_anim_set_path_cb(&logo_anim, lv_anim_path_ease_in);  // 使用ease-in-out效果
    lv_anim_start(&logo_anim);

    // 设置缩放动画
    LV_LOG_USER("配置缩放动画");
    lv_anim_init(&scale_anim);
    lv_anim_set_var(&scale_anim, logo_img);
    lv_anim_set_exec_cb(&scale_anim, logo_scale_anim);
    lv_anim_set_values(&scale_anim, 0, 256);
    lv_anim_set_time(&scale_anim, 2000);  // 与淡入动画保持一致
    lv_anim_set_path_cb(&scale_anim, lv_anim_path_overshoot);  // 使用overshoot效果
    lv_anim_set_ready_cb(&scale_anim, logo_anim_ready_cb);
    lv_anim_start(&scale_anim);
    LV_LOG_USER("Logo动画已启动");
}
