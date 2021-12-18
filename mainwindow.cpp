#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

#define CV_AA 16

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        ui->display1->setScaledContents(true);
        ui->display2->setScaledContents(true);
        ui->display3->setScaledContents(true);
        ui->display4->setScaledContents(true);
        ui->display5->setScaledContents(true);
        ui->display6->setScaledContents(true);

        ui->display1->setFixedSize( QLWidth, QLHeight );
        ui->display2->setFixedSize( QLWidth, QLHeight );
        ui->display3->setFixedSize( QLWidth, QLHeight );
        ui->display4->setFixedSize( QLWidth, QLHeight );
        ui->display5->setFixedSize( QLWidth, QLHeight );
        ui->display6->setFixedSize( QLWidth, QLHeight );

    cap.open(2);

    // Call update every 100ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::update(){
    //************************************************************************************* Capture image

    // Capture image from camera, return if fail to load the camera
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    if( !cap.isOpened() ) return;

    cap >> capture;
    cap >> capture2;

    if(!BGFlag)
    {
        capture2.copyTo(BackGround);
        BGFlag=true;
    }

    if (ui->BackGroundButtom->isChecked())
        absdiff(capture2, BackGround, capture2);

    //************************************************************************************** Preprocessor

    // Camera calibration, make the image undistort.

    //************************************************************************************** Smooth
    // Blur the image so that edges detected by Canny due to noises will reduced
    medianBlur(capture, smooth, 5);
    for( int i = 0; i < level; i++)
        medianBlur(smooth, smooth2, 5);


    // ****************************************Main Algorithm **************************************

    //************************************************************************************** Contour
    //contour detection
    capture.copyTo(img);

    cvtColor(img, gray2, COLOR_BGR2GRAY);
    Canny(gray2, edge2, 50, 100, 3);
    findContours( edge2, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    contours.resize(contours0.size());
    for( size_t k = 0; k < contours0.size(); k++ )
        approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

    Mat cnt_img = Mat::zeros(w, w, CV_8UC3);
    int _levels = levels - 3;
    drawContours( cnt_img, contours, _levels <= 0 ? 3 : -1, Scalar(blue,green,red),
                  3, LINE_AA, hierarchy, std::abs(_levels) );

    //Scalar(128,255,255)=yellow
    //Scalar(0,0,255)=red
    //Scalar(0,255,0)=green
    //Scalar(255,0,0)=blue

    //************************************************************************************** Sharp Edges
    //lk detection
    capture.copyTo(image);
    cvtColor(image, gray, COLOR_BGR2GRAY);

    if( nightMode )
    image = Scalar::all(0);

    if( needToInit)
    {
    // automatic initialization
    goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
    cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
    addRemovePt = false;
    }

    else if( !points[0].empty() )
    {
    vector<uchar> status;
    vector<float> err;
    if(prevGray.empty())
    gray.copyTo(prevGray);
    calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,3, termcrit, 0, 0.001);
    size_t i, k;

              for( i = k = 0; i < points[1].size(); i++ )
              {
                 if( addRemovePt )
                 {
                     if( norm(point - points[1][i]) <= 5 )
                     {
                     addRemovePt = false;
                     continue;
                     }
                 }

                 if( !status[i] )
                 continue;

                 points[1][k++] = points[1][i];
                 circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
               }

                 points[1].resize(k);
     }

            if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
            {
                vector<Point2f> tmp;
                tmp.push_back(point);
                cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
                points[1].push_back(tmp[0]);
                addRemovePt = false;
            }

            needToInit = false;

            std::swap(points[1], points[0]);
            cv::swap(prevGray, gray);

    //************************************************************************************** Edge
    // Edge detection
    Canny(smooth, edge3, edgelevel, edgelevel, 3);

    //************************************************************************************** Line
    // Hough Line
    std::vector<Vec4i> lines;
    Canny(smooth, edge, 50, 100, 3);
    HoughLinesP(edge, lines, 1, CV_PI/180, thresholdvalue, 50, 10 );

    // Draw Hough Line
    capture.copyTo( hline );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( hline, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
    }

    // Search red line from all detected line
    std::vector<Vec4i> redLine;

    // Draw Red Lines
    capture.copyTo(hline2);
    for( size_t i = 0; i < redLine.size(); i++ ){
        Vec4i l = redLine[i];
        line( hline2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 2, CV_AA);
    }

    // Convert cv::Mat to QImage
    QImage img1 = MatToQImage( capture2 );
    QImage img2 = MatToQImage( image  );
    QImage img3 = MatToQImage( smooth2    );
    QImage img4 = MatToQImage( edge3   );
    QImage img5 = MatToQImage( hline );
    QImage img6 = MatToQImage( cnt_img );

    // Display QImage
    ui->display1->setPixmap(QPixmap::fromImage(img1));
    ui->display2->setPixmap(QPixmap::fromImage(img2));
    ui->display3->setPixmap(QPixmap::fromImage(img3));
    ui->display4->setPixmap(QPixmap::fromImage(img4));
    ui->display5->setPixmap(QPixmap::fromImage(img5));
    ui->display6->setPixmap(QPixmap::fromImage(img6));


}

void MainWindow::on_BackGroundButtom_toggled(bool checked)
{
    BGFlag=checked;
}

void MainWindow::on_pushButton_clicked()
{
    needToInit=true;
}

void MainWindow::on_Nightmode_clicked()
{
    nightMode=true;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    level=value;
    qDebug()<< "slider";
    QString level_string = QString::number(level);
    ui->textBrowser_4->setText(level_string);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
     edgelevel=arg1;
     qDebug()<< "spinbox";
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "x" << event->x();
    int x=event->x();
    QString xs = QString::number(x);
    ui->textBrowser_2->setText(xs);


    qDebug() << "y" << event->y();
    int y=event->y();
    QString ys = QString::number(y);
    ui->textBrowser_3->setText(ys);

    Vec3b intensity=img.at<Vec3b>(x,y);
    int blue_=intensity.val[0];
    int green_=intensity.val[1];
    int red_=intensity.val[2];

    QString bs = QString::number(blue_);
    QString gs = QString::number(green_);
    QString rs = QString::number(red_);

    ui->textBrowser_6->setText(rs);
    ui->textBrowser_7->setText(gs);
    ui->textBrowser_8->setText(bs);

//    qDebug() << blue_;
//    qDebug() << green_;
//    qDebug() << red_;



}

void MainWindow::on_Nightmode_2_clicked(bool checked)
{
    nightMode=false;
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    thresholdvalue=value;
    qDebug()<< "threshold";
    QString threshold_string = QString::number(thresholdvalue);
    ui->textBrowser_5->setText(threshold_string);
}

void MainWindow::on_checkBox_clicked()
{
    blue=0;
    green=0;
    red=255;
}

void MainWindow::on_checkBox_2_clicked()
{
    blue=0;
    green=255;
    red=0;
}

void MainWindow::on_checkBox_3_clicked()
{
    blue=255;
    green=0;
    red=0;
}

void MainWindow::on_checkBox_4_clicked()
{
    blue=128;
    green=255;
    red=255;
}
