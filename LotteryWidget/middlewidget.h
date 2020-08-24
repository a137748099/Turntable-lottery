#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>

/**
 * @brief 中间点击抽奖，为了增加闪动的效果，作为一个独立的控件
 */
class MiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MiddleWidget(QWidget *parent = nullptr);

    void setScaleFactor(qreal scale);
    void stop();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void sigStart();

private:
    qreal m_rScaleFactor = 1.0;
    QFont m_font;
    QTimer *m_pMiddleTimer = nullptr;
    bool m_bMiddleFlag = false;
};

#endif // MIDDLEWIDGET_H
