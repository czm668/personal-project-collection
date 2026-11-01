#include "menu.h"
#include "car.h"
#include "park_record.h"
#include "month_card.h"
#include "fee.h"

// 清空输入缓冲区残留字符
void clear_input_buffer() {
    while (getchar() != '\n');
}

// 校验车牌号合法性
int is_valid_plate_num(const char *plate) {
    if (plate == NULL || strlen(plate) == 0 || strlen(plate) > MAX_PLATE_NUM) {
        return 0;
    }
    return 1;
}

// 校验时段类型合法性
int is_valid_fee_type(int type) {
    return (type == 0 || type == 1 || type == 2) ? 1 : 0;
}

// 校验费率合法性
int is_valid_rate(float rate) {
    return (rate >= MIN_RATE) ? 1 : 0;
}

// 校验余额合法性
int is_valid_balance(float balance) {
    return (balance >= MIN_BALANCE) ? 1 : 0;
}

// 校验车辆类型合法性
int is_valid_car_type(int type) {
    return (type == 0 || type == 1) ? 1 : 0;
}

// 校验续费月数合法性
int is_valid_renew_months(int months) {
    return (months >= MIN_RENEW_MONTHS && months <= MAX_RENEW_MONTHS) ? 1 : 0;
}

// 管理员菜单展示
void show_admin_menu() {
    printf("\n======= 停车场管理员菜单 =======\n");
    printf("1. 添加计费规则\n");
    printf("2. 开通月卡用户\n");
    printf("3. 车辆入场登记\n");
    printf("4. 车辆出场结算\n");
    printf("5. 查看所有停车记录\n");
    printf("6. 保存所有数据到文件\n");
    printf("7. 删除月卡用户\n");
    printf("8. 删除计费规则\n");
    printf("0. 退出系统\n");
    printf("===============================\n");
    printf("请输入操作编号：");
    fflush(stdout);
}

// 普通用户菜单展示
void show_user_menu() {
    printf("\n======= 停车场用户菜单 =======\n");
    printf("1. 查询我的停车记录\n");
    printf("2. 缴纳停车费\n");
    printf("3. 月卡续费\n");
    printf("0. 退出系统\n");
    printf("===============================\n");
    printf("请输入操作编号：");
    fflush(stdout);
}

// 处理管理员操作
void handle_admin_oper(int choice) {
    char plate_num[MAX_PLATE_NUM];
    int type;
    float rate, balance;
    time_t expire_time;
    Car *car = NULL;
    float fee;
    char del_plate_num[MAX_PLATE_NUM];
    int del_type;

    switch (choice) {
        case 1: // 添加计费规则
            do {
                printf("请输入时段类型（0-白天，1-夜间，2-节假日）：");
                fflush(stdout);
                while (scanf("%d", &type) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入数字（0/1/2）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_fee_type(type)) {
                    printf("时段类型错误！仅支持0（白天）、1（夜间）、2（节假日）\n");
                }
            } while (!is_valid_fee_type(type));

            do {
                printf("请输入该时段费率（元/小时，最低%.2f元）：", MIN_RATE);
                fflush(stdout);
                while (scanf("%f", &rate) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入合法的数字（如2.5）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_rate(rate)) {
                    printf("费率错误！最低费率为%.2f元/小时\n", MIN_RATE);
                }
            } while (!is_valid_rate(rate));

            add_fee_rule(create_fee_rule(type, rate));
            break;

        case 2: // 开通月卡用户
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            do {
                printf("请输入初始余额（元，最低%.2f元）：", MIN_BALANCE);
                fflush(stdout);
                while (scanf("%f", &balance) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入合法的数字（如100）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_balance(balance)) {
                    printf("余额错误！最低余额为%.2f元\n", MIN_BALANCE);
                }
            } while (!is_valid_balance(balance));

            expire_time = time(NULL) + 30 * 24 * 3600;
            add_month_card(create_month_card(plate_num, expire_time, balance));
            printf("月卡用户%s开通成功！\n", plate_num);
            break;

        case 3: // 车辆入场登记
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            do {
                printf("请输入车辆类型（0-临时车，1-月卡车）：");
                fflush(stdout);
                while (scanf("%d", &type) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入数字（0/1）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_car_type(type)) {
                    printf("车辆类型错误！仅支持0（临时车）、1（月卡车）\n");
                }
            } while (!is_valid_car_type(type));

            car = create_car(plate_num, type, time(NULL));
            if (car != NULL) {
                enqueue_car(car);
                printf("车辆%s入场登记成功！\n", plate_num);
            } else {
                printf("车辆%s入场登记失败！\n", plate_num);
            }
            break;

        case 4: // 车辆出场结算
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            car = dequeue_car(plate_num);
            if (car != NULL) {
                fee = calculate_fee(car);
                add_park_record(create_park_record(plate_num, car->in_time, car->out_time, fee, 0));
                free(car);
                printf("车辆%s出场结算成功！应缴费用：%.2f元\n", plate_num, fee);
            }
            break;

        case 5: // 查看所有停车记录
            if (park_record_list == NULL) {
                printf("暂无停车记录！\n");
                break;
            }
            printf("\n======= 所有停车记录 =======\n");
            printf("车牌号\t入场时间\t\t出场时间\t\t费用\t缴费状态\n");
            printf("------------------------------------------------------------\n");
            ParkRecord *temp = park_record_list;
            while (temp != NULL) {
                printf("%s\t%s\t%s\t%.2f\t%s\n",
                       temp->plate_num,
                       time_to_str(temp->in_time),
                       time_to_str(temp->out_time),
                       temp->fee,
                       temp->is_paid ? "已缴" : "未缴");
                temp = temp->next;
            }
            printf("============================\n");
            break;

        case 6: // 保存所有数据
            save_cars_to_file();
            save_fee_rules_to_file();
            save_month_cards_to_file();
            save_park_records_to_file();
            printf("所有数据已保存到本地文件！\n");
            break;

        case 7: // 删除月卡用户
            do {
                printf("请输入要删除的月卡用户车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", del_plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(del_plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(del_plate_num));

            if (delete_month_card(del_plate_num)) {
                printf("月卡用户%s删除成功！\n", del_plate_num);
            } else {
                printf("未找到月卡用户%s！\n", del_plate_num);
            }
            break;

        case 8: // 删除计费规则
            do {
                printf("请输入要删除的时段类型（0-白天，1-夜间，2-节假日）：");
                fflush(stdout);
                while (scanf("%d", &del_type) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入数字（0/1/2）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_fee_type(del_type)) {
                    printf("时段类型错误！仅支持0（白天）、1（夜间）、2（节假日）\n");
                }
            } while (!is_valid_fee_type(del_type));

            if (delete_fee_rule(del_type)) {
                printf("时段%d的计费规则删除成功！\n", del_type);
            } else {
                printf("未找到时段%d的计费规则！\n", del_type);
            }
            break;

        case 0: // 退出
            free_car_list();
            free_fee_rule_list();
            free_month_card_list();
            free_park_record_list();
            printf("管理员退出，系统已释放资源，再见！\n");
            exit(0);

        default:
            printf("输入错误！仅支持0-8的操作编号\n");
            clear_input_buffer();
            break;
    }
}

// 处理用户操作
void handle_user_oper(int choice) {
    char plate_num[MAX_PLATE_NUM];
    int months;
    ParkRecord *record = NULL;

    switch (choice) {
        case 1: // 查询我的停车记录
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            record = find_park_record(plate_num);
            if (record != NULL) {
                printf("\n======= 停车记录 =======\n");
                printf("车牌号：%s\n", record->plate_num);
                printf("入场时间：%s\n", time_to_str(record->in_time));
                printf("出场时间：%s\n", time_to_str(record->out_time));
                printf("应缴费用：%.2f元\n", record->fee);
                printf("缴费状态：%s\n", record->is_paid ? "已缴" : "未缴");
                printf("========================\n");
            } else {
                printf("未找到车牌号%s的停车记录！\n", plate_num);
            }
            break;

        case 2: // 缴纳停车费
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            if (pay_park_fee(plate_num)) {
                printf("车牌号%s缴费成功！\n", plate_num);
            } else {
                printf("车牌号%s缴费失败（无未缴记录）！\n", plate_num);
            }
            break;

        case 3: // 月卡续费
            do {
                printf("请输入车牌号（最长%d位）：", MAX_PLATE_NUM);
                fflush(stdout);
                scanf("%s", plate_num);
                clear_input_buffer();
                if (!is_valid_plate_num(plate_num)) {
                    printf("车牌号错误！不能为空且长度不超过%d位\n", MAX_PLATE_NUM);
                }
            } while (!is_valid_plate_num(plate_num));

            do {
                printf("请输入续费月数（%d-%d个月）：", MIN_RENEW_MONTHS, MAX_RENEW_MONTHS);
                fflush(stdout);
                while (scanf("%d", &months) != 1) {
                    clear_input_buffer();
                    printf("输入错误！请输入数字（如6）：");
                    fflush(stdout);
                }
                clear_input_buffer();
                if (!is_valid_renew_months(months)) {
                    printf("续费月数错误！仅支持%d-%d个月\n", MIN_RENEW_MONTHS, MAX_RENEW_MONTHS);
                }
            } while (!is_valid_renew_months(months));

            if (renew_month_card(plate_num, months)) {
                printf("车牌号%s续费%d个月成功！\n", plate_num, months);
            } else {
                printf("车牌号%s续费失败（非月卡用户）！\n", plate_num);
            }
            break;

        case 0: // 退出
            free_car_list();
            free_fee_rule_list();
            free_month_card_list();
            free_park_record_list();
            printf("用户退出，再见！\n");
            exit(0);

        default:
            printf("输入错误！仅支持0-3的操作编号\n");
            clear_input_buffer();
            break;
    }
}
