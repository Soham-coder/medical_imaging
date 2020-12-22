#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define sizex 350
#define sizey 350
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void showMousePosition(QPoint &pos);
    void getPos1(QPoint &pos);
    void getPos2(QPoint &pos);
    void getPixelInfo(QRgb &pixVal);
    void getSeedInfo1(QPoint &pos,QRgb &val);
    void getSeedInfo2(QPoint &pos,QRgb &val);
signals:
    void loadImage(QString fileName);
    void sendPixmap(QImage &);
private slots:
    void on_loadimage_clicked();

    void on_btnRefresh1_clicked();

    void on_btnRefresh2_clicked();

    void on_binarize_clicked();
    void on_dt8_clicked();

    void on_histogramequilization_clicked();

    void on_ccl4_clicked();

    void on_colormap_clicked();

    void on_FDT_clicked();

    void on_localmaxima_clicked();

    void on_spinBox_valueChanged(int arg1);

protected:
    void drawimage();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
