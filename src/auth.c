#include "auth.h"
#include "common.h"

// 管理员密码验证
int auth_admin(const char *pwd) {
    if (pwd == NULL) {
        return 0;
    }
    // 与common.h中定义的ADMIN_PWD比对
    return strcmp(pwd, ADMIN_PWD) == 0 ? 1 : 0;
}

// 普通用户密码验证
int auth_user(const char *pwd) {
    if (pwd == NULL) {
        return 0;
    }
    // 与common.h中定义的USER_PWD比对
    return strcmp(pwd, USER_PWD) == 0 ? 1 : 0;
}
