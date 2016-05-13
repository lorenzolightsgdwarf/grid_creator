QT += core gui
CONFIG += c++11

TARGET = generate_tags_configuration
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv /usr/local/include
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_video -lopencv_photo -lopencv_aruco -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
TEMPLATE = app

SOURCES += main.cpp
