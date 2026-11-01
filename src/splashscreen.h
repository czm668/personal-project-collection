#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>

/**
 * @brief 启动页类，程序启动时显示，3秒后自动关闭
 */
class SplashScreen : public QWidget
{
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = nullptr);

    /**
     * @brief 延迟指定毫秒数
     * @param msec 延迟毫秒数
     */
    void delay(int msec);

private:
    QLabel *m_label; // 显示启动页图片的标签
};

#endif // SPLASHSCREEN_H
