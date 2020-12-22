#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "labeldisp.h"
#include<QPixmap>
#include<QFileDialog>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<cmath>
#define d1 2
#define d2 3
int cc=0,hist[sizex][sizey],arr[sizex][sizey],dt[sizex][sizey],ipmaxinten=0,maxintendt=0,FDT[sizex][sizey],r,c,value1,flag1,ccl[sizex][sizey],localmax[sizex][sizey],binary[sizex][sizey],threshold =100;
double fuzzy[sizex][sizey];
bool inputimg=false, dtimg8=false, cclimg4=false,cclimg8=false,fdtimg8=false,bina=false,hg=false,lclm=false,hgdtimg8=false,ldt=false,lfdt=false;
struct node
{
    int data;
    struct node *next;
};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*To set the limits of spinbox according to max height and width of the image/QLabel*/
        ui->spinBoxX1->setRange(0,ui->ipimage->width()-1);
        ui->spinBoxY1->setRange(0,ui->ipimage->height()-1);
        ui->horizontalSliderX1->setRange(0,ui->ipimage->width()-1);
        ui->horizontalSliderY1->setRange(0,ui->ipimage->height()-1);

        ui->spinBoxX2->setRange(0,ui->opimage->width()-1);
        ui->spinBoxY2->setRange(0,ui->opimage->height()-1);
        ui->horizontalSliderX2->setRange(0,ui->opimage->width()-1);
        ui->horizontalSliderY2->setRange(0,ui->opimage->height()-1);

        ui->widthvalue->setText(QString::number(ui->ipimage->width()));
        ui->heightvalue->setText(QString::number(ui->ipimage->height()));

    connect(ui->ipimage,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
    connect(ui->opimage,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));

    connect(this,SIGNAL(loadImage(QString)),ui->ipimage,SLOT(getImgFileName(QString)));

    connect(this,SIGNAL(sendPixmap(QImage&)),ui->opimage,SLOT(getPixmap(QImage&)));

    connect(ui->ipimage,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(getPos1(QPoint&)));
    connect(ui->opimage,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(getPos2(QPoint&)));

    connect(ui->ipimage,SIGNAL(sendPixelInfo(QRgb&)),this,SLOT(getPixelInfo(QRgb&)));
    connect(ui->opimage,SIGNAL(sendPixelInfo(QRgb&)),this,SLOT(getPixelInfo(QRgb&)));

    connect(ui->ipimage,SIGNAL(seedInfo(QPoint&,QRgb&)),this,SLOT(getSeedInfo1(QPoint&,QRgb&)));
    connect(ui->opimage,SIGNAL(seedInfo(QPoint&,QRgb&)),this,SLOT(getSeedInfo2(QPoint&,QRgb&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadimage_clicked()
{
    int i,j,red,green,blue,intensity;
    QImage image;
    QRgb value;
    QString filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.png *.jpg *.jpeg *.bmp *.gif"));
        if(QString::compare(filename,"")!=0)
        {
            bool valid;
            valid=image.load(filename);
            if(valid)
            {
                emit loadImage(filename);
                ui->binarize->setEnabled(true);
                ui->dt8->setEnabled(true);
                ui->histogramequilization->setEnabled(true);
                ui->ccl4->setEnabled(true);
                ui->FDT->setEnabled(true);
                ui->localmaxima->setEnabled(true);
            }
            c=image.width();
            r=image.height();
            ui->heightvalue->setNum(r);
            ui->widthvalue->setNum(c);
            ui->ipimage->setMouseTracking(true);
            image=image.convertToFormat(QImage::Format_RGB32);
            value=qRgb(0,0,0);
            for(j=0;j<r;j++)
            {
                for(i=0;i<c;i++)
                {
                    value=image.pixel(i,j);
                    red=qRed(value);
                    green=qGreen(value);
                    blue=qBlue(value);
                    intensity=(red+green+blue)/3;
                    arr[i][j]=intensity;
                    if(intensity>ipmaxinten)ipmaxinten=intensity;
                }
            }
        }
}
void MainWindow::showMousePosition(QPoint &pos)
{
    double x,y;
    if(pos.x()>ui->ipimage->width()) /*If x value greater than the max Width of QLabel, it means this coordinate is for
    the 2nd Label (lblImage), hence MaxWidth and Max height are subtracted from x & y coordinates respectively */
    {
        x=pos.x()-ui->ipimage->width();
        y=pos.y()-ui->ipimage->height();
        ui->lblMouse_Position->setText("X = " + QString::number(x) + " ,Y = " + QString::number(y));
        /*Calculate 4 neighbour and 8 Neighbour and print in the corresponding QLabels
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
                                ")\nDownRight = (" + QString::number(x+1)+ "," + QString::number(y+1) +")");*/
    }
    else /*Coordinates for the 1st Image (lblMouse)*/
    {

        ui->lblMouse_Position->setText("X = " + QString::number(pos.x()) + " ,Y = " + QString::number(pos.y()));
        /*Calculate 4 neighbour and 8 Neighbour and print in the corresponding QLabels
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
                                ")\nDownRight = (" + QString::number(pos.x()+1)+ "," + QString::number(pos.y()+1) +")");*/
    }
}

void MainWindow::getPos1(QPoint &pos)
{
    ui->horizontalSliderX1->setValue(pos.x());
    ui->horizontalSliderY1->setValue(pos.y());
}

void MainWindow::getPos2(QPoint &pos)
{
    double x,y;
    x=pos.x()-ui->ipimage->width();
    y=pos.y()-ui->ipimage->height();
    ui->horizontalSliderX2->setValue(x);
    ui->horizontalSliderY2->setValue(y);
}

void MainWindow::getPixelInfo(QRgb &pixVal)
{
    int red,green,blue;
    red = qRed(pixVal);
    blue = qBlue(pixVal);
    green = qGreen(pixVal);
    ui->Redvalue->setText(QString::number(red));
    ui->Bluevalue->setText(QString::number(blue));
    ui->Greenvalue->setText(QString::number(green));
}

void MainWindow::getSeedInfo1(QPoint &pos, QRgb &val)
{
    QFile seedFile("Input_Seed.txt");
    int red,green,blue;
    red = qRed(val);
    blue = qBlue(val);
    green = qGreen(val);
    ui->Redvalue->setText(QString::number(red));
    ui->Bluevalue->setText(QString::number(blue));
    ui->Greenvalue->setText(QString::number(green));
    QString seedVal = "Coordinate-> ("+QString::number(pos.x())+","+QString::number(pos.y())+")\tR="+QString::number(red)+" G="+QString::number(green)+" B="+QString::number(blue)+"\n";
    if(!seedFile.open(QFile::Append|QFile::Text))
    {
        qDebug()<<"Could not open file for writing";
    }
    else
    {
        QTextStream out(&seedFile);
        out<<seedVal;
    }
    seedFile.flush();
    seedFile.close();
}
void MainWindow::getSeedInfo2(QPoint &pos, QRgb &val)
{
    QFile seedFile("Output_Seed.txt");
    int red,green,blue;
    red = qRed(val);
    blue = qBlue(val);
    green = qGreen(val);
    ui->Redvalue->setText(QString::number(red));
    ui->Bluevalue->setText(QString::number(blue));
    ui->Greenvalue->setText(QString::number(green));
    QString seedVal = "Coordinate-> ("+QString::number(pos.x())+","+QString::number(pos.y())+")\tR="+QString::number(red)+" G="+QString::number(green)+" B="+QString::number(blue)+"\n";
    if(!seedFile.open(QFile::Append|QFile::Text))
    {
        qDebug()<<"Could not open file for writing";
    }
    else
    {
        QTextStream out(&seedFile);
        out<<seedVal;
    }
    seedFile.flush();
    seedFile.close();
}
void MainWindow::on_btnRefresh1_clicked()
{
    double x,y;
    x=ui->ipimage->x();
    y=ui->ipimage->y();

    QPoint newPoint(ui->spinBoxX1->value()+x,ui->spinBoxY1->value()+y);
    ui->ipimage->cursor().setPos(QWidget::mapToParent(newPoint));
}

void MainWindow::on_btnRefresh2_clicked()
{
    double x,y;
        x=ui->opimage->x();
        y=ui->opimage->y();

        QPoint newPoint(ui->spinBoxX2->value()+x,ui->spinBoxY2->value()+y);
        ui->opimage->cursor().setPos(QWidget::mapToParent(newPoint));
}

void MainWindow::on_binarize_clicked()
{
    ui->colormap->setEnabled(false);
    int i,j;
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(arr[i][j]>threshold)
                binary[i][j]=255;
            else
                binary[i][j]=0;
        }
    }
    inputimg=false;dtimg8=false;cclimg4=false;cclimg8=false;fdtimg8=false;bina=true;hg=false;lclm=false;
    drawimage();
}
void MainWindow::drawimage()
{
    int i,j;
    QImage opimg(c,r,QImage::Format_RGB32);
    QRgb value;
    if(dtimg8==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                if(dt[i][j]>255)
                {
                    //x=(dt[i][j]/(double)maxintendt);
                    //if(x!=0)printf("X is non zero");
                    //value1=(double)((double)x*255);
                     value1=255;
                }
                else value1=dt[i][j];
                value=qRgb(value1,value1,value1);
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
    else if(hg==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                value1=hist[i][j];
                value=qRgb(value1,value1,value1);
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
    else if(bina==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                value1=binary[i][j];
                value=qRgb(value1,value1,value1);
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
    else if(fdtimg8==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                value1=FDT[i][j];
                value=qRgb(value1,value1,value1);
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
    else if(cclimg4==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                value1=ccl[i][j];
                if(value1!=0)
                {
                      if(cc==1)
                      {
                       value=qRgb(0,0,255);
                      }
                      else if(cc==2)
                      {
                        if(value1==2)
                        value=qRgb(0,0,255);
                        else
                        value=qRgb(255,0,0);
                      }
                      else if(cc==3)
                      {

                      }
                }
                else
                {
                  value=qRgb(0,0,0);
                }
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        cc=0;
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
    else if(lclm==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                value1=localmax[i][j];
                if(value1!=0)value1=value1+25;
                if(value1>255)value1=255;
                value=qRgb(value1,value1,value1);
                opimg.setPixel(QPoint(i,j),value);
            }
        }
        //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
        emit sendPixmap(opimg);
    }
}

void MainWindow::on_dt8_clicked()
{
    int i,j,min;
    double v;
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(arr[i][j]!=0)dt[i][j]=INT_MAX;
            else
                dt[i][j]=0;
        }
    }
    do
    {
        flag1=0;
        for(j=0;j<r;j++)//Forward Pass
        {
            for(i=0;i<c;i++)
            {
                min=dt[i][j];
                if((i-1>=0)&&((dt[i-1][j]+d1)<min))
                {
                    min=dt[i-1][j]+d1;
                    flag1++;
                }
                if((i-1>=0)&&(j-1>=0)&&((dt[i-1][j-1]+d2)<min))
                {
                    min=dt[i-1][j-1]+d2;
                    flag1++;
                }
                if((j-1>=0)&&((dt[i][j-1]+d1)<min))
                {
                    min=dt[i][j-1]+d1;
                    flag1++;
                }
                if((i+1<c)&&(j-1>=0)&&((dt[i+1][j-1]+d2)<min))
                {
                    min=dt[i+1][j-1]+d2;
                    flag1++;
                }
                dt[i][j]=min;
                //if(min>maxintendt)maxintendt=min;
            }
        }
        for(j=r-1;j>=0;j--)//Backward Pass
        {
            for(i=c-1;i>=0;i--)
            {
                min=dt[i][j];
                if((i+1<c)&&(j+1<r)&&((dt[i+1][j+1]+d2)<min))
                {
                    min=dt[i+1][j+1]+d2;
                    flag1++;
                }
                if((i+1<c)&&((dt[i+1][j]+d1)<min))
                {
                    min=dt[i+1][j]+d1;
                    flag1++;
                }
                if((j+1<r)&&((dt[i][j+1]+d1)<min))
                {
                    min=dt[i][j+1]+d1;
                    flag1++;
                }
                if((i-1>=0)&&(j+1<r)&&((dt[i-1][j+1]+d2)<min))
                {
                    min=dt[i-1][j+1]+d2;
                    flag1++;
                }
                dt[i][j]=min;
                if(min>maxintendt)maxintendt=min;
            }
        }
    }while(flag1);
    //ui->heightvalue->setNum(maxintendt);
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            v=round((dt[i][j]/(double)maxintendt)*255);
            dt[i][j]=v;
        }
    }
    hgdtimg8=true;
    ldt=true;lfdt=false;
    inputimg=false;dtimg8=true;cclimg4=false;cclimg8=false;fdtimg8=false;bina=false;hg=false;lclm=false;
    ui->colormap->setEnabled(true);
    drawimage();
}

void MainWindow::on_histogramequilization_clicked()
{
    ui->colormap->setEnabled(false);
    if(hgdtimg8==true)
    {
     int intensity[256],index,i,j,max=-1;
     double normal[256];
     for(i=0;i<256;i++)intensity[i]=0;

     for(j=0;j<r;j++)
     {
        for(i=0;i<c;i++)
        {
            index=dt[i][j];
            intensity[index]=intensity[index]+1;
            if(index>max)max=index;
        }
     }
    //ui->heightvalue->setNum(max);
    for(i=1;i<256;i++)//cummulative sum
    {
        intensity[i]=intensity[i]+intensity[i-1];
        normal[i]=(double)(intensity[i]/(double)(r*c))*255;
    }
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            index=dt[i][j];
            hist[i][j]=normal[index];
        }
    }
    }
    else if(fdtimg8==true)
    {

    }
    inputimg=false;dtimg8=false;cclimg4=false;cclimg8=false;fdtimg8=false;bina=false;hg=true;lclm=false;
    drawimage();
}

void MainWindow::on_ccl4_clicked()
{
    ui->colormap->setEnabled(false);
    int i,j,curlabel=2,min,max,labeltrack[1000];
    struct node *curnode,*temp,*ar[1000];
    for(i=0;i<1000;i++)
    {
        ar[i]=(struct node *)malloc(sizeof(struct node));
        ar[i]->data=i;
        ar[i]->next=NULL;
        labeltrack[i]=0;
    }

   for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(arr[i][j]!=0)
                ccl[i][j]=1;
            else
                ccl[i][j]=0;
        }
    }

    //first pass
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(ccl[i][j]!=0)
            {
                if((i-1>=0&&j-1>=0&&(ccl[i-1][j]<=1)&&(ccl[i][j-1]<=1))||(i-1<0&&j-1<0))
                {
                    ccl[i][j]=curlabel;
                    curlabel=curlabel+1;
                }
                else if(i-1<0&&j-1>=0&&ccl[i][j-1]<=1)
                {
                    ccl[i][j]=curlabel;
                    curlabel=curlabel+1;
                }
                else if(j-1<0&&i-1>=0&&ccl[i-1][j]<=1)
                {
                    ccl[i][j]=curlabel;
                    curlabel=curlabel+1;
                }
                else if(i-1<0&&j-1>=0&&ccl[i][j-1]>1)
                {
                    ccl[i][j]=ccl[i][j-1];
                }
                else if(j-1<0&&i-1>=0&&ccl[i-1][j]>1)
                {
                    ccl[i][j]=ccl[i-1][j];
                }
                else if(i-1>=0&&j-1>=0&&(ccl[i-1][j]>1)&&(ccl[i][j-1]<=1))
                {
                    ccl[i][j]=ccl[i-1][j];
                }
                else if(i-1>=0&&j-1>=0&&(ccl[i-1][j]<=1)&&(ccl[i][j-1]>1))
                {
                    ccl[i][j]=ccl[i][j-1];
                }
                else if(i-1>=0&&j-1>=0&&(ccl[i][j-1]>1)&&(ccl[i-1][j]>1))
                {
                    min=ccl[i-1][j];
                    max=ccl[i][j-1];
                    if(ccl[i][j-1]<min)
                    {
                        min=ccl[i][j-1];
                        max=ccl[i-1][j];
                    }
                    ccl[i][j]=min;
                    curnode=(struct node *)malloc(sizeof(struct node));
                    //temp=(struct node *)malloc(sizeof(struct node));
                    curnode->data=max;
                    curnode->next=NULL;
                    temp=ar[min];
                    if(temp->next==NULL)
                    {
                        temp->next=curnode;
                    }
                    else
                    {
                        curnode->next=temp->next;
                        temp->next=curnode;
                    }
                    curnode=NULL;
                    temp=NULL;
                    free(temp);
                    free(curnode);
                }
            }
        }
    }
    //secondpass
    for(i=2;i<1000;i++)
    {
        temp=ar[i];
        while(temp->next!=NULL)
        {
            temp=temp->next;
            ar[temp->data]->data=ar[i]->data;
        }
    }

    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            ccl[i][j]=ar[ccl[i][j]]->data;
            labeltrack[ccl[i][j]]=labeltrack[ccl[i][j]]+1;
        }
    }
    for(i=2;i<1000;i++)
    {
        if(labeltrack[i]!=0)cc++;
    }
    ui->nocc->setNum(cc);
    inputimg=false;dtimg8=false;cclimg4=true;cclimg8=false;fdtimg8=false;bina=false;hg=false;lclm=false;
    drawimage();
}

void MainWindow::on_colormap_clicked()
{
    int v1=0,v2=0,v3=255,x,i,j;
    double lbl;
    if(dtimg8==true)
    {
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(dt[i][j]>maxintendt)maxintendt=dt[i][j];
        }
    }
    //ui->heightvalue->setNum(maxintendt);
    lbl=round((double)(255/(double)(maxintendt/2)));
    int level=lbl;
    QRgb colmap[maxintendt];
    colmap[0]=qRgb(0,0,0);
    colmap[1]=qRgb(v1,v2,v3);
    for(i=2;i<=maxintendt;i++)
    {
        if(v3>0&&v2<255)
        {
            v3=v3-((i-1)*level);
            if(v3<0)v3=0;
            v2=v2+((i-1)*level);
            if(v2>255)v2=255;
            colmap[i]=qRgb(v1,v2,v3);
        }
        //printf("%d ",i);
        else if(v3==0&&v2>0&&v1<255)
        {
            v2=v2-((i-1)*level);
            if(v2<0)v2=0;
            v1=v1+((i-1)*level);
            if(v1>255)v1=255;
            colmap[i]=qRgb(v1,v2,v3);
        }

    }
    QImage opimg(c,r,QImage::Format_RGB32);
    QRgb value;
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            x=dt[i][j];
            value=colmap[x];
            opimg.setPixel(QPoint(i,j),value);
        }
    }
    //ui->opimage->setPixmap(QPixmap::fromImage(opimg));
    emit sendPixmap(opimg);
    }
}

void MainWindow::on_FDT_clicked()
{
    ui->colormap->setEnabled(false);
    int i,j,mini,maxi=0,iter=0;
    double v;
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            v=arr[i][j]/(double)ipmaxinten;
            fuzzy[i][j]=v;
        }
    }
    for(j=0;j<r;j++)//prefdt
    {
        for(i=0;i<c;i++)
        {
            if(arr[i][j]!=0)
            {
                FDT[i][j]=INT_MAX;
            }
            else
            {
                FDT[i][j]=0;
            }
        }
    }
    do
    {
        iter++;
        flag1=0;
        for(j=0;j<r;j++)//Forward Pass
        {
            for(i=0;i<c;i++)
            {
                mini=FDT[i][j];
                if((i-1>=0)&&((FDT[i-1][j]+(fuzzy[i-1][j]+fuzzy[i][j])*2)<mini))
                {
                    mini=FDT[i-1][j]+(fuzzy[i-1][j]+fuzzy[i][j])*2;
                    flag1++;
                }
                if((i-1>=0)&&(j-1>=0)&&((FDT[i-1][j-1]+(fuzzy[i-1][j-1]+fuzzy[i][j])*3)<mini))
                {
                    mini=FDT[i-1][j-1]+(fuzzy[i-1][j-1]+fuzzy[i][j])*3;
                    flag1++;
                }
                if((j-1>=0)&&((FDT[i][j-1]+(fuzzy[i][j-1]+fuzzy[i][j])*2)<mini))
                {
                    mini=FDT[i][j-1]+(fuzzy[i][j-1]+fuzzy[i][j])*2;
                    flag1++;
                }
                if((i+1<c)&&(j-1>=0)&&((FDT[i+1][j-1]+(fuzzy[i+1][j-1]+fuzzy[i][j])*3)<mini))
                {
                    mini=FDT[i+1][j-1]+(fuzzy[i+1][j-1]+fuzzy[i][j])*3;
                    flag1++;
                }
                FDT[i][j]=mini;
                if(mini>maxi)maxi=mini;
            }
        }
        for(j=r-1;j>=0;j--)//Backward Pass
        {
            for(i=c-1;i>=0;i--)
            {
                mini=FDT[i][j];;
                if((i+1<c)&&(j+1<r)&&((FDT[i+1][j+1]+(fuzzy[i+1][j+1]+fuzzy[i][j])*3)<mini))
                {
                    mini=FDT[i+1][j+1]+(fuzzy[i+1][j+1]+fuzzy[i][j])*3;
                    flag1++;
                }
                if((i+1<c)&&((FDT[i+1][j]+(fuzzy[i+1][j]+fuzzy[i][j])*2)<mini))
                {
                    mini=FDT[i+1][j]+(fuzzy[i+1][j]+fuzzy[i][j])*2;
                    flag1++;
                }
                if((j+1<r)&&((FDT[i][j+1]+(fuzzy[i][j+1]+fuzzy[i][j])*3)<mini))
                {
                    mini=FDT[i][j+1]+(fuzzy[i][j+1]+fuzzy[i][j])*2;
                    flag1++;
                }
                if((i-1>=0)&&(j+1<r)&&((FDT[i-1][j+1]+(fuzzy[i-1][j+1]+fuzzy[i][j])*3)<mini))
                {
                    mini=FDT[i-1][j+1]+(fuzzy[i-1][j+1]+fuzzy[i][j])*3;
                    flag1++;
                }
                FDT[i][j]=mini;
                if(mini>maxi)maxi=mini;
            }
        }
    }while(flag1);
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            v=round((FDT[i][j]/(double)maxi)*255);
            FDT[i][j]=v;
        }
    }
    lfdt=true;ldt=false;
    inputimg=false;dtimg8=false;cclimg4=false;cclimg8=false;fdtimg8=true;bina=false;hg=false;lclm=false;
    drawimage();
}

void MainWindow::on_localmaxima_clicked()
{
    ui->colormap->setEnabled(false);
    int lmax,i,j,p,q,ws=2,flg;
    if(ldt==true)
    {
    for(j=0;j<r;j++)
    {
        for(i=0;i<c;i++)
        {
            if(dt[i][j]!=0)
            {
            lmax=dt[i][j];
            flg=0;
            for(q=j-ws;q<=j+ws;q++)
            {
                for(p=i-ws;p<=i+ws;p++)
                {
                    if(p>=0&&p<c&&q>=0&&q<r)
                        if(dt[p][q]>lmax)
                        {
                            lmax=dt[p][q];
                            flg++;
                        }
                }
            }
            if(flg==0)
            {
                localmax[i][j]=lmax;
            }
            else
                localmax[i][j]=0;
            }
        }
    }
    }
    else if(lfdt==true)
    {
        for(j=0;j<r;j++)
        {
            for(i=0;i<c;i++)
            {
                if(FDT[i][j]!=0)
                {
                lmax=FDT[i][j];
                flg=0;
                for(q=j-ws;q<=j+ws;q++)
                {
                    for(p=i-ws;p<=i+ws;p++)
                    {
                        if(p>=0&&p<c&&q>=0&&q<r)
                            if(FDT[p][q]>lmax)
                            {
                                lmax=FDT[p][q];
                                flg++;
                            }
                    }
                }
                if(flg==0)
                {
                    localmax[i][j]=lmax;
                }
                else
                    localmax[i][j]=0;
                }
            }
        }
    }
    lclm=true;inputimg=false;dtimg8=false;cclimg4=false;cclimg8=false;fdtimg8=false;bina=false;hg=false;
    drawimage();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    threshold = arg1;
}
