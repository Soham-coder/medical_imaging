#ifndef LABELDISP_H
#define LABELDISP_H

#include <QLabel>
#include<QObject>
#include<QWidget>

#include<QMouseEvent>
#include<QEvent>

#include<QtGui>
#include<QtCore>
class LabelDisp : public QLabel
{
    Q_OBJECT
private:
    QPoint point;
    QImage image;
    QString fname;
    QColor color;
    QPixmap *pixmap;
    QPixmap opimage;
    QRgb **pixMat;
    QRgb **opPixMat;
    bool pressed;
    void draw(QMouseEvent *e);
    void drawGrid(QMouseEvent *ev);
public:
    explicit LabelDisp(QWidget *parent = 0);
protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *ev);
signals:
    void sendMousePosition(QPoint&);
    void sendPixelInfo(QRgb&);
    void seedInfo(QPoint&,QRgb&);
public slots:
    void getImgFileName(QString fileName);
    void getPixmap(QImage &img);
};

#endif // LABELDISP_H
