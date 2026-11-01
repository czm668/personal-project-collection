/*
 Navicat Premium Dump SQL

 Source Server         : 本地 MySQL
 Source Server Type    : MySQL
 Source Server Version : 80044 (8.0.44)
 Source Host           : localhost:3306
 Source Schema         : hotel_db

 Target Server Type    : MySQL
 Target Server Version : 80044 (8.0.44)
 File Encoding         : 65001

 Date: 08/12/2025 12:30:26
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for guests
-- ----------------------------
DROP TABLE IF EXISTS `guests`;
CREATE TABLE `guests`  (
  `id` int NOT NULL AUTO_INCREMENT COMMENT '客户 ID',
  `name` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '客户姓名',
  `id_card` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '身份证号（唯一）',
  `phone` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '联系电话',
  `address` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL COMMENT '居住地址',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `idx_id_card`(`id_card` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 22 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of guests
-- ----------------------------
INSERT INTO `guests` VALUES (1, '刘备', '110101198001011234', '13800138001', '北京市东城区', '2024-01-05 10:00:00');
INSERT INTO `guests` VALUES (2, '关羽', '110101198203045678', '13800138002', '北京市西城区', '2024-01-05 10:10:00');
INSERT INTO `guests` VALUES (3, '张飞', '110101198505067890', '13800138003', '北京市朝阳区', '2024-01-05 10:20:00');
INSERT INTO `guests` VALUES (4, '赵云', '310101198807081234', '13900139001', '上海市黄浦区', '2024-01-05 10:30:00');
INSERT INTO `guests` VALUES (5, '马超', '310101199009105678', '13900139002', '上海市静安区', '2024-01-05 10:40:00');
INSERT INTO `guests` VALUES (6, '黄忠', '440101197511127890', '13700137001', '广州市天河区', '2024-01-05 10:50:00');
INSERT INTO `guests` VALUES (7, '诸葛亮', '440101198301151234', '13700137002', '广州市越秀区', '2024-01-05 11:00:00');
INSERT INTO `guests` VALUES (8, '周瑜', '330101198603185678', '13600136001', '杭州市西湖区', '2024-01-05 11:10:00');
INSERT INTO `guests` VALUES (9, '吕布', '330101198905207890', '13600136002', '杭州市滨江区', '2024-01-05 11:20:00');
INSERT INTO `guests` VALUES (10, '貂蝉', '510101199207221234', '13500135001', '成都市锦江区', '2024-01-05 11:30:00');
INSERT INTO `guests` VALUES (11, '曹操', '510101197809255678', '13500135002', '成都市青羊区', '2024-01-05 11:40:00');
INSERT INTO `guests` VALUES (12, '孙权', '420101198111287890', '13400134001', '武汉市武昌区', '2024-01-05 11:50:00');
INSERT INTO `guests` VALUES (13, '司马懿', '420101198401301234', '13400134002', '武汉市汉口区', '2024-01-05 12:00:00');
INSERT INTO `guests` VALUES (14, '孙策', '610101198703035678', '13300133001', '西安市雁塔区', '2024-01-05 12:10:00');
INSERT INTO `guests` VALUES (15, '太史慈', '610101199005057890', '13300133002', '西安市碑林区', '2024-01-05 12:20:00');
INSERT INTO `guests` VALUES (16, '许褚', '210101197907071234', '13200132001', '沈阳市和平区', '2024-01-05 12:30:00');
INSERT INTO `guests` VALUES (17, '典韦', '210101198309095678', '13200132002', '沈阳市沈河区', '2024-01-05 12:40:00');
INSERT INTO `guests` VALUES (18, '张辽', '120101198611117890', '13100131001', '天津市和平区', '2024-01-05 12:50:00');
INSERT INTO `guests` VALUES (19, '徐晃', '120101198901131234', '13100131002', '天津市河西区', '2024-01-05 13:00:00');

-- ----------------------------
-- Table structure for reservations
-- ----------------------------
DROP TABLE IF EXISTS `reservations`;
CREATE TABLE `reservations`  (
  `id` int NOT NULL AUTO_INCREMENT COMMENT '预订 ID',
  `room_id` int NOT NULL COMMENT '关联客房 ID',
  `guest_id` int NOT NULL COMMENT '关联客户 ID',
  `checkin_date` datetime NOT NULL COMMENT '入住日期',
  `checkout_date` datetime NOT NULL COMMENT '退房日期',
  `status` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '预订中' COMMENT '预订状态',
  `created_by` int NOT NULL COMMENT '操作人（关联用户 ID）',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '预订创建时间',
  PRIMARY KEY (`id` DESC) USING BTREE,
  INDEX `fk_reserve_room`(`room_id` ASC) USING BTREE,
  INDEX `fk_reserve_guest`(`guest_id` ASC) USING BTREE,
  INDEX `fk_reserve_user`(`created_by` ASC) USING BTREE,
  CONSTRAINT `fk_reserve_guest` FOREIGN KEY (`guest_id`) REFERENCES `guests` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_reserve_room` FOREIGN KEY (`room_id`) REFERENCES `rooms` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_reserve_user` FOREIGN KEY (`created_by`) REFERENCES `users` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 25 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic STATS_PERSISTENT = 0;

-- ----------------------------
-- Records of reservations
-- ----------------------------
INSERT INTO `reservations` VALUES (24, 1, 2, '2025-12-05 13:08:06', '2025-12-06 13:08:06', '预订中', 1, '2025-12-05 13:10:03');
INSERT INTO `reservations` VALUES (15, 19, 19, '2024-04-30 14:00:00', '2024-05-02 12:00:00', '已退房', 20, '2024-04-28 12:00:00');
INSERT INTO `reservations` VALUES (14, 18, 18, '2024-05-01 14:00:00', '2024-05-08 12:00:00', '已入住', 19, '2024-04-28 11:50:00');
INSERT INTO `reservations` VALUES (13, 17, 17, '2024-05-06 14:00:00', '2024-05-09 12:00:00', '已完成', 18, '2025-12-04 21:25:15');
INSERT INTO `reservations` VALUES (12, 16, 16, '2024-05-02 14:00:00', '2024-05-04 12:00:00', '已退房', 17, '2024-04-28 11:30:00');
INSERT INTO `reservations` VALUES (11, 15, 15, '2024-05-04 14:00:00', '2024-05-07 12:00:00', '预订中', 16, '2024-04-28 11:20:00');
INSERT INTO `reservations` VALUES (10, 14, 14, '2024-05-01 14:00:00', '2024-05-05 12:00:00', '已入住', 15, '2024-04-28 11:10:00');
INSERT INTO `reservations` VALUES (9, 13, 13, '2024-04-30 14:00:00', '2024-05-03 12:00:00', '已退房', 14, '2024-04-28 11:00:00');
INSERT INTO `reservations` VALUES (8, 12, 12, '2024-05-05 14:00:00', '2024-05-10 12:00:00', '预订中', 13, '2024-04-28 10:50:00');
INSERT INTO `reservations` VALUES (7, 11, 11, '2024-05-02 14:00:00', '2024-05-06 12:00:00', '已入住', 12, '2024-04-28 10:40:00');
INSERT INTO `reservations` VALUES (6, 10, 10, '2024-05-03 14:00:00', '2024-05-08 12:00:00', '已入住', 11, '2024-04-28 10:30:00');
INSERT INTO `reservations` VALUES (5, 9, 9, '2024-05-01 14:00:00', '2024-05-02 12:00:00', '已退房', 10, '2024-04-28 10:20:00');
INSERT INTO `reservations` VALUES (4, 8, 8, '2024-05-04 14:00:00', '2024-05-09 12:00:00', '预订中', 9, '2024-04-28 10:10:00');
INSERT INTO `reservations` VALUES (3, 7, 7, '2024-05-02 14:00:00', '2024-05-04 12:00:00', '已退房', 8, '2024-04-28 10:00:00');
INSERT INTO `reservations` VALUES (2, 6, 6, '2024-05-05 14:00:00', '2024-05-08 12:00:00', '预订中', 7, '2024-04-28 09:50:00');
INSERT INTO `reservations` VALUES (1, 5, 5, '2024-04-30 14:00:00', '2024-05-02 12:00:00', '已退房', 6, '2024-04-28 09:40:00');

-- ----------------------------
-- Table structure for rooms
-- ----------------------------
DROP TABLE IF EXISTS `rooms`;
CREATE TABLE `rooms`  (
  `id` int NOT NULL AUTO_INCREMENT COMMENT '客房 ID',
  `room_number` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '房间号（唯一)',
  `type` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '客房类型（如 “单人间”）',
  `price` decimal(10, 2) NOT NULL COMMENT '价格（元 / 晚）',
  `status` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL DEFAULT '空闲' COMMENT '状态（空闲 / 已预订 / 维修中）',
  `description` text CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL COMMENT '房间描述',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `idx_room_number`(`room_number` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 21 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of rooms
-- ----------------------------
INSERT INTO `rooms` VALUES (1, '101', '单人间', 199.00, '已预订', '', '2025-12-05 13:10:03');
INSERT INTO `rooms` VALUES (3, '103', '标准间', 299.00, '已入住', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (4, '104', '标准间', 299.00, '空闲', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (5, '105', '标准间', 299.00, '已预订', '带独立卫生间，双床，含双早', '2025-12-05 13:15:32');
INSERT INTO `rooms` VALUES (6, '106', '标准间', 299.00, '已退房', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (7, '107', '标准间', 299.00, '空闲', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (8, '108', '标准间', 299.00, '已预订', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (9, '109', '标准间', 299.00, '空闲', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (10, '110', '标准间', 299.00, '已入住', '带独立卫生间，双床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (11, '201', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (12, '202', '豪华间', 499.00, '已预订', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (13, '203', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (14, '204', '豪华间', 499.00, '已入住', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (15, '205', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2025-12-05 12:06:30');
INSERT INTO `rooms` VALUES (16, '206', '豪华间', 499.00, '已退房', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (17, '207', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2025-12-04 21:25:15');
INSERT INTO `rooms` VALUES (18, '208', '豪华间', 499.00, '已预订', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (19, '209', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2024-01-01 00:00:00');
INSERT INTO `rooms` VALUES (20, '210', '豪华间', 499.00, '空闲', '大户型，带阳台， king 床，含双早', '2025-12-05 12:06:40');

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users`  (
  `id` int NOT NULL AUTO_INCREMENT COMMENT '用户 ID（唯一标识）',
  `username` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '用户名（唯一）',
  `password` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '密码',
  `name` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '真实姓名',
  `is_admin` tinyint NOT NULL DEFAULT 0 COMMENT '是否管理员（0 = 否，1 = 是）',
  `is_locked` tinyint NOT NULL DEFAULT 0 COMMENT '是否锁定（0 = 否，1 = 是）',
  `login_attempts` int NOT NULL DEFAULT 0 COMMENT '登录失败次数',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `idx_username`(`username` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 26 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES (1, 'admin', '123456', '系统管理员', 1, 0, 0, '2025-12-04 20:54:34');
INSERT INTO `users` VALUES (6, '张三', '654321', '张三', 0, 0, 0, '2025-12-04 21:13:23');
INSERT INTO `users` VALUES (7, 'staff1', 'staff123', '张三', 0, 0, 0, '2024-01-02 09:00:00');
INSERT INTO `users` VALUES (8, 'staff2', 'staff123', '李四', 0, 0, 0, '2024-01-02 09:10:00');
INSERT INTO `users` VALUES (9, 'staff3', 'staff123', '王五', 0, 0, 0, '2024-01-02 09:20:00');
INSERT INTO `users` VALUES (10, 'staff4', 'staff123', '赵六', 0, 0, 0, '2024-01-02 09:30:00');
INSERT INTO `users` VALUES (11, 'staff5', 'staff123', '孙七', 0, 0, 0, '2024-01-02 09:40:00');
INSERT INTO `users` VALUES (12, 'staff6', 'staff123', '周八', 0, 0, 0, '2024-01-02 09:50:00');
INSERT INTO `users` VALUES (13, 'staff7', 'staff123', '吴九', 0, 0, 0, '2024-01-02 10:00:00');
INSERT INTO `users` VALUES (14, 'staff8', 'staff123', '郑十', 0, 0, 0, '2024-01-02 10:10:00');
INSERT INTO `users` VALUES (15, 'staff9', 'staff123', '钱一', 0, 0, 0, '2024-01-02 10:20:00');
INSERT INTO `users` VALUES (16, 'staff10', 'staff123', '孙二', 0, 0, 0, '2024-01-02 10:30:00');
INSERT INTO `users` VALUES (17, 'staff11', 'staff123', '周三', 0, 0, 0, '2024-01-02 10:40:00');
INSERT INTO `users` VALUES (18, 'staff12', 'staff123', '吴四', 0, 0, 0, '2024-01-02 10:50:00');
INSERT INTO `users` VALUES (19, 'staff13', 'staff123', '郑五', 0, 0, 0, '2024-01-02 11:00:00');
INSERT INTO `users` VALUES (20, 'staff14', 'staff123', '王六', 0, 0, 0, '2024-01-02 11:10:00');

SET FOREIGN_KEY_CHECKS = 1;
