#ifndef MATDISPLAY_H
#define MATDISPLAY_H

#include <QLabel>
#include<QObject>
#include<QWidget>

#include<QMouseEvent>
#include<QEvent>

#include<QtGui>
#include<QtCore>

class matDisplay : public QLabel
{
    Q_OBJECT
private:
    QPoint point;
    QImage image;
    QString fname;
public:
    explicit matDisplay(QWidget *parent = 0);
protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *);
signals:
    void sendMousePosition(QPoint&);
public slots:
    void getImgFileName(QString fileName);
};

#endif // MATDISPLAY_H
