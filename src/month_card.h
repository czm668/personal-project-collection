#ifndef MONTH_CARD_H
#define MONTH_CARD_H

#include "common.h"

// 全局月卡链表声明（extern关键字确保跨文件使用不重复定义）
extern MonthCard *month_card_list;

// ===================== 函数声明（明确返回类型+参数，解决类型冲突）=====================
// 创建月卡节点
MonthCard* create_month_card(const char *plate_num, time_t expire_time, float balance);

// 添加月卡用户
void add_month_card(MonthCard *card);

// 删除月卡用户
int delete_month_card(const char *plate_num);

// 月卡续费（修改月卡有效期，核心“改”操作）
int renew_month_card(const char *plate_num, int months);

// 检查月卡是否有效
int is_month_card_valid(const char *plate_num);

// 释放月卡链表内存
void free_month_card_list();

// 保存月卡数据到文件（明确返回类型void，解决隐式int声明冲突）
void save_month_cards_to_file(void);

// 加载月卡数据从文件（明确返回类型void）
void load_month_cards_from_file(void);

#endif // MONTH_CARD_H
