#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QString>
#include <QVariantMap>
#include <QList>
#include <QSqlDatabase>

/**
 * @brief 客房管理类（单例模式）
 * 负责客房信息的增删改查业务逻辑
 */
class RoomManager
{
public:
    /** 获取单例实例 */
    static RoomManager* getInstance();

    /** 添加客房：返回是否成功 */
    bool addRoom(const QString& roomNumber, const QString& type, double price,
                 const QString& status = "空闲", const QString& description = "");

    /** 获取所有客房列表 */
    QList<QVariantMap> getAllRooms();

    /** 根据ID获取客房信息 */
    QVariantMap getRoomById(int id);

    /** 修改客房信息：返回是否成功 */
    bool updateRoom(int id, const QString& roomNumber, const QString& type, double price,
                    const QString& status, const QString& description);

    /** 删除客房：返回是否成功 */
    bool deleteRoom(int id);

private:
    QSqlDatabase m_db;          // 数据库连接
    static RoomManager* m_instance; // 单例实例

    /** 私有构造函数（单例限制） */
    RoomManager();

    /** 检查房间号是否已存在（更新时排除自身ID） */
    bool isRoomNumberExists(const QString& roomNumber, int excludeId = -1);
};

#endif // ROOMMANAGER_H
