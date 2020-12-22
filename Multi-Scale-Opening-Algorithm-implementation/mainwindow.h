#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>






namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

//public slots:
    void My_Main();

protected:
    void        mouseMoveEvent( QMouseEvent * );
    void        mousePressEvent( QMouseEvent * );
    void        wheelEvent(QWheelEvent *);
private:
    Ui::MainWindow *ui;
    QByteArray filename;
    unsigned short curX;
    unsigned short curY;
    unsigned short curZ;
    unsigned short Xdim;
    unsigned short Ydim;
    unsigned short Zdim;
    float sp[3];
    unsigned long hist[6000];
    unsigned long hmax;
    float scale;
    float maxFDTb;
    float maxFDTv;
    unsigned short Flag;
    unsigned short GITN;
    char DataID[5];
    short offset;
    //short int *vol;


  typedef signed short        InputPixelType;
  typedef unsigned char        OutputPixelType;
  void load_seeds();
  void load_GT();
  void draw_image();
  void allocate_memory();
  void allocate_memorycore();
  void histogram();
  int bright_cont(int);
  void compute_FDT(unsigned short***);
  void pos_FDT(int,int,int);
  void neg_FDT(int,int,int);
  float get_mu(int,int,int);
  void local_scale();
  void local_maxima();
  void compute_MR( short***);
  void VTKdisplay();
  void core_vessel();
  void erosion(unsigned short***,int);
  void dialation(unsigned short***,int);
  void load_coreVessel();
  void load_coreVessel_file();
 // int get_orange(int);

  void de_allocate_memory();
private slots:
       void on_pushButton_EvPh_clicked();
       void on_verticalSlider_valueChanged(int value);
       void on_pushButton_EstAgr_clicked();
       void on_pushButton_LU2_clicked();
       void on_pushButton_LU1_clicked();
       void on_pushButton_18_clicked();
       void on_pushButton_16_clicked();
       void on_checkBox_11_clicked();
       void on_pushButton_15_clicked();
       void on_pushButton_14_clicked();
       void on_pushButton_13_clicked();
       void on_pushButton_12_clicked();
       void on_pushButton_11_clicked();
       void on_pushButton_10_clicked();
       void on_pushButton_20_clicked();
       void on_pushButton_21_clicked();
       void on_pushButton_19_clicked();
       void on_checkBox_13_clicked();
       void on_checkBox_9_clicked();
       void on_pushButton_9_clicked();
       void on_checkBox_8_clicked();
       void on_checkBox_6_clicked();
       void on_checkBox_5_clicked();
       void on_pushButton_8_clicked();
       void on_pushButton_7_clicked();
       void on_pushButton_6_clicked();
       void on_pushButton_5_clicked();
       void on_checkBox_4_clicked();
       void on_pushButton_4_clicked();
       void on_checkBox_3_clicked();
       void on_checkBox_2_clicked();
       void on_pushButton_test_clicked();
       void on_horizontalSlider_c_valueChanged(int value);
       void on_horizontalSlider_b_valueChanged(int value);
       void on_checkBox_clicked(bool checked);
       void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_spinBox_z_valueChanged(int );
    void on_spinBox_y_valueChanged(int );
    void on_spinBox_x_valueChanged(int );
    void on_pushButton_2_clicked();

};



#endif // MAINWINDOW_H

