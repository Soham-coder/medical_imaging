/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <labeldisp.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    LabelDisp *ipimage;
    LabelDisp *opimage;
    QLabel *label_5;
    QLabel *lblMouse_Position;
    QPushButton *loadimage;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *Redvalue;
    QLabel *label_10;
    QLabel *Greenvalue;
    QLabel *label_12;
    QLabel *Bluevalue;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *binarize;
    QPushButton *dt8;
    QPushButton *histogramequilization;
    QPushButton *ccl4;
    QPushButton *colormap;
    QPushButton *FDT;
    QPushButton *localmaxima;
    QGroupBox *groupBox_3;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_14;
    QLabel *nocc;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout;
    QSlider *horizontalSliderX1;
    QSpinBox *spinBoxX1;
    QSlider *horizontalSliderY1;
    QSpinBox *spinBoxY1;
    QPushButton *btnRefresh1;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout_2;
    QSlider *horizontalSliderX2;
    QSpinBox *spinBoxX2;
    QSlider *horizontalSliderY2;
    QSpinBox *spinBoxY2;
    QPushButton *btnRefresh2;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLabel *widthvalue;
    QLabel *label_3;
    QLabel *heightvalue;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1142, 784);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(249, 255, 229);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ipimage = new LabelDisp(centralWidget);
        ipimage->setObjectName(QStringLiteral("ipimage"));
        ipimage->setGeometry(QRect(10, 20, 350, 350));
        ipimage->setMinimumSize(QSize(350, 350));
        ipimage->setCursor(QCursor(Qt::CrossCursor));
        ipimage->setMouseTracking(true);
        ipimage->setFrameShape(QFrame::NoFrame);
        ipimage->setLineWidth(3);
        ipimage->setScaledContents(true);
        opimage = new LabelDisp(centralWidget);
        opimage->setObjectName(QStringLiteral("opimage"));
        opimage->setGeometry(QRect(470, 20, 350, 350));
        opimage->setMinimumSize(QSize(350, 350));
        opimage->setCursor(QCursor(Qt::ArrowCursor));
        opimage->setMouseTracking(true);
        opimage->setFrameShape(QFrame::NoFrame);
        opimage->setFrameShadow(QFrame::Raised);
        opimage->setLineWidth(2);
        opimage->setMidLineWidth(2);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(200, 390, 111, 41));
        QFont font;
        font.setPointSize(11);
        label_5->setFont(font);
        label_5->setCursor(QCursor(Qt::ArrowCursor));
        label_5->setAlignment(Qt::AlignCenter);
        lblMouse_Position = new QLabel(centralWidget);
        lblMouse_Position->setObjectName(QStringLiteral("lblMouse_Position"));
        lblMouse_Position->setGeometry(QRect(330, 390, 121, 41));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        lblMouse_Position->setFont(font1);
        lblMouse_Position->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        lblMouse_Position->setFrameShape(QFrame::Box);
        lblMouse_Position->setAlignment(Qt::AlignCenter);
        loadimage = new QPushButton(centralWidget);
        loadimage->setObjectName(QStringLiteral("loadimage"));
        loadimage->setGeometry(QRect(10, 380, 181, 51));
        loadimage->setFont(font1);
        loadimage->setStyleSheet(QStringLiteral("background-color: rgb(224, 220, 255);"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 351, 371));
        QFont font2;
        font2.setPointSize(10);
        groupBox->setFont(font2);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(470, 0, 351, 371));
        groupBox_2->setFont(font2);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 440, 481, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font);

        horizontalLayout->addWidget(label_7);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        label_8->setFont(font3);
        label_8->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_8);

        Redvalue = new QLabel(layoutWidget);
        Redvalue->setObjectName(QStringLiteral("Redvalue"));
        Redvalue->setFont(font3);
        Redvalue->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        Redvalue->setFrameShape(QFrame::Box);
        Redvalue->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Redvalue);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font3);
        label_10->setStyleSheet(QStringLiteral("color: rgb(0, 255, 0);"));
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_10);

        Greenvalue = new QLabel(layoutWidget);
        Greenvalue->setObjectName(QStringLiteral("Greenvalue"));
        Greenvalue->setFont(font3);
        Greenvalue->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        Greenvalue->setFrameShape(QFrame::Box);
        Greenvalue->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Greenvalue);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font3);
        label_12->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        label_12->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_12);

        Bluevalue = new QLabel(layoutWidget);
        Bluevalue->setObjectName(QStringLiteral("Bluevalue"));
        Bluevalue->setFont(font3);
        Bluevalue->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        Bluevalue->setFrameShape(QFrame::Box);
        Bluevalue->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Bluevalue);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 490, 1061, 41));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        binarize = new QPushButton(layoutWidget1);
        binarize->setObjectName(QStringLiteral("binarize"));
        binarize->setEnabled(false);
        binarize->setMinimumSize(QSize(0, 24));
        binarize->setFont(font1);

        horizontalLayout_2->addWidget(binarize);

        dt8 = new QPushButton(layoutWidget1);
        dt8->setObjectName(QStringLiteral("dt8"));
        dt8->setEnabled(false);
        dt8->setFont(font1);

        horizontalLayout_2->addWidget(dt8);

        histogramequilization = new QPushButton(layoutWidget1);
        histogramequilization->setObjectName(QStringLiteral("histogramequilization"));
        histogramequilization->setEnabled(false);
        histogramequilization->setFont(font1);

        horizontalLayout_2->addWidget(histogramequilization);

        ccl4 = new QPushButton(layoutWidget1);
        ccl4->setObjectName(QStringLiteral("ccl4"));
        ccl4->setEnabled(false);
        ccl4->setFont(font1);

        horizontalLayout_2->addWidget(ccl4);

        colormap = new QPushButton(layoutWidget1);
        colormap->setObjectName(QStringLiteral("colormap"));
        colormap->setEnabled(false);
        colormap->setFont(font1);

        horizontalLayout_2->addWidget(colormap);

        FDT = new QPushButton(layoutWidget1);
        FDT->setObjectName(QStringLiteral("FDT"));
        FDT->setEnabled(false);
        FDT->setFont(font1);

        horizontalLayout_2->addWidget(FDT);

        localmaxima = new QPushButton(layoutWidget1);
        localmaxima->setObjectName(QStringLiteral("localmaxima"));
        localmaxima->setEnabled(false);
        localmaxima->setFont(font1);

        horizontalLayout_2->addWidget(localmaxima);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(720, 390, 321, 80));
        groupBox_3->setFont(font);
        horizontalSlider = new QSlider(groupBox_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(10, 30, 160, 22));
        horizontalSlider->setMaximum(255);
        horizontalSlider->setValue(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksBelow);
        horizontalSlider->setTickInterval(10);
        spinBox = new QSpinBox(groupBox_3);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(180, 30, 42, 22));
        spinBox->setMaximum(255);
        spinBox->setValue(100);
        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(350, 540, 251, 31));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(layoutWidget2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font2);
        label_14->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_14);

        nocc = new QLabel(layoutWidget2);
        nocc->setObjectName(QStringLiteral("nocc"));
        nocc->setFont(font1);
        nocc->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        nocc->setFrameShape(QFrame::Box);
        nocc->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(nocc);

        layoutWidget3 = new QWidget(centralWidget);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(20, 580, 261, 151));
        gridLayout = new QGridLayout(layoutWidget3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSliderX1 = new QSlider(layoutWidget3);
        horizontalSliderX1->setObjectName(QStringLiteral("horizontalSliderX1"));
        horizontalSliderX1->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderX1, 0, 0, 1, 1);

        spinBoxX1 = new QSpinBox(layoutWidget3);
        spinBoxX1->setObjectName(QStringLiteral("spinBoxX1"));

        gridLayout->addWidget(spinBoxX1, 0, 1, 1, 1);

        horizontalSliderY1 = new QSlider(layoutWidget3);
        horizontalSliderY1->setObjectName(QStringLiteral("horizontalSliderY1"));
        horizontalSliderY1->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderY1, 1, 0, 1, 1);

        spinBoxY1 = new QSpinBox(layoutWidget3);
        spinBoxY1->setObjectName(QStringLiteral("spinBoxY1"));

        gridLayout->addWidget(spinBoxY1, 1, 1, 1, 1);

        btnRefresh1 = new QPushButton(layoutWidget3);
        btnRefresh1->setObjectName(QStringLiteral("btnRefresh1"));
        btnRefresh1->setFont(font);
        btnRefresh1->setStyleSheet(QStringLiteral("background-color: rgb(229, 255, 241);"));

        gridLayout->addWidget(btnRefresh1, 2, 0, 1, 1);

        layoutWidget4 = new QWidget(centralWidget);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(550, 580, 271, 151));
        gridLayout_2 = new QGridLayout(layoutWidget4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSliderX2 = new QSlider(layoutWidget4);
        horizontalSliderX2->setObjectName(QStringLiteral("horizontalSliderX2"));
        horizontalSliderX2->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSliderX2, 0, 0, 1, 1);

        spinBoxX2 = new QSpinBox(layoutWidget4);
        spinBoxX2->setObjectName(QStringLiteral("spinBoxX2"));

        gridLayout_2->addWidget(spinBoxX2, 0, 1, 1, 1);

        horizontalSliderY2 = new QSlider(layoutWidget4);
        horizontalSliderY2->setObjectName(QStringLiteral("horizontalSliderY2"));
        horizontalSliderY2->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSliderY2, 1, 0, 1, 1);

        spinBoxY2 = new QSpinBox(layoutWidget4);
        spinBoxY2->setObjectName(QStringLiteral("spinBoxY2"));

        gridLayout_2->addWidget(spinBoxY2, 1, 1, 1, 1);

        btnRefresh2 = new QPushButton(layoutWidget4);
        btnRefresh2->setObjectName(QStringLiteral("btnRefresh2"));
        btnRefresh2->setFont(font);
        btnRefresh2->setStyleSheet(QStringLiteral("background-color: rgb(229, 255, 241);"));

        gridLayout_2->addWidget(btnRefresh2, 2, 0, 1, 1);

        layoutWidget5 = new QWidget(centralWidget);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(460, 390, 241, 41));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget5);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        horizontalLayout_4->addWidget(label);

        widthvalue = new QLabel(layoutWidget5);
        widthvalue->setObjectName(QStringLiteral("widthvalue"));
        widthvalue->setFont(font1);
        widthvalue->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        widthvalue->setFrameShape(QFrame::Box);
        widthvalue->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(widthvalue);

        label_3 = new QLabel(layoutWidget5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        horizontalLayout_4->addWidget(label_3);

        heightvalue = new QLabel(layoutWidget5);
        heightvalue->setObjectName(QStringLiteral("heightvalue"));
        heightvalue->setFont(font1);
        heightvalue->setStyleSheet(QStringLiteral("background-color: rgb(222, 236, 255);"));
        heightvalue->setFrameShape(QFrame::Box);
        heightvalue->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(heightvalue);

        MainWindow->setCentralWidget(centralWidget);
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        groupBox_2->raise();
        groupBox->raise();
        ipimage->raise();
        opimage->raise();
        label_5->raise();
        lblMouse_Position->raise();
        loadimage->raise();
        groupBox_3->raise();
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(horizontalSliderX1, SIGNAL(valueChanged(int)), spinBoxX1, SLOT(setValue(int)));
        QObject::connect(horizontalSliderY1, SIGNAL(valueChanged(int)), spinBoxY1, SLOT(setValue(int)));
        QObject::connect(horizontalSliderX2, SIGNAL(valueChanged(int)), spinBoxX2, SLOT(setValue(int)));
        QObject::connect(horizontalSliderY2, SIGNAL(valueChanged(int)), spinBoxY2, SLOT(setValue(int)));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        ipimage->setText(QString());
        opimage->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Pixel Coordinate", 0));
        lblMouse_Position->setText(QApplication::translate("MainWindow", "X = ? , Y = ?", 0));
        loadimage->setText(QApplication::translate("MainWindow", "Load Image", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Input Image", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Output Image", 0));
        label_7->setText(QApplication::translate("MainWindow", "RGB Value ->", 0));
        label_8->setText(QApplication::translate("MainWindow", "Red", 0));
        Redvalue->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Green", 0));
        Greenvalue->setText(QString());
        label_12->setText(QApplication::translate("MainWindow", "Blue", 0));
        Bluevalue->setText(QString());
        binarize->setText(QApplication::translate("MainWindow", "Binarize", 0));
        dt8->setText(QApplication::translate("MainWindow", "8 Distance Transform", 0));
        histogramequilization->setText(QApplication::translate("MainWindow", "Histogram Equalization", 0));
        ccl4->setText(QApplication::translate("MainWindow", "4 CCL", 0));
        colormap->setText(QApplication::translate("MainWindow", "Add Colormap", 0));
        FDT->setText(QApplication::translate("MainWindow", "FDT", 0));
        localmaxima->setText(QApplication::translate("MainWindow", "Local Maxima", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Set Threshold for Binarization", 0));
        label_14->setText(QApplication::translate("MainWindow", "No. of Connected Components", 0));
        nocc->setText(QApplication::translate("MainWindow", "0", 0));
        btnRefresh1->setText(QApplication::translate("MainWindow", "Refresh input image", 0));
        btnRefresh2->setText(QApplication::translate("MainWindow", "Refresh ouput Image", 0));
        label->setText(QApplication::translate("MainWindow", "Width", 0));
        widthvalue->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Height", 0));
        heightvalue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
