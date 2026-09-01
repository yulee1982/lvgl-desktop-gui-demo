#include "desktop_usermgr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lvgl/src/misc/lv_log.h"

// 数据库连接句柄
static sqlite3* db = NULL;

// 创建用户表的SQL语句
static const char* CREATE_TABLE_SQL = 
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "username TEXT UNIQUE NOT NULL,"
    "password TEXT NOT NULL,"
    "create_time TEXT NOT NULL,"
    "is_active INTEGER DEFAULT 1"
    ")";

// 获取当前时间字符串
static void get_current_time(char* time_str, size_t size)
{
    time_t now;
    struct tm* timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(time_str, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// 初始化用户管理模块
bool usermgr_init(const char* db_path)
{
    bool is_new_db = false;
    LV_LOG_INFO("Initializing user management module with database: %s", db_path);

    // 检查数据库文件是否存在
    FILE* file = fopen(db_path, "r");
    if (file == NULL) {
        // 数据库文件不存在，创建一个新的数据库文件
        file = fopen(db_path, "w");
        if (file == NULL) {
            LV_LOG_ERROR("Cannot create database file: %s", db_path);
            return false;
        }
        is_new_db = true;
        fclose(file);
    } else {
        fclose(file);
    }

    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Cannot open database: %s", sqlite3_errmsg(db));
        return false;
    }

    // 创建用户表
    char* err_msg = NULL;
    rc = sqlite3_exec(db, CREATE_TABLE_SQL, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("SQL error: %s", err_msg);
        sqlite3_free(err_msg);
        return false;
    }

    // 如果是新创建的数据库文件，添加默认的admin用户
    if (is_new_db) {
        LV_LOG_INFO("Creating default admin user");
        if (!usermgr_add_user("admin", "123456")) {
            LV_LOG_ERROR("Failed to add default admin user");
            return false;
        }
    }

    LV_LOG_INFO("User management module initialized successfully");

    return true;
}

// 关闭用户管理模块
void usermgr_deinit(void)
{
    if (db) {
        LV_LOG_INFO("Closing user management module");
        sqlite3_close(db);
        db = NULL;
    }
}

// 添加新用户
bool usermgr_add_user(const char* username, const char* password) {
    if (!db || !username || !password) {
        LV_LOG_ERROR("Invalid parameters for adding user");
        return false;
    }

    LV_LOG_INFO("Adding new user: %s", username);

    const char* sql = "INSERT INTO users (username, password, create_time) VALUES (?, ?, ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Failed to prepare SQL statement");
        return false;
    }

    char time_str[20];
    get_current_time(time_str, sizeof(time_str));

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, time_str, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        LV_LOG_INFO("User %s added successfully", username);
        return true;
    } else {
        LV_LOG_ERROR("Failed to add user %s", username);
        return false;
    }
}

// 删除用户
bool usermgr_delete_user(const char* username)
{
    if (!db || !username) {
        LV_LOG_ERROR("Invalid parameters for deleting user");
        return false;
    }

    LV_LOG_INFO("Deleting user: %s", username);

    const char* sql = "DELETE FROM users WHERE username = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Failed to prepare SQL statement");
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        LV_LOG_INFO("User %s deleted successfully", username);
        return true;
    } else {
        LV_LOG_ERROR("Failed to delete user %s", username);
        return false;
    }

}

// 验证用户密码
bool usermgr_verify_password(const char* username, const char* password)
{
    if (!db || !username || !password) {
        LV_LOG_ERROR("Invalid parameters for password verification");
        return false;
    }

    LV_LOG_INFO("Verifying password for user: %s", username);

    const char* sql = "SELECT password FROM users WHERE username = ? AND is_active = 1";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Failed to prepare SQL statement");
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    bool result = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* stored_password = (const char*)sqlite3_column_text(stmt, 0);
        result = strcmp(password, stored_password) == 0;
        if (result) {
            LV_LOG_INFO("Password verification successful for user %s", username);
        } else {
            LV_LOG_WARN("Password verification failed for user %s", username);
        }
    } else {
        LV_LOG_WARN("User %s not found or inactive", username);
    }

    sqlite3_finalize(stmt);
    return result;
}

// 查询用户信息
bool usermgr_get_user_info(const char* username, UserInfo* user_info)
{
    if (!db || !username || !user_info) {
        LV_LOG_ERROR("Invalid parameters for getting user info");
        return false;
    }

    LV_LOG_INFO("Getting user info for: %s", username);

    const char* sql = "SELECT id, username, password, create_time, is_active FROM users WHERE username = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Failed to prepare SQL statement");
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    bool result = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user_info->id = sqlite3_column_int(stmt, 0);
        strncpy(user_info->username, (const char*)sqlite3_column_text(stmt, 1), sizeof(user_info->username) - 1);
        strncpy(user_info->password, (const char*)sqlite3_column_text(stmt, 2), sizeof(user_info->password) - 1);
        strncpy(user_info->create_time, (const char*)sqlite3_column_text(stmt, 3), sizeof(user_info->create_time) - 1);
        user_info->is_active = sqlite3_column_int(stmt, 4) != 0;
        result = true;
        LV_LOG_INFO("Successfully retrieved user info for %s", username);
    } else {
        LV_LOG_WARN("User %s not found", username);
    }

    sqlite3_finalize(stmt);
    return result;
}

// 查询所有用户
int usermgr_get_all_users(UserInfo** users, int* count)
{
    if (!db || !users || !count) {
        LV_LOG_ERROR("Invalid parameters for getting all users");
        return -1;
    }

    LV_LOG_INFO("Getting all users");

    const char* sql = "SELECT id, username, password, create_time, is_active FROM users";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LV_LOG_ERROR("Failed to prepare SQL statement");
        return -1;
    }

    // 计算用户数量
    *count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*count)++;
    }
    sqlite3_reset(stmt);

    // 分配内存
    *users = (UserInfo*)malloc(sizeof(UserInfo) * (*count));
    if (!*users) {
        LV_LOG_ERROR("Failed to allocate memory for user list");
        sqlite3_finalize(stmt);
        return -1;
    }

    // 读取用户信息
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        UserInfo* user = &(*users)[index++];
        user->id = sqlite3_column_int(stmt, 0);
        strncpy(user->username, (const char*)sqlite3_column_text(stmt, 1), sizeof(user->username) - 1);
        strncpy(user->password, (const char*)sqlite3_column_text(stmt, 2), sizeof(user->password) - 1);
        strncpy(user->create_time, (const char*)sqlite3_column_text(stmt, 3), sizeof(user->create_time) - 1);
        user->is_active = sqlite3_column_int(stmt, 4) != 0;
    }

    sqlite3_finalize(stmt);

    LV_LOG_INFO("Successfully retrieved %d users", *count);
    return 0;
}

// 释放用户列表内存
void usermgr_free_user_list(UserInfo* users)
{
    if (users) {
        LV_LOG_INFO("Freeing user list memory");
        free(users);
    }
}
