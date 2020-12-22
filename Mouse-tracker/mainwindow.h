#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
private slots:
    void on_btnLoadImage_clicked();
    void on_btnLoadImage2_clicked();

    void on_btnRefresh1_clicked();

    void on_btnRefresh2_clicked();

signals:
    void loadImage(QString fileName);
    void loadImage2(QString fileName);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
