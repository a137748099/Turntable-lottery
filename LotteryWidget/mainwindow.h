#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>


class MaskWidget;
class QTableView;
class QStandardItemModel;
class QLabel;
class MiddleWidget;

struct RangValue {

    RangValue(qreal startVal, qreal endVal):
        m_startValue(startVal), m_endValue(endVal)
    {

    }

    bool contain(qreal val) {
        if(val >= m_startValue && val < m_endValue)
            return true;
        else
            return false;
    }
    qreal m_startValue;
    qreal m_endValue;
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void initCommonSet();
    void initUi();
    void initTurntableParam();

private slots:
    void onStartLottery();
    void onStart();

private:
    //生成的随机数对应的奖品
    int getOption(int rand);

private:
    QRect m_middleRect;
    QList<QColor> m_colorList;
    QList<int> m_indexList;
    QMap<int, QString> m_prizeMap;
    QMap<int, QColor> m_colorMap;
    QTimer *m_pTimer = nullptr;
    int m_index = 0;
    int m_circle = 0;
    int m_srandBase = 1000;
    QFont m_font;
    QMap<int, RangValue> m_optionMap;
    int m_prizeIndex = 0;
    QTimer *m_pMiddleTimer = nullptr;
    bool m_bMiddleFlag = false;

    qreal m_rScaleFactor = 1.0;
    qreal m_rXScale = 1.0;
    qreal m_rYScale = 1.0;

    QRect m_outFirstRect;
    QRect m_outSecondRect;
    QRect m_startRect;
    int m_singleWidth;

    MiddleWidget *m_pMiddleWidget = nullptr;
    MaskWidget *m_pMaskWidget = nullptr;
    QTableView *m_pTableView = nullptr;
    QStandardItemModel *m_pModel = nullptr;
    int m_rowIndex = 0;
    QLabel *m_pTableTitleLabel = nullptr;
};

#endif // MAINWINDOW_H
