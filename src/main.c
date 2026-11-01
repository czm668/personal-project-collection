#include "common.h"
#include "menu.h"
#include "car.h"
#include "fee.h"
#include "month_card.h"
#include "park_record.h"
#include "auth.h"

// 权限验证函数实现
int verify_auth(const char *role, const char *password) {
    // 简化版：仅验证密码为123456，角色为admin/user
    if (strcmp(password, "123456") != 0) {
        return 0;
    }
    if (strcmp(role, "admin") != 0 && strcmp(role, "user") != 0) {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    // 程序启动时加载所有历史数据
    load_cars_from_file();
    load_fee_rules_from_file();
    load_month_cards_from_file();
    load_park_records_from_file();

    // 校验启动参数
    if (argc != 3) {
        printf("使用方式：%s <角色> <密码>\n", argv[0]);
        printf("角色：admin（管理员）/ user（普通用户）\n");
        printf("默认密码：123456\n");
        return -1;
    }

    // 权限验证
    if (!verify_auth(argv[1], argv[2])) {
        printf("角色或密码错误！\n");
        return -1;
    }

    char *role = argv[1];
    int choice;

    // 欢迎信息
    printf("=======================================\n");
    printf("欢迎%s %s 登录停车场管理系统！\n", 
           strcmp(role, "admin") == 0 ? "管理员" : "普通用户", role);
    printf("=======================================\n");

    // 菜单循环
    while (1) {
        // 展示对应菜单
        if (strcmp(role, "admin") == 0) {
            show_admin_menu();
        } else {
            show_user_menu();
        }

        // 读取操作编号
        while (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("输入错误！请输入数字：");
            fflush(stdout);
        }
        clear_input_buffer();

        // 处理操作
        if (strcmp(role, "admin") == 0) {
            handle_admin_oper(choice);
        } else {
            handle_user_oper(choice);
        }
    }

    return 0;
}
