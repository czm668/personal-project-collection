#include "splashscreen.h"
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <QApplication>

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent)
{
    // 设置启动页大小
    setFixedSize(600, 400);
    setWindowTitle("酒店客房管理系统");

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建图片标签
    m_label = new QLabel(this);
    // 设置启动页图片（这里使用文字代替，实际项目中应使用QPixmap加载图片）
    m_label->setText("<h1>酒店客房管理系统</h1><p>加载中...</p>");
    m_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_label);

    setLayout(layout);
}

void SplashScreen::delay(int msec)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < msec) {
        QApplication::processEvents();
    }
}
