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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <matdisplay.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    matDisplay *lblMouse;
    QLabel *lblMouse_Position;
    QPushButton *btnLoadImage;
    matDisplay *lblImage;
    QLabel *lblMouse_Position2;
    QPushButton *btnLoadImage2;
    QSlider *horizontalSliderX1;
    QSpinBox *spinBoxX1;
    QSlider *horizontalSliderY1;
    QSpinBox *spinBoxY1;
    QSlider *horizontalSliderX2;
    QSlider *horizontalSliderY2;
    QSpinBox *spinBoxX2;
    QSpinBox *spinBoxY2;
    QGroupBox *groupBox;
    QLabel *lbl4nbour1;
    QGroupBox *groupBox_2;
    QLabel *lbl4nbour2;
    QGroupBox *groupBox_3;
    QLabel *lbl8nbour1;
    QGroupBox *groupBox_4;
    QLabel *lbl8nbour2;
    QPushButton *btnRefresh1;
    QPushButton *btnRefresh2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(935, 704);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lblMouse = new matDisplay(centralWidget);
        lblMouse->setObjectName(QStringLiteral("lblMouse"));
        lblMouse->setGeometry(QRect(30, 20, 381, 331));
        lblMouse->setCursor(QCursor(Qt::CrossCursor));
        lblMouse->setMouseTracking(true);
        lblMouse->setStyleSheet(QStringLiteral(""));
        lblMouse->setFrameShape(QFrame::Box);
        lblMouse_Position = new QLabel(centralWidget);
        lblMouse_Position->setObjectName(QStringLiteral("lblMouse_Position"));
        lblMouse_Position->setGeometry(QRect(30, 360, 221, 41));
        lblMouse_Position->setFrameShape(QFrame::Box);
        lblMouse_Position->setFrameShadow(QFrame::Raised);
        lblMouse_Position->setAlignment(Qt::AlignCenter);
        btnLoadImage = new QPushButton(centralWidget);
        btnLoadImage->setObjectName(QStringLiteral("btnLoadImage"));
        btnLoadImage->setGeometry(QRect(30, 420, 111, 31));
        lblImage = new matDisplay(centralWidget);
        lblImage->setObjectName(QStringLiteral("lblImage"));
        lblImage->setGeometry(QRect(510, 10, 381, 331));
        lblImage->setMouseTracking(true);
        lblImage->setFrameShape(QFrame::Box);
        lblImage->setAlignment(Qt::AlignCenter);
        lblMouse_Position2 = new QLabel(centralWidget);
        lblMouse_Position2->setObjectName(QStringLiteral("lblMouse_Position2"));
        lblMouse_Position2->setGeometry(QRect(530, 360, 221, 41));
        lblMouse_Position2->setFrameShape(QFrame::Box);
        lblMouse_Position2->setFrameShadow(QFrame::Raised);
        lblMouse_Position2->setAlignment(Qt::AlignCenter);
        btnLoadImage2 = new QPushButton(centralWidget);
        btnLoadImage2->setObjectName(QStringLiteral("btnLoadImage2"));
        btnLoadImage2->setGeometry(QRect(530, 410, 111, 31));
        horizontalSliderX1 = new QSlider(centralWidget);
        horizontalSliderX1->setObjectName(QStringLiteral("horizontalSliderX1"));
        horizontalSliderX1->setGeometry(QRect(30, 530, 160, 22));
        horizontalSliderX1->setOrientation(Qt::Horizontal);
        spinBoxX1 = new QSpinBox(centralWidget);
        spinBoxX1->setObjectName(QStringLiteral("spinBoxX1"));
        spinBoxX1->setGeometry(QRect(200, 530, 42, 22));
        horizontalSliderY1 = new QSlider(centralWidget);
        horizontalSliderY1->setObjectName(QStringLiteral("horizontalSliderY1"));
        horizontalSliderY1->setGeometry(QRect(30, 570, 160, 22));
        horizontalSliderY1->setOrientation(Qt::Horizontal);
        spinBoxY1 = new QSpinBox(centralWidget);
        spinBoxY1->setObjectName(QStringLiteral("spinBoxY1"));
        spinBoxY1->setGeometry(QRect(200, 560, 42, 22));
        horizontalSliderX2 = new QSlider(centralWidget);
        horizontalSliderX2->setObjectName(QStringLiteral("horizontalSliderX2"));
        horizontalSliderX2->setGeometry(QRect(529, 530, 161, 22));
        horizontalSliderX2->setOrientation(Qt::Horizontal);
        horizontalSliderY2 = new QSlider(centralWidget);
        horizontalSliderY2->setObjectName(QStringLiteral("horizontalSliderY2"));
        horizontalSliderY2->setGeometry(QRect(530, 570, 160, 22));
        horizontalSliderY2->setOrientation(Qt::Horizontal);
        spinBoxX2 = new QSpinBox(centralWidget);
        spinBoxX2->setObjectName(QStringLiteral("spinBoxX2"));
        spinBoxX2->setGeometry(QRect(700, 530, 42, 22));
        spinBoxY2 = new QSpinBox(centralWidget);
        spinBoxY2->setObjectName(QStringLiteral("spinBoxY2"));
        spinBoxY2->setGeometry(QRect(700, 560, 42, 22));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(260, 350, 151, 111));
        lbl4nbour1 = new QLabel(groupBox);
        lbl4nbour1->setObjectName(QStringLiteral("lbl4nbour1"));
        lbl4nbour1->setGeometry(QRect(20, 20, 121, 81));
        lbl4nbour1->setFrameShape(QFrame::Box);
        lbl4nbour1->setAlignment(Qt::AlignCenter);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(760, 350, 141, 111));
        lbl4nbour2 = new QLabel(groupBox_2);
        lbl4nbour2->setObjectName(QStringLiteral("lbl4nbour2"));
        lbl4nbour2->setGeometry(QRect(20, 20, 111, 81));
        lbl4nbour2->setFrameShape(QFrame::Box);
        lbl4nbour2->setAlignment(Qt::AlignCenter);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(260, 470, 151, 181));
        lbl8nbour1 = new QLabel(groupBox_3);
        lbl8nbour1->setObjectName(QStringLiteral("lbl8nbour1"));
        lbl8nbour1->setGeometry(QRect(20, 20, 121, 151));
        lbl8nbour1->setFrameShape(QFrame::Box);
        lbl8nbour1->setAlignment(Qt::AlignCenter);
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(760, 470, 161, 181));
        lbl8nbour2 = new QLabel(groupBox_4);
        lbl8nbour2->setObjectName(QStringLiteral("lbl8nbour2"));
        lbl8nbour2->setGeometry(QRect(10, 20, 141, 151));
        lbl8nbour2->setFrameShape(QFrame::Box);
        lbl8nbour2->setAlignment(Qt::AlignCenter);
        btnRefresh1 = new QPushButton(centralWidget);
        btnRefresh1->setObjectName(QStringLiteral("btnRefresh1"));
        btnRefresh1->setGeometry(QRect(80, 600, 111, 23));
        btnRefresh2 = new QPushButton(centralWidget);
        btnRefresh2->setObjectName(QStringLiteral("btnRefresh2"));
        btnRefresh2->setGeometry(QRect(544, 610, 111, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 935, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(horizontalSliderX1, SIGNAL(valueChanged(int)), spinBoxX1, SLOT(setValue(int)));
        QObject::connect(horizontalSliderY1, SIGNAL(valueChanged(int)), spinBoxY1, SLOT(setValue(int)));
        QObject::connect(horizontalSliderX2, SIGNAL(valueChanged(int)), spinBoxX2, SLOT(setValue(int)));
        QObject::connect(horizontalSliderY2, SIGNAL(valueChanged(int)), spinBoxY2, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        lblMouse->setText(QString());
        lblMouse_Position->setText(QApplication::translate("MainWindow", "X = ? , Y = ?", 0));
        btnLoadImage->setText(QApplication::translate("MainWindow", "Load Image", 0));
        lblImage->setText(QString());
        lblMouse_Position2->setText(QApplication::translate("MainWindow", "X = ? , Y = ?", 0));
        btnLoadImage2->setText(QApplication::translate("MainWindow", "Load Image", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "4neighbour", 0));
        lbl4nbour1->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "4neighbour", 0));
        lbl4nbour2->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "8neighbour", 0));
        lbl8nbour1->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "8neighbour", 0));
        lbl8nbour2->setText(QString());
        btnRefresh1->setText(QApplication::translate("MainWindow", "Refresh Image 1", 0));
        btnRefresh2->setText(QApplication::translate("MainWindow", "Refresh Image 2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
