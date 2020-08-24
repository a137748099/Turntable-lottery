#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QTime>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDateTime>
#include <QLabel>
#include <QScreen>
#include "maskwidget.h"
#include "middlewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    initCommonSet();
    initUi();
    initTurntableParam();
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 211, 212));
    //画背景
    painter.drawRect(this->rect());

    //画标题栏
    painter.setBrush(QColor(255, 68, 72));
    painter.drawRect(QRect(0, 0, this->width(), this->height()/10));
    painter.setPen(Qt::white);
    m_font.setPixelSize(50);
    painter.setFont(m_font);
    painter.drawText(QRect(0, 0, this->width(), this->height()/10), Qt::AlignCenter, u8"七夕送女朋友的礼物");
    painter.setPen(Qt::NoPen);

    painter.setBrush(Qt::white);
    painter.drawRect(m_outFirstRect);
    painter.setBrush(QColor(255, 117, 88));
    painter.drawRect(m_outSecondRect);
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            //排除中间四个
            if((i == 1 && (j == 1 || j == 2)) || (i == 2 && (j == 1 || j == 2)))
                continue;
            QRect sigleRect = QRect(m_startRect.x() + i*m_singleWidth,
                                    m_startRect.y() + j*m_singleWidth,
                                    m_startRect.width(), m_startRect.height());
//            qDebug() << __FUNCTION__ << sigleRect << startRect;
            painter.setBrush(m_colorMap.value(i + j*4));
            painter.drawRect(sigleRect);
            painter.setPen(QPen(Qt::white, 6*m_rScaleFactor));
            m_font.setPixelSize(40);
            painter.setFont(m_font);
            painter.drawText(sigleRect, Qt::AlignCenter, m_prizeMap.value(i + j*4));
            painter.setPen(Qt::NoPen);
            //画选中的边框
            if(m_indexList.at(m_index) == i + j*4) {
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen(Qt::white, 16*m_rScaleFactor));
                painter.drawRect(sigleRect.adjusted(8*m_rScaleFactor, 8*m_rScaleFactor,
                                                    -8*m_rScaleFactor, -8*m_rScaleFactor));
                painter.setPen(Qt::NoPen);
            }

        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_pTableView->resize(this->width(), this->height()/6);
    m_pTableView->move(0, this->height() - m_pTableView->height());
    m_pTableTitleLabel->resize(this->width(), this->height()/12);
    m_pTableTitleLabel->move(0, m_pTableView->y() - m_pTableTitleLabel->height());
    QWidget::resizeEvent(event);
}

void MainWindow::initCommonSet()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    QRect screenRect = qApp->desktop()->screenGeometry();

    qreal normalScale = screenRect.height() / 1080.;
    qreal curScale = screenRect.width() / 1920.;
    m_rScaleFactor = normalScale <= curScale ? normalScale : curScale;

    //设置随机数的种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    this->resize(screenRect.width(), screenRect.height());

    int outsideWidth = (this->width() < this->height() ? this->height() : this->width())*2/5;
    m_outFirstRect = QRect(this->width()/2 - outsideWidth/2, this->height()/2 - outsideWidth/2 - this->height()/10, outsideWidth, outsideWidth);
    outsideWidth -= 30*m_rScaleFactor;
    m_outSecondRect = QRect(this->width()/2 - outsideWidth/2, this->height()/2 - outsideWidth/2 - this->height()/10, outsideWidth, outsideWidth);
    outsideWidth -= 30*m_rScaleFactor;
    int startX = this->width()/2 - outsideWidth/2;
    int startY = this->height()/2 - outsideWidth/2 - this->height()/10;
    m_singleWidth = outsideWidth/4;
    m_startRect = QRect(startX, startY, m_singleWidth, m_singleWidth);
    m_middleRect = QRect(startX + m_singleWidth, startY + m_singleWidth, m_singleWidth*2, m_singleWidth*2);

}

void MainWindow::initUi()
{
    m_font.setPixelSize(100);

    //中间点击部分
    m_pMiddleWidget = new MiddleWidget(this);
    m_pMiddleWidget->setGeometry(m_middleRect);
    connect(m_pMiddleWidget, &MiddleWidget::sigStart, this, &MainWindow::onStart);

    //抽奖结果
    m_pTableView = new QTableView(this);
    m_pTableView->horizontalHeader()->setVisible(false);
//    m_pTableView->verticalHeader()->setVisible(false);
    m_pTableView->setEditTriggers(QTableView::NoEditTriggers);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pModel = new QStandardItemModel();

    m_pTableTitleLabel = new QLabel(this);
    m_font.setPixelSize(60);
    m_pTableTitleLabel->setFont(m_font);
    m_pTableTitleLabel->setStyleSheet("color:rgb(255, 68, 72);");
    m_pTableTitleLabel->setText(u8"抽奖结果:");

    m_pMaskWidget = new MaskWidget(this);
    m_pMaskWidget->resize(this->size());
    m_pMaskWidget->hide();
    connect(m_pMaskWidget, &MaskWidget::sigHide, this, [=]{
        QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        m_pModel->setItem(m_rowIndex, 0, new QStandardItem(curTime));
        m_pModel->setItem(m_rowIndex++, 1, new QStandardItem(m_prizeMap.value(m_indexList.at(m_index))));
        m_pTableView->setModel(m_pModel);
    });
}

void MainWindow::initTurntableParam()
{
    m_indexList << 0 << 1 << 2 << 3 << 7 << 11 << 15 << 14 << 13 << 12 << 8 << 4;
    QList<QColor> colorList;
    colorList << QColor(134, 112, 255) << QColor(223, 136, 207)
                << QColor(142, 217, 249) << QColor(250, 89, 143)
                << QColor(33, 217, 207)  << QColor(255, 177, 25)
                << QColor(255, 214, 232)  << QColor(234, 0, 1)
                << QColor(20, 214, 232) << QColor(98, 177, 25)
                << QColor(255, 63, 232) << QColor(10, 177, 25);

    QStringList priceNameList;
    priceNameList << "Kiss" << "1024\n红包" << u8"拥抱" << u8"万元红包" << u8"电影" << u8"火锅"
                    << u8"520\n红包" << u8"口红" << u8"烧烤" << u8"全都送" << u8"巧克力\n礼盒"
                    << u8"深圳\n一日游";
    for(int i = 0; i < m_indexList.size(); ++i) {
        m_prizeMap.insert(m_indexList.at(i), priceNameList.at(i));
        m_colorMap.insert(m_indexList.at(i), colorList.at(i));
    }

    //设置概率分布
    m_optionMap.insert(0, RangValue(0, 0.05));
    m_optionMap.insert(1, RangValue(0.05, 0.08));
    m_optionMap.insert(2, RangValue(0.08, 0.13));
    m_optionMap.insert(3, RangValue(0.13, 0.135));
    m_optionMap.insert(4, RangValue(0.135, 0.235));
    m_optionMap.insert(5, RangValue(0.235, 0.335));
    m_optionMap.insert(6, RangValue(0.335, 0.535));
    m_optionMap.insert(7, RangValue(0.535, 0.835));
    m_optionMap.insert(8, RangValue(0.835, 0.885));
    m_optionMap.insert(9, RangValue(0.885, 0.89));
    m_optionMap.insert(10, RangValue(0.89, 0.94));
    m_optionMap.insert(11, RangValue(0.94, 1));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &MainWindow::onStartLottery);

}


void MainWindow::onStartLottery()
{
    m_index++;
    if(m_index >= 12) {
        m_index = 0;
        m_circle++;
    }

    //第二圈开始减速
    if(m_circle >= 2) {
        m_pTimer->start((m_circle)*120);
        //第5圈停
        if(m_circle >= 4 && m_index == m_prizeIndex)
        {
            update();
            m_pTimer->stop();
            //延时一下，不然show的时候会卡
            QTimer::singleShot(10, this, [=]{
                m_pMaskWidget->setPrizeText(m_prizeMap.value(m_indexList.at(m_index)));
                m_pMaskWidget->show();
            });
            m_pMiddleWidget->stop();
        }
    }
    update();
}

void MainWindow::onStart()
{
    if(!m_pTimer->isActive()) {
        m_circle = 0;
        m_index = 0;
        m_pTimer->start(100);
        //生成随机数
        int a = qrand()%m_srandBase;
        m_prizeIndex = getOption(a);
        qDebug() << __FUNCTION__ << "prize index" << m_prizeIndex;
    }
}


int MainWindow::getOption(int rand)
{
    int index = 0;
    qreal percentVal = (qreal)rand/(qreal)m_srandBase;
    QMapIterator<int, RangValue> iter(m_optionMap);
    while (iter.hasNext()) {
        iter.next();
        RangValue rangVal = iter.value();
        if(rangVal.contain(percentVal)) {
            index = iter.key();
            break;
        }
    }
    return index;
}
