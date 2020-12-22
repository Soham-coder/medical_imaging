#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matdisplay.h"
#include<QPixmap>
#include<QFileDialog>
#include<QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*To set the limits of spinbox according to max height and width of the image/QLabel*/
    ui->spinBoxX1->setRange(0,ui->lblMouse->width()-1);
    ui->spinBoxY1->setRange(0,ui->lblMouse->height()-1);
    ui->horizontalSliderX1->setRange(0,ui->lblMouse->width()-1);
    ui->horizontalSliderY1->setRange(0,ui->lblMouse->height()-1);

    ui->spinBoxX2->setRange(0,ui->lblImage->width()-1);
    ui->spinBoxY2->setRange(0,ui->lblImage->height()-1);
    ui->horizontalSliderX2->setRange(0,ui->lblImage->width()-1);
    ui->horizontalSliderY2->setRange(0,ui->lblImage->height()-1);

    /*Connect MouseMove event of QLabels to show in QLabels of Main Window*/
    connect(ui->lblMouse,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
    connect(ui->lblImage,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
    /*To send the location of image file to QLabel in order to load it*/
    connect(this,SIGNAL(loadImage(QString)),ui->lblMouse,SLOT(getImgFileName(QString)));
    connect(this,SIGNAL(loadImage2(QString)),ui->lblImage,SLOT(getImgFileName(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*Gets the coordinates of cursor from QLabel Signal - sendMousePosition and displays the coordinates in the corresponding Label*/
void MainWindow::showMousePosition(QPoint &pos)
{
    double x,y;
    if(pos.x()>ui->lblMouse->width()) /*If x value greater than the max Width of QLabel, it means this coordinate is for
    the 2nd Label (lblImage), hence MaxWidth and Max height are subtracted from x & y coordinates respectively */
    {
        x=pos.x()-ui->lblMouse->width();
        y=pos.y()-ui->lblMouse->height();
        ui->lblMouse_Position2->setText("X = " + QString::number(x) + " ,Y = " + QString::number(y));
        /*Calculate 4 neighbour and 8 Neighbour and print in the corresponding QLabels */
        ui->lbl4nbour2->setText("Up = (" + QString::number(x) + "," + QString::number(y-1) +
                                ")\nDown = (" + QString::number(x) + "," + QString::number(y+1) +
                                ")\nLeft = (" + QString::number(x-1) + "," + QString::number(y) +
                                ")\nRight = (" + QString::number(x+1) + "," + QString::number(y) +")");

        ui->lbl8nbour2->setText("Up = (" + QString::number(x) + "," + QString::number(y-1) +
                                ")\nDown = (" + QString::number(x) + "," + QString::number(y+1) +
                                ")\nLeft = (" + QString::number(x-1) + "," + QString::number(y) +
                                ")\nRight = (" + QString::number(x+1) + "," + QString::number(y) +
                                ")\nUpLeft = (" + QString::number(x-1) + "," + QString::number(y-1) +
                                ")\nUpRight = (" + QString::number(x+1) + "," + QString::number(y-1) +
                                ")\nDownLeft = (" + QString::number(x-1) + "," + QString::number(y+1) +
                                ")\nDownRight = (" + QString::number(x+1)+ "," + QString::number(y+1) +")");
    }
    else /*Coordinates for the 1st Image (lblMouse)*/
    {

        ui->lblMouse_Position->setText("X = " + QString::number(pos.x()) + " ,Y = " + QString::number(pos.y()));
        /*Calculate 4 neighbour and 8 Neighbour and print in the corresponding QLabels */
        ui->lbl4nbour1->setText("Up = (" + QString::number(pos.x()) + "," + QString::number(pos.y()-1) +
                                ")\nDown = (" + QString::number(pos.x()) + "," + QString::number(pos.y()+1) +
                                ")\nLeft = (" + QString::number(pos.x()-1) + "," + QString::number(pos.y()) +
                                ")\nRight = (" + QString::number(pos.x()+1) + "," + QString::number(pos.y())+")");
        ui->lbl8nbour1->setText("Up = (" + QString::number(pos.x()) + "," + QString::number(pos.y()-1) +
                                ")\nDown = (" + QString::number(pos.x()) + "," + QString::number(pos.y()+1) +
                                ")\nLeft = (" + QString::number(pos.x()-1) + "," + QString::number(pos.y()) +
                                ")\nRight = (" + QString::number(pos.x()+1) + "," + QString::number(pos.y()) +
                                ")\nUpLeft = (" + QString::number(pos.x()-1) + "," + QString::number(pos.y()-1) +
                                ")\nUpRight = (" + QString::number(pos.x()+1) + "," + QString::number(pos.y()-1) +
                                ")\nDownLeft = (" + QString::number(pos.x()-1) + "," + QString::number(pos.y()+1) +
                                ")\nDownRight = (" + QString::number(pos.x()+1)+ "," + QString::number(pos.y()+1) +")");
    }

}
/*Function to select the image to be uploaded*/
void MainWindow::on_btnLoadImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.png *.jpg *.jpeg *.bmp *.gif"));
        if(QString::compare(filename,"")!=0)
        {
            emit loadImage(filename);
        }

}
/*Function to select the image to be uploaded in 2nd Label*/
void MainWindow::on_btnLoadImage2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.png *.jpg *.jpeg *.bmp *.gif"));
        if(QString::compare(filename,"")!=0)
        {
            emit loadImage2(filename);
        }
}
/*Gets the x and y values from Slider and sets it in the 1st Image*/
void MainWindow::on_btnRefresh1_clicked()
{
    double x,y;
    x=ui->lblMouse->x();
    y=ui->lblMouse->y()+12;
    QPoint newPoint(ui->spinBoxX1->value()+x,ui->spinBoxY1->value()+y);
    ui->lblMouse->cursor().setPos(QWidget::mapToParent(newPoint));
}
/*Gets the x and y values from Slider and sets it in the 2nd Image*/
void MainWindow::on_btnRefresh2_clicked()
{
    double x,y;
    x=ui->lblImage->x();
    y=ui->lblImage->y()+12;
    QPoint newPoint(ui->spinBoxX2->value()+x,ui->spinBoxY2->value()+y);
    ui->lblImage->cursor().setPos(QWidget::mapToParent(newPoint));
}
