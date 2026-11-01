#ifndef GUESTMANAGER_H
#define GUESTMANAGER_H

#include <QString>
#include <QVariantMap>
#include <QList>
#include <QSqlDatabase>

/**
 * @brief 客户管理类（单例模式）
 * 负责客户信息的增删改查业务逻辑
 */
class GuestManager
{
public:
    /** 获取单例实例 */
    static GuestManager* getInstance();

    /** 添加客户：返回是否成功 */
    bool addGuest(const QString& name, const QString& idCard, const QString& phone,
                  const QString& address = "");

    /** 获取所有客户列表 */
    QList<QVariantMap> getAllGuests();

    /** 根据ID获取客户信息 */
    QVariantMap getGuestById(int id);

    /** 修改客户信息：返回是否成功 */
    bool updateGuest(int id, const QString& name, const QString& idCard, const QString& phone,
                     const QString& address);

    /** 删除客户：返回是否成功 */
    bool deleteGuest(int id);

private:
    QSqlDatabase m_db;          // 数据库连接
    static GuestManager* m_instance; // 单例实例

    /** 私有构造函数（单例限制） */
    GuestManager();

    /** 检查身份证号是否已存在（更新时排除自身ID） */
    bool isIdCardExists(const QString& idCard, int excludeId = -1);
};

#endif // GUESTMANAGER_H
