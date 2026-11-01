#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 公共常量
#define MAX_PLATE_NUM 8       // 车牌号最大长度
#define MIN_RATE 0.01         // 最低计费费率（元/小时）
#define MIN_BALANCE 0.0       // 月卡最低初始余额
#define MIN_RENEW_MONTHS 1    // 最低续费月数
#define MAX_RENEW_MONTHS 36   // 最高续费月数

// 车辆数据结构
typedef struct Car {
    char plate_num[MAX_PLATE_NUM]; // 车牌号
    int type;                      // 0-临时车，1-月卡车
    time_t in_time;                // 入场时间
    time_t out_time;               // 出场时间
    struct Car *next;              // 链表下一个节点
} Car;

// 计费规则数据结构
typedef struct FeeRule {
    int type;          // 0-白天，1-夜间，2-节假日
    float rate;        // 费率（元/小时）
    struct FeeRule *next;
} FeeRule;

// 月卡用户数据结构
typedef struct MonthCard {
    char plate_num[MAX_PLATE_NUM]; // 车牌号
    time_t expire_time;            // 过期时间
    float balance;                 // 余额
    struct MonthCard *next;
} MonthCard;

// 停车记录数据结构
typedef struct ParkRecord {
    char plate_num[MAX_PLATE_NUM]; // 车牌号
    time_t in_time;                // 入场时间
    time_t out_time;               // 出场时间
    float fee;                     // 费用
    int is_paid;                   // 0-未缴，1-已缴
    struct ParkRecord *next;
} ParkRecord;

// 全局链表声明（各模块使用）
extern Car *car_list;
extern FeeRule *fee_rule_list;
extern MonthCard *month_card_list;
extern ParkRecord *park_record_list;

// 工具函数：时间戳转字符串
char* time_to_str(time_t t);

// 清空输入缓冲区函数声明
void clear_input_buffer();

#endif // COMMON_H
