#ifndef RESERVATIONMANAGER_H
#define RESERVATIONMANAGER_H

#include <QString>
#include <QVariantMap>
#include <QList>
#include <QSqlDatabase>
#include <QDateTime>

/**
 * @brief 预订管理类（单例模式）
 * 负责客房预订的增删改查、房间可用性校验等业务逻辑
 */
class ReservationManager
{
public:
    /** 获取单例实例 */
    static ReservationManager* getInstance();

    /** 添加预订：返回是否成功 */
    bool addReservation(int roomId, int guestId, const QDateTime& checkinDate,
                        const QDateTime& checkoutDate, int createdBy);

    /** 获取所有预订列表（关联客房、客户信息） */
    QList<QVariantMap> getAllReservations();

    /** 根据ID获取预订信息 */
    QVariantMap getReservationById(int id);

    /** 修改预订：返回是否成功 */
    bool updateReservation(int id, int roomId, int guestId, const QDateTime& checkinDate,
                           const QDateTime& checkoutDate, const QString& status);

    /** 删除预订：返回是否成功 */
    bool deleteReservation(int id);

    /** 检查客房在指定时间段是否可用 */
    bool isRoomAvailable(int roomId, const QDateTime& checkinDate,
                         const QDateTime& checkoutDate, int excludeId = -1);

private:
    QSqlDatabase m_db;          // 数据库连接
    static ReservationManager* m_instance; // 单例实例

    /** 私有构造函数（单例限制） */
    ReservationManager();
};

#endif // RESERVATIONMANAGER_H
