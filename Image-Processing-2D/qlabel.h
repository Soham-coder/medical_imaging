#ifndef QLABEL_H
#define QLABEL_H

#include <QLabel>
#include<QObject>
#include<QWidget>

#include<QMouseEvent>
#include<QEvent>

#include<QtGui>
#include<QtCore>

class qlabel : public QLabel
{
    Q_OBJECT
private:
    QPoint point;
    QImage image;
    QString fname;
    QColor color;
    QPixmap *pixmap;
    bool pressed;
    void draw(QMouseEvent *e);
public:
    explicit qlabel(QWidget *parent = 0);
protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void sendMousePosition(QPoint&);
public slots:
    void getImgFileName(QString fileName);
};

#endif // QLABEL_H
