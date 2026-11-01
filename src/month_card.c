#include "month_card.h"

// 全局月卡链表定义（仅在本文件定义，其他文件通过extern引用）
MonthCard *month_card_list = NULL;

// 创建月卡节点（修复车牌号截断：添加字符串终止符）
MonthCard* create_month_card(const char *plate_num, time_t expire_time, float balance) {
    MonthCard *card = (MonthCard*)malloc(sizeof(MonthCard));
    if (card == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    // 关键修复：拷贝车牌号+手动添加终止符，避免截断
    strncpy(card->plate_num, plate_num, MAX_PLATE_NUM);
    card->plate_num[MAX_PLATE_NUM - 1] = '\0'; // 强制终止符，解决显示异常
    card->expire_time = expire_time;
    card->balance = balance;
    card->next = NULL;
    return card;
}

// 添加月卡用户
void add_month_card(MonthCard *card) {
    if (month_card_list == NULL) {
        month_card_list = card;
    } else {
        MonthCard *temp = month_card_list;
        while (temp->next != NULL) {
            temp = temp->next; // 修复：确保每行结尾分号，语法正确
        }
        temp->next = card;
    }
    save_month_cards_to_file(); // 自动保存，避免数据丢失
}

// 删除月卡用户
int delete_month_card(const char *plate_num) {
    if (month_card_list == NULL) {
        printf("暂无月卡用户！\n");
        return 0;
    }

    MonthCard *temp = month_card_list;
    MonthCard *prev = NULL;

    // 遍历链表：完整匹配车牌号（解决“按省份查”问题）
    while (temp != NULL && strcmp(temp->plate_num, plate_num) != 0) {
        prev = temp;
        temp = temp->next; // 修复：语法正确，分号结尾
    }

    if (temp == NULL) {
        printf("未找到月卡用户%s！\n", plate_num);
        return 0;
    }

    // 从链表移除节点
    if (prev == NULL) {
        month_card_list = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    save_month_cards_to_file(); // 自动保存
    return 1;
}

// 月卡续费（核心“改”操作，修复语法错误）
int renew_month_card(const char *plate_num, int months) {
    if (month_card_list == NULL) {
        printf("暂无月卡用户！\n");
        return 0;
    }

    MonthCard *temp = month_card_list;
    // 遍历链表：完整匹配车牌号（非前缀）
    while (temp != NULL && strcmp(temp->plate_num, plate_num) != 0) {
        temp = temp->next; // 修复：语法正确，分号结尾
    }

    if (temp == NULL) {
        printf("未找到月卡用户%s！\n", plate_num);
        return 0;
    }

    // 核心修改：延长月卡有效期（修复语法错误，确保->使用合法）
    temp->expire_time += months * 30 * 24 * 3600; // 按30天/月计算，语法正确
    save_month_cards_to_file(); // 自动保存修改后的数据
    return 1;
}

// 检查月卡是否有效
int is_month_card_valid(const char *plate_num) {
    if (month_card_list == NULL) {
        return 0;
    }

    MonthCard *temp = month_card_list;
    while (temp != NULL) {
        // 完整匹配车牌号+检查有效期
        if (strcmp(temp->plate_num, plate_num) == 0 && temp->expire_time > time(NULL)) {
            return 1; // 有效
        }
        temp = temp->next; // 修复：语法正确，分号结尾
    }

    return 0; // 无效
}

// 释放月卡链表内存
void free_month_card_list() {
    MonthCard *temp = month_card_list;
    while (temp != NULL) {
        MonthCard *next = temp->next;
        free(temp);
        temp = next;
    }
    month_card_list = NULL;
}

// 保存月卡数据到文件（修复所有语法错误，解决编译冲突）
void save_month_cards_to_file(void) {
    FILE *fp = fopen("month_card.dat", "wb");
    if (fp == NULL) {
        printf("月卡数据文件打开失败！\n");
        return;
    }

    MonthCard *temp = month_card_list;
    // 遍历链表写入文件（修复第86行语法错误：->使用合法+分号结尾）
    while (temp != NULL) {
        fwrite(temp, sizeof(MonthCard), 1, fp);
        temp = temp->next; // 关键修复：该行添加分号，解决“->”前语法错误
    }

    fclose(fp);
    printf("月卡数据已保存到month_card.dat！\n");
}

// 加载月卡数据从文件（修复语法错误，大括号匹配）
void load_month_cards_from_file(void) {
    FILE *fp = fopen("month_card.dat", "rb");
    if (fp == NULL) {
        printf("暂无月卡历史数据！\n");
        return;
    }

    MonthCard *temp = NULL;
    // 遍历文件读取数据（修复大括号/语法错误）
    while (!feof(fp)) {
        temp = (MonthCard*)malloc(sizeof(MonthCard));
        if (fread(temp, sizeof(MonthCard), 1, fp) != 1) {
            free(temp);
            break; // 修复：大括号匹配，语法正确
        }
        temp->next = month_card_list;
        month_card_list = temp;
    }

    fclose(fp);
    printf("月卡数据加载成功！\n");
}
