// LVGL version: 8.3.5
// Project name: phoneui

#include "ui.h"
#include "ui_helpers.h"
#include "lvgl/lvgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "lv_app_conf.h"
#include "page_manager.h"
#include "desktop_usermgr.h"

// 信号处理函数，用于安全退出和处理段错误
static void signal_handler(int signo) {
    printf("\n=========>>>catch signal %d <<<=========\n", signo);

    if (signo == SIGINT || signo == SIGTERM || signo == SIGSEGV) {
        //backtrace_print();
        fflush(stdout);

        // 关闭日志文件
        //lv_log_file_close();

        exit(0);
    }
}

void desktop_init(void)
{
    // 注册SIGSEGV信号处理函数
    signal(SIGSEGV, signal_handler);
    signal(SIGTERM, signal_handler);

    // 初始化用户管理模块
    if (!usermgr_init(DB_USER_FILE_PATH)) {
        fprintf(stderr, "Failed to initialize user management module\n");
        return -1;
    }

    // 初始化页面管理器
    page_manager_init();
}
