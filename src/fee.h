#ifndef FEE_H
#define FEE_H

#include "common.h"

// 全局计费规则链表
extern FeeRule *fee_rule_list;

// 创建计费规则节点
FeeRule* create_fee_rule(int type, float rate);

// 添加计费规则
void add_fee_rule(FeeRule *rule);

// 删除计费规则
int delete_fee_rule(int type);

// 计算停车费用
float calculate_fee(Car *car);

// 释放计费规则链表内存
void free_fee_rule_list();

// 保存计费规则到文件
void save_fee_rules_to_file();

// 加载计费规则从文件
void load_fee_rules_from_file();

#endif // FEE_H
