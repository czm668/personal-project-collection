#ifndef CAR_H
#define CAR_H

#include "common.h"

// 全局车辆链表
extern Car *car_list;

// 创建车辆节点
Car* create_car(const char *plate_num, int type, time_t in_time);

// 车辆入场（入队）
void enqueue_car(Car *car);

// 车辆出场（出队）
Car* dequeue_car(const char *plate_num);

// 释放车辆链表内存
void free_car_list();

// 保存车辆数据到文件
void save_cars_to_file();

// 加载车辆数据从文件
void load_cars_from_file();

#endif // CAR_H
