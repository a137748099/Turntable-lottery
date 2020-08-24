#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include <QWidget>

/**
 * @brief 抽奖结果，遮罩效果，点击自动隐藏
 */
class MaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaskWidget(QWidget *parent = nullptr);

    void setPrizeText(const QString &text);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sigHide();

private:
    QFont m_font;
    QPainterPath m_heartPath;
    QString m_prizeText;
};

#endif // MASKWIDGET_H
