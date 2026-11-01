#ifndef AUTH_H
#define AUTH_H

// 管理员密码验证（参数：输入的密码）
int auth_admin(const char *pwd);

// 普通用户密码验证（参数：输入的密码）
int auth_user(const char *pwd);

#endif // AUTH_H
