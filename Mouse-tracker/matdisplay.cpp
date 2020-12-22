#include "matdisplay.h"
#include "mainwindow.h"
#include<QPoint>
#include<QPixmap>
#include<QFileDialog>
matDisplay::matDisplay(QWidget *parent) :
    QLabel(parent)
{
    fname="C:/Users/DELL/Desktop/MTECH PROJECT/Workshop/MouseTracker/g1.png";  /*Default or initial Location from which picture to be uploaded*/
}
/*Draws the image in the label and then draws the criss cross lines*/
void matDisplay::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.setPen(QPen(Qt::white,1));
    image.load(fname);
    image = image.scaledToWidth(this->size().width(),Qt::SmoothTransformation);
    pa.drawImage(0,0,image);
    pa.drawLine(point.x(),0,point.x(),this->size().height());
    pa.drawLine(0,point.y(),this->size().width(),point.y());
}
/*Mouse event to get the current cursor coordinates and send it to Main Window*/
void matDisplay::mouseMoveEvent(QMouseEvent *ev)
{
    QString fileName = this->objectName();
    double x,y;
    point = ev->pos();
    if(point.x() <= this->size().width() && point.y() <= this->size().height())
    {
        if(fileName.compare("lblMouse")==0) /*If the current QLabel is lblMouse then send coordinate as it is*/
        {
            if(point.x() >=0 && point.y()>=0)
            {
                emit sendMousePosition(point);
            }
        }
        else /*The current QLabel is the lblImage, hence modify the coordinates by adding maxWidth & MaxHeight so that
        it can be calculated properly for 2nd Image in Main Window*/
        {
            x=point.x();
            y=point.y();
            x=x+this->size().width();
            y=y+this->size().height();
            QPoint temp(x,y);
            emit sendMousePosition(temp);
        }
        repaint();
    }
}
/*Gets the location of image file sent from Main Window and loads that image*/
void matDisplay::getImgFileName(QString fileName)
{
    fname = fileName;
    bool valid = image.load(fname);
    if(valid)
    {
        repaint();  /*Repaint actually draws the loaded image in the QLabel*/
    }
    else
    {
        this->setText("Failed to Load Image");
    }
}
