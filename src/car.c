#include "car.h"

Car *car_list = NULL;

// 创建车辆节点
Car* create_car(const char *plate_num, int type, time_t in_time) {
    Car *car = (Car*)malloc(sizeof(Car));
    if (car == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    // 拷贝后添加终止符
    strncpy(car->plate_num, plate_num, MAX_PLATE_NUM);
    car->plate_num[MAX_PLATE_NUM - 1] = '\0'; // 添加终止符，避免截断
    car->type = type;
    car->in_time = in_time;
    car->out_time = 0;
    car->next = NULL;
    return car;
}
// 车辆入场（入队）
void enqueue_car(Car *car) {
    if (car_list == NULL) {
        car_list = car;
    } else {
        Car *temp = car_list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = car;
    }
}

// 车辆出场（出队）
Car* dequeue_car(const char *plate_num) {
    if (car_list == NULL) {
        printf("暂无入场车辆！\n");
        return NULL;
    }

    Car *temp = car_list;
    Car *prev = NULL;

    // 查找目标车辆
    while (temp != NULL && strcmp(temp->plate_num, plate_num) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("未找到车牌号%s的入场记录！\n", plate_num);
        return NULL;
    }

    // 从链表移除
    if (prev == NULL) {
        car_list = temp->next;
    } else {
        prev->next = temp->next;
    }

    // 设置出场时间
    temp->out_time = time(NULL);
    return temp;
}

// 释放车辆链表内存
void free_car_list() {
    Car *temp = car_list;
    while (temp != NULL) {
        Car *next = temp->next;
        free(temp);
        temp = next;
    }
    car_list = NULL;
}

// 保存车辆数据到car.dat
void save_cars_to_file() {
    FILE *fp = fopen("car.dat", "wb");
    if (fp == NULL) {
        printf("车辆数据文件打开失败！\n");
        return;
    }

    Car *temp = car_list;
    while (temp != NULL) {
        fwrite(temp, sizeof(Car), 1, fp);
        temp = temp->next;
    }

    fclose(fp);
    printf("车辆数据已保存到car.dat！\n");
}

// 加载车辆数据从car.dat
void load_cars_from_file() {
    FILE *fp = fopen("car.dat", "rb");
    if (fp == NULL) {
        printf("暂无车辆历史数据！\n");
        return;
    }

    Car *temp = NULL;
    while (!feof(fp)) {
        temp = (Car*)malloc(sizeof(Car));
        if (fread(temp, sizeof(Car), 1, fp) != 1) {
            free(temp);
            break;
        }
        temp->next = car_list;
        car_list = temp;
    }

    fclose(fp);
    printf("车辆数据加载成功！\n");
}
