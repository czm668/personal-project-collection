#include "park_record.h"

ParkRecord *park_record_list = NULL;

// 时间戳转字符串
char* time_to_str(time_t t) {
    static char str[20];
    strftime(str, sizeof(str), "%Y-%m-%d %H:%M", localtime(&t));
    return str;
}

// 创建停车记录节点
ParkRecord* create_park_record(const char *plate_num, time_t in_time, time_t out_time, float fee, int is_paid) {
    ParkRecord *record = (ParkRecord*)malloc(sizeof(ParkRecord));
    if (record == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    strncpy(record->plate_num, plate_num, MAX_PLATE_NUM);
    record->plate_num[MAX_PLATE_NUM - 1] = '\0';
    record->in_time = in_time;
    record->out_time = out_time;
    record->fee = fee;
    record->is_paid = is_paid;
    record->next = NULL;
    return record;
}

// 添加停车记录
void add_park_record(ParkRecord *record) {
    if (park_record_list == NULL) {
        park_record_list = record;
    } else {
        ParkRecord *temp = park_record_list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = record;
    }
    save_park_records_to_file(); // 自动保存
}

// 查找停车记录
ParkRecord* find_park_record(const char *plate_num) {
    if (park_record_list == NULL) {
        return NULL;
    }
    ParkRecord *temp = park_record_list;
    while (temp != NULL) {
        // strcmp是完整字符串对比，不是前缀！
        if (strcmp(temp->plate_num, plate_num) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 缴纳停车费
int pay_park_fee(const char *plate_num) {
    if (park_record_list == NULL) {
        printf("暂无停车记录！\n");
        return 0;
    }

    ParkRecord *temp = park_record_list;
    while (temp != NULL) {
        if (strcmp(temp->plate_num, plate_num) == 0 && !temp->is_paid) {
            temp->is_paid = 1; // 标记为已缴
            save_park_records_to_file(); // 自动保存
            return 1;
        }
        temp = temp->next;
    }

    printf("未找到车牌号%s的未缴停车记录！\n", plate_num);
    return 0;
}

// 释放停车记录链表内存
void free_park_record_list() {
    ParkRecord *temp = park_record_list;
    while (temp != NULL) {
        ParkRecord *next = temp->next;
        free(temp);
        temp = next;
    }
    park_record_list = NULL;
}

// 保存停车记录到park_record.dat
void save_park_records_to_file() {
    FILE *fp = fopen("park_record.dat", "wb");
    if (fp == NULL) {
        printf("停车记录文件打开失败！\n");
        return;
    }

    ParkRecord *temp = park_record_list;
    while (temp != NULL) {
        fwrite(temp, sizeof(ParkRecord), 1, fp);
        temp = temp->next;
    }

    fclose(fp);
    printf("停车记录已保存到park_record.dat！\n");
}

// 加载停车记录从park_record.dat
void load_park_records_from_file() {
    FILE *fp = fopen("park_record.dat", "rb");
    if (fp == NULL) {
        printf("暂无停车记录历史数据！\n");
        return;
    }

    ParkRecord *temp = NULL;
    while (!feof(fp)) {
        temp = (ParkRecord*)malloc(sizeof(ParkRecord));
        if (fread(temp, sizeof(ParkRecord), 1, fp) != 1) {
            free(temp);
            break;
        }
        temp->next = park_record_list;
        park_record_list = temp;
    }

    fclose(fp);
    printf("停车记录加载成功！\n");
}
