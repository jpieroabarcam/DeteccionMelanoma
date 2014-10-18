#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T11:56:44
#
#-------------------------------------------------

QT       += core gui
QT += widgets

TARGET = DeteccionMelanoma
TEMPLATE = app


SOURCES += main.cpp \
    GUI.cpp \
    procesarimagen.cpp \
    caracteristicas.cpp \
    agpoblacion.cpp \
    agcromosoma.cpp \
    agalgoritmogenetico.cpp \
    backpropagation/capa.cpp \
    backpropagation/neurona.cpp \
    backpropagation/redneuronal.cpp \

HEADERS  += \
    GUI.h \
    procesarimagen.h \
    caracteristicas.h \
    agpoblacion.h \
    agcromosoma.h \
    agalgoritmogenetico.h \
    backpropagation/capa.h \
    backpropagation/neurona.h \
    backpropagation/redneuronal.h

FORMS    += \
    GUI.ui

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann
