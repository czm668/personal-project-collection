#include "fee.h"

FeeRule *fee_rule_list = NULL;

// 创建计费规则节点
FeeRule* create_fee_rule(int type, float rate) {
    FeeRule *rule = (FeeRule*)malloc(sizeof(FeeRule));
    if (rule == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    rule->type = type;
    rule->rate = rate;
    rule->next = NULL;
    return rule;
}

// 添加计费规则
void add_fee_rule(FeeRule *rule) {
    if (fee_rule_list == NULL) {
        fee_rule_list = rule;
    } else {
        FeeRule *temp = fee_rule_list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = rule;
    }
    printf("时段%d计费规则添加成功（%.2f元/小时）！\n", rule->type, rule->rate);
    save_fee_rules_to_file(); // 自动保存
}

// 删除计费规则
int delete_fee_rule(int type) {
    if (fee_rule_list == NULL) {
        printf("暂无计费规则！\n");
        return 0;
    }

    FeeRule *temp = fee_rule_list;
    FeeRule *prev = NULL;

    while (temp != NULL && temp->type != type) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("未找到时段%d的计费规则！\n", type);
        return 0;
    }

    // 从链表移除
    if (prev == NULL) {
        fee_rule_list = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    save_fee_rules_to_file(); // 自动保存
    return 1;
}

// 计算停车费用（简化版：默认用白天费率，可扩展时段判断）
float calculate_fee(Car *car) {
    // 月卡车免费
    if (car->type == 1) {
        return 0.0;
    }

    // 计算停车时长（小时）
    double hours = difftime(car->out_time, car->in_time) / 3600;
    if (hours < 1) hours = 1; // 不足1小时按1小时算

    // 查找白天费率（默认）
    FeeRule *temp = fee_rule_list;
    float rate = MIN_RATE; // 默认最低费率
    while (temp != NULL) {
        if (temp->type == 0) {
            rate = temp->rate;
            break;
        }
        temp = temp->next;
    }

    return hours * rate;
}

// 释放计费规则链表内存
void free_fee_rule_list() {
    FeeRule *temp = fee_rule_list;
    while (temp != NULL) {
        FeeRule *next = temp->next;
        free(temp);
        temp = next;
    }
    fee_rule_list = NULL;
}

// 保存计费规则到fee_rule.dat
void save_fee_rules_to_file() {
    FILE *fp = fopen("fee_rule.dat", "wb");
    if (fp == NULL) {
        printf("计费规则文件打开失败！\n");
        return;
    }

    FeeRule *temp = fee_rule_list;
    while (temp != NULL) {
        fwrite(temp, sizeof(FeeRule), 1, fp);
        temp = temp->next;
    }

    fclose(fp);
    printf("计费规则已保存到fee_rule.dat！\n");
}

// 加载计费规则从fee_rule.dat
void load_fee_rules_from_file() {
    FILE *fp = fopen("fee_rule.dat", "rb");
    if (fp == NULL) {
        printf("暂无计费规则历史数据！\n");
        return;
    }

    FeeRule *temp = NULL;
    while (!feof(fp)) {
        temp = (FeeRule*)malloc(sizeof(FeeRule));
        if (fread(temp, sizeof(FeeRule), 1, fp) != 1) {
            free(temp);
            break;
        }
        temp->next = fee_rule_list;
        fee_rule_list = temp;
    }

    fclose(fp);
    printf("计费规则加载成功！\n");
}
