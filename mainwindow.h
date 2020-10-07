#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/tracking.hpp>
#include "MatToQImage.h"
#include <QMouseEvent>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>

using namespace cv;
using namespace std;

// Define the size of QLabel
#define scale  1.3
#define QLHeight scale*240
#define QLWidth  scale*320

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    VideoCapture cap;
    Mat capture, smooth, edge, hline, hline2, BackGround, image,gray, prevGray, img, cnt_img, gray2, edge2, edge3, smooth2, capture2;
    bool BGFlag=false;
    const int MAX_COUNT = 500;
    bool needToInit = true;
    bool nightMode = false;
    std::vector<Point2f> points[2];
    Point2f point;
    bool addRemovePt = false;
    vector<vector<Point> > contours;
    vector<vector<Point> > contours0;
    vector<Vec4i> hierarchy;
    const int w = 500;
    int levels = 4;
    int level=1;
    int edgelevel=1;
    int thresholdvalue=1;
    int red=255;
    int green=255;
    int blue=128;

private slots:
    void update();
    void on_BackGroundButtom_toggled(bool checked);
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_Nightmode_clicked();
    void on_spinBox_valueChanged(int arg1);
    void mousePressEvent(QMouseEvent *event);
    void on_Nightmode_2_clicked(bool checked);
    void on_horizontalScrollBar_valueChanged(int value);
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
};

#endif // MAINWINDOW_H
