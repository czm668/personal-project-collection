#ifndef PARK_RECORD_H
#define PARK_RECORD_H

#include "common.h"

// 全局停车记录链表
extern ParkRecord *park_record_list;

// 创建停车记录节点
ParkRecord* create_park_record(const char *plate_num, time_t in_time, time_t out_time, float fee, int is_paid);

// 添加停车记录
void add_park_record(ParkRecord *record);

// 查找停车记录
ParkRecord* find_park_record(const char *plate_num);

// 缴纳停车费
int pay_park_fee(const char *plate_num);

// 释放停车记录链表内存
void free_park_record_list();

// 保存停车记录到文件
void save_park_records_to_file();

// 加载停车记录从文件
void load_park_records_from_file();

#endif // PARK_RECORD_H
