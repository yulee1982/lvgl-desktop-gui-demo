#ifndef USERMGR_H
#define USERMGR_H

#include "sqlite3/sqlite3.h"
#include <stdbool.h>

// 用户信息结构体
typedef struct {
    int id;                 // 用户ID
    char username[32];      // 用户名
    char password[64];      // 密码（加密后的）
    char create_time[20];   // 创建时间
    bool is_active;         // 是否激活
} UserInfo;

// 初始化用户管理模块
bool usermgr_init(const char* db_path);

// 关闭用户管理模块
void usermgr_deinit(void);

// 添加新用户
bool usermgr_add_user(const char* username, const char* password);

// 删除用户
bool usermgr_delete_user(const char* username);

// 验证用户密码
bool usermgr_verify_password(const char* username, const char* password);

// 查询用户信息
bool usermgr_get_user_info(const char* username, UserInfo* user_info);

// 查询所有用户
int usermgr_get_all_users(UserInfo** users, int* count);

// 释放用户列表内存
void usermgr_free_user_list(UserInfo* users);

#endif // USERMGR_H
