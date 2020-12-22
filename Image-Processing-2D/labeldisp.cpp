#include "labeldisp.h"
#include<QPoint>
#include<QPixmap>
#include<QFileDialog>
#include<QException>
LabelDisp::LabelDisp(QWidget *parent) :
    QLabel(parent)
{
    fname="";  /*Default or initial Location from which picture to be uploaded*/
    color = Qt::red;
    pixmap = new QPixmap(fname);
    QRgb temp = qRgb(0,0,0);
    pixMat = new QRgb*[350];
    opPixMat = new QRgb*[350];
    for(int i=0;i<350;i++)
    {
        pixMat[i] = new QRgb[350];
        opPixMat[i] = new QRgb[350];
    }
    for(int i=0;i<350;i++)
    {
        for(int j=0;j<350;j++)
        {
            pixMat[i][j] = temp;
            opPixMat[i][j] = temp;
        }
    }
}

void LabelDisp::mouseMoveEvent(QMouseEvent *ev)
{
    QString fileName = this->objectName();
    double x,y;
    point = ev->pos();
    if(point.x() <= this->size().width() && point.y() <= this->size().height())
    {
        if(fileName.compare("ipimage")==0) /*If the current QLabel is lblMouse then send coordinate as it is*/
        {
            if(point.x() >=0 && point.y()>=0)
            {
                try
                {
                    emit sendPixelInfo(pixMat[point.x()][point.y()]);
                }
                catch(...)
                {

                }

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
            try{
                emit sendPixelInfo(opPixMat[point.x()][point.y()]);
            }
            catch(...)
            {

            }

            emit sendMousePosition(temp);
        }
        //draw(ev);
        repaint();
    }
}

void LabelDisp::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    //image.load(fname);
    //image = image.scaledToWidth(this->size().width(),Qt::SmoothTransformation);
    //QPixmap img = QPixmap::fromImage(image);
    pa.drawPixmap(0,0,*pixmap);
    /*pa.setPen(QPen(Qt::red,1));
    pa.drawLine(point.x(),0,point.x(),this->size().height());
    pa.setPen(QPen(Qt::blue,1));
    pa.drawLine(0,point.y(),this->size().width(),point.y());*/
}

void LabelDisp::mousePressEvent(QMouseEvent *ev)
{
    QString fName = this->objectName();
    if(fName.compare("ipimage")==0)
    {
        pixmap = new QPixmap(fname);
    }
    else
    {
        pixmap = new QPixmap(opimage);
    }
    if (ev->button() == Qt::RightButton)
                color = color == Qt::red ? Qt::blue : Qt::red;
            else
            {
                pressed = 1;
                drawGrid(ev);
                draw(ev);
            }
}

void LabelDisp::mouseReleaseEvent(QMouseEvent *ev)
{
    /*QPainter p(this);
    p.drawPixmap(0,0,*pixmap);*/
    pressed = 0;
}

void LabelDisp::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QString fName = this->objectName();
    point=ev->pos();
    if(fName.compare("ipimage")==0)
    {
        if(ev->button()==Qt::LeftButton)
        {
            emit seedInfo(point,pixMat[ev->pos().x()][ev->pos().y()]);
        }
    }
    else
    {
        if(ev->button()==Qt::LeftButton)
        {
            emit seedInfo(point,opPixMat[ev->pos().x()][ev->pos().y()]);
        }
    }
}

void LabelDisp::leaveEvent(QEvent *ev)
{
    QString fName = this->objectName();
    if(fName.compare("ipimage")==0)
    {
        pixmap = new QPixmap(fname);
    }
    else
    {
        pixmap = new QPixmap(opimage);
    }
    repaint();
}


void LabelDisp::getImgFileName(QString fileName)
{
    int i,j,rows,cols;
        fname = fileName;
        bool valid = image.load(fname);
        if(valid)
        {
            rows=image.height();
            cols=image.width();
            pixMat = new QRgb*[rows];
            for(i=0;i<rows;i++)
            {
                pixMat[i] = new QRgb[cols];
            }
            image=image.convertToFormat(QImage::Format_RGB32);
            for(i=0;i<rows;i++)
            {
                for(j=0;j<cols;j++)
                {
                    pixMat[i][j] = image.pixel(i,j);
                }
            }
            pixmap = new QPixmap(fname);
            repaint();  /*Repaint actually draws the loaded image in the QLabel*/
        }
        else
        {
            this->setText("Failed to Load Image");
        }
}

void LabelDisp::getPixmap(QImage &img)
{
    int i,j,rows,cols;
        rows = img.height();
        cols = img.width();
        pixmap = new QPixmap(QPixmap::fromImage(img));
        opimage = *pixmap;
        opPixMat = new QRgb*[rows];
        for(i=0;i<rows;i++)
        {
            opPixMat[i] = new QRgb[cols];
        }
        img=img.convertToFormat(QImage::Format_RGB32);
        for(i=0;i<rows;i++)
        {
            for(j=0;j<cols;j++)
            {
                opPixMat[i][j] = img.pixel(i,j);
            }
        }
        repaint();
}

/**/
void LabelDisp::draw(QMouseEvent *e)
{
    if (pressed) {
        QPainter painter(pixmap);
        painter.setPen(color);
        int x = e->pos().x();
        int y = e->pos().y();
        painter.drawPoint(x+1,y);
        painter.drawPoint(x-1,y);
        painter.drawPoint(x,y+1);
        painter.drawPoint(x,y-1);
        repaint();
    }
}

void LabelDisp::drawGrid(QMouseEvent *ev)
{
    if (pressed) {
        QPainter painter(pixmap);
        painter.setPen(color);
        int x = ev->pos().x();
        int y = ev->pos().y();

        painter.setPen(QPen(Qt::red,1));
        painter.drawLine(x,0,x,this->size().height());
        painter.setPen(QPen(Qt::blue,1));
        painter.drawLine(0,y,this->size().width(),y);
        repaint();
    }
}
