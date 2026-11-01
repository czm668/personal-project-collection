#include "guestmanagement.h"
#include "ui_guestmanagement.h"
#include "logger.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

GuestManagement::GuestManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuestManagement),
    m_guestMgr(GuestManager::getInstance()),
    m_selectedGuestId(-1)
{
    ui->setupUi(this);
    initTable();
    refreshGuestList();

    // 身份证号格式校验（18位）
    QRegularExpression idCardRegex("^[1-9]\\d{5}(18|19|20)\\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\\d{3}[0-9Xx]$");
    ui->lineEdit_idcard->setValidator(new QRegularExpressionValidator(idCardRegex, this));

    // 手机号格式校验（11位数字）
    QRegularExpression phoneRegex("^1[3-9]\\d{9}$");
    ui->lineEdit_phone->setValidator(new QRegularExpressionValidator(phoneRegex, this));
}

GuestManagement::~GuestManagement()
{
    delete ui;
}

void GuestManagement::initTable()
{
    // 设置表格列头和列数
    QStringList headers = {"ID", "客户姓名", "身份证号", "联系电话", "居住地址", "创建时间"};
    ui->tableWidget_guests->setColumnCount(headers.size());
    ui->tableWidget_guests->setHorizontalHeaderLabels(headers);

    // 表格样式配置
    ui->tableWidget_guests->setSelectionBehavior(QTableWidget::SelectRows);  // 整行选中
    ui->tableWidget_guests->setEditTriggers(QTableWidget::NoEditTriggers);   // 禁止编辑
    ui->tableWidget_guests->horizontalHeader()->setStretchLastSection(true); // 最后一列自适应
    // 设置表格所有行的默认高度（通过垂直表头接口实现）
    ui->tableWidget_guests->verticalHeader()->setDefaultSectionSize(25);
}

void GuestManagement::refreshGuestList()
{
    // 清空表格现有数据
    ui->tableWidget_guests->setRowCount(0);

    // 从业务类获取所有客户数据
    QList<QVariantMap> guests = m_guestMgr->getAllGuests();

    // 填充表格数据
    for (int i = 0; i < guests.size(); i++) {
        QVariantMap guest = guests[i];
        ui->tableWidget_guests->insertRow(i);

        // 逐列设置数据
        ui->tableWidget_guests->setItem(i, 0, new QTableWidgetItem(QString::number(guest["id"].toInt())));
        ui->tableWidget_guests->setItem(i, 1, new QTableWidgetItem(guest["name"].toString()));
        ui->tableWidget_guests->setItem(i, 2, new QTableWidgetItem(guest["id_card"].toString()));
        ui->tableWidget_guests->setItem(i, 3, new QTableWidgetItem(guest["phone"].toString()));
        ui->tableWidget_guests->setItem(i, 4, new QTableWidgetItem(guest["address"].toString()));
        ui->tableWidget_guests->setItem(i, 5, new QTableWidgetItem(guest["created_at"].toString()));
    }

    Logger::getInstance()->log("刷新客户列表，共加载 " + QString::number(guests.size()) + " 条客户记录");
}

void GuestManagement::clearForm()
{
    // 清空所有输入框
    ui->lineEdit_name->clear();
    ui->lineEdit_idcard->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_address->clear();

    // 重置选中ID
    m_selectedGuestId = -1;
}

bool GuestManagement::validateForm()
{
    // 获取表单输入值
    QString name = ui->lineEdit_name->text().trimmed();
    QString idCard = ui->lineEdit_idcard->text().trimmed();
    QString phone = ui->lineEdit_phone->text().trimmed();

    // 非空校验
    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入校验", "客户姓名不能为空！");
        return false;
    }

    if (idCard.isEmpty()) {
        QMessageBox::warning(this, "输入校验", "身份证号不能为空！");
        return false;
    }

    if (phone.isEmpty()) {
        QMessageBox::warning(this, "输入校验", "联系电话不能为空！");
        return false;
    }

    // 身份证号长度校验（18位）
    if (idCard.length() != 18) {
        QMessageBox::warning(this, "输入校验", "身份证号必须为18位！");
        return false;
    }

    // 手机号长度校验（11位）
    if (phone.length() != 11) {
        QMessageBox::warning(this, "输入校验", "联系电话必须为11位手机号！");
        return false;
    }

    return true;
}

void GuestManagement::on_pushButton_add_clicked()
{
    // 先校验表单
    if (!validateForm()) return;

    // 获取表单数据
    QString name = ui->lineEdit_name->text().trimmed();
    QString idCard = ui->lineEdit_idcard->text().trimmed();
    QString phone = ui->lineEdit_phone->text().trimmed();
    QString address = ui->lineEdit_address->text().trimmed();

    // 调用业务类添加客户
    if (m_guestMgr->addGuest(name, idCard, phone, address)) {
        QMessageBox::information(this, "操作成功", "客户信息添加成功！");
        Logger::getInstance()->log("添加客户成功：" + name + "（身份证：" + idCard + "）");
        refreshGuestList(); // 刷新列表
        clearForm();        // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客户信息添加失败（身份证号已存在或系统错误）！");
    }
}

void GuestManagement::on_pushButton_update_clicked()
{
    // 校验是否选中客户
    if (m_selectedGuestId == -1) {
        QMessageBox::warning(this, "操作提示", "请先从表格中选择要修改的客户！");
        return;
    }

    // 校验表单
    if (!validateForm()) return;

    // 获取表单数据
    QString name = ui->lineEdit_name->text().trimmed();
    QString idCard = ui->lineEdit_idcard->text().trimmed();
    QString phone = ui->lineEdit_phone->text().trimmed();
    QString address = ui->lineEdit_address->text().trimmed();

    // 调用业务类修改客户
    if (m_guestMgr->updateGuest(m_selectedGuestId, name, idCard, phone, address)) {
        QMessageBox::information(this, "操作成功", "客户信息修改成功！");
        Logger::getInstance()->log("修改客户成功：ID=" + QString::number(m_selectedGuestId) + "，姓名=" + name);
        refreshGuestList(); // 刷新列表
        clearForm();        // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客户信息修改失败（身份证号已存在或系统错误）！");
    }
}

void GuestManagement::on_pushButton_delete_clicked()
{
    // 校验是否选中客户
    if (m_selectedGuestId == -1) {
        QMessageBox::warning(this, "操作提示", "请先从表格中选择要删除的客户！");
        return;
    }

    // 确认删除
    int ret = QMessageBox::question(this, "确认删除", "是否确定删除选中的客户？删除后无法恢复！",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 调用业务类删除客户
    if (m_guestMgr->deleteGuest(m_selectedGuestId)) {
        QMessageBox::information(this, "操作成功", "客户信息删除成功！");
        Logger::getInstance()->log("删除客户成功：ID=" + QString::number(m_selectedGuestId));
        refreshGuestList(); // 刷新列表
        clearForm();        // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客户信息删除失败（该客户存在关联预订记录）！");
    }
}

void GuestManagement::on_pushButton_clear_clicked()
{
    clearForm(); // 清空表单
}

void GuestManagement::on_tableWidget_guests_itemClicked(QTableWidgetItem *item)
{
    // 获取选中行
    int row = item->row();

    // 获取选中客户ID
    m_selectedGuestId = ui->tableWidget_guests->item(row, 0)->text().toInt();

    // 加载选中客户数据到表单
    QVariantMap guest = m_guestMgr->getGuestById(m_selectedGuestId);
    if (!guest.isEmpty()) {
        ui->lineEdit_name->setText(guest["name"].toString());
        ui->lineEdit_idcard->setText(guest["id_card"].toString());
        ui->lineEdit_phone->setText(guest["phone"].toString());
        ui->lineEdit_address->setText(guest["address"].toString());
    }
}
