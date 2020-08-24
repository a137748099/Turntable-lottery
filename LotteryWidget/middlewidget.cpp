#include "middlewidget.h"
#include <QPainter>
#include <QTimer>

MiddleWidget::MiddleWidget(QWidget *parent):
    QWidget (parent)
{
    //定时器闪动
    m_pMiddleTimer = new QTimer(this);
    m_pMiddleTimer->setInterval(300);
    connect(m_pMiddleTimer, &QTimer::timeout, this, [=]{
        m_bMiddleFlag = !m_bMiddleFlag;
        update();
    });
}

void MiddleWidget::setScaleFactor(qreal scale)
{
    m_rScaleFactor = scale;
}

void MiddleWidget::stop()
{
    m_pMiddleTimer->stop();
    m_bMiddleFlag = false;
    update();
}

void MiddleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int middleStartX = 15*m_rScaleFactor;
    int middleStartY = 15*m_rScaleFactor;
    QRect middleStartRect(middleStartX, middleStartY, 15*m_rScaleFactor, 15*m_rScaleFactor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_bMiddleFlag ? Qt::red : Qt::white);
//    painter.setBrush(Qt::white);
    int xCount = this->width()/23.*m_rScaleFactor;
    int yCount = this->height()/23.*m_rScaleFactor;
    for(int i = 0; i < xCount; ++i) {
        for(int j = 0; j < yCount; ++j) {
            if(i == 0 || i == (xCount - 1) || j == 0 || j == (yCount - 1)) {
                QRect ellipseRect = QRect(middleStartRect.x() + i*23*m_rScaleFactor, middleStartRect.y() + j*23*m_rScaleFactor,
                                          middleStartRect.width(), middleStartRect.height());
                painter.drawEllipse(ellipseRect);
            }
        }
    }

    m_font.setPixelSize(100);
    painter.setFont(m_font);
    painter.setPen(Qt::white);
    painter.drawText(this->rect(), Qt::AlignCenter, u8"开始\n抽奖");

    QWidget::paintEvent(event);
}

void MiddleWidget::mousePressEvent(QMouseEvent *event)
{
    emit sigStart();
    m_pMiddleTimer->start();
    QWidget::mousePressEvent(event);
}
