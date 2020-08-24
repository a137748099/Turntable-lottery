#include "maskwidget.h"
#include <QPainter>
#include <math.h>

MaskWidget::MaskWidget(QWidget *parent):
    QWidget (parent)
{
    m_font.setPixelSize(60);
    //绘制爱心的path
    m_heartPath.setFillRule(Qt::WindingFill);
    //调整爱心大小的系数
    double k = 18;
    float x = 16 * k * sin(0.0)*sin(0.0)*sin(0.0);
    float y = 13 * k * cos(0.0) - 5 * k*cos(0.0) - 2 * k*cos(0.0) - cos(0.0);
    m_heartPath.moveTo(x, -y);
    for (double t = 0.01; t < 100; t += 0.05)
    {
        x = 16 * k * sin(k*t)*sin(k*t)*sin(k*t);
        y = 13 * k * cos(k*t) - 5 * k * cos(2 * k * t) - 2 * k * cos(3 * k * t) - cos(4 * k * t);
        m_heartPath.lineTo(x, -y);
    }
}

void MaskWidget::setPrizeText(const QString &text)
{
    m_prizeText = text;
    update();
}

void MaskWidget::mousePressEvent(QMouseEvent *event)
{
    this->hide();
    emit sigHide();
    QWidget::mousePressEvent(event);
}

void MaskWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());

    painter.translate(this->width()/2, this->height()/2);
//    painter.setBrush(Qt::NoBrush);
    painter.setBrush(QColor(253, 189, 210));
//    painter.setPen(QColor(253, 189, 210));
    painter.drawPath(m_heartPath);
    painter.translate(-this->width()/2, -this->height()/2);

    painter.setPen(QColor(248, 10, 87));
    painter.setFont(m_font);
    painter.drawText(this->rect(), Qt::AlignCenter, "恭喜亲爱的抽中了\n" + m_prizeText);
    QWidget::paintEvent(event);
}
