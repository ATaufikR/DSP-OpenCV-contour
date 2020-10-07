#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T11:26:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DSP_openCV_Demo_Rosa
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    MatToQImage.cpp

HEADERS  += mainwindow.h \

    MatToQImage.h

FORMS    += mainwindow.ui

#linux openCV package
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

#Linux opencv link
# OpenCv Configuration opencv-3.3.0
#INCLUDEPATH += "~/opencv-3.3.0/opencv/include"
#INCLUDEPATH += "~/opencv-3.3.0/opencv/include/opencv"

#LIBS += -L~/opencv-3.3.0/build/bin \
#-lopencv_core \
#-lopencv_highgui \
#-lopencv_imgproc \
#-lopencv_features2d \
#-lopencv_calib3d \
#-lopencv_video\
#-lopencv_videoio

# OpenCv Configuration ROS opencv-3.3.1

#INCLUDEPATH += "/opt/ros/kinetic/include/opencv-3.2.0-dev"
#LIBS += -L/opt/ros/kinetic/lib -lopencv_core3 \
#-lopencv_videoio3 \
#-lopencv_highgui3 \
#-lopencv_imgproc3 \
#-lopencv_features2d3 \
#-lopencv_calib3d3 \
#-lopencv_video3
