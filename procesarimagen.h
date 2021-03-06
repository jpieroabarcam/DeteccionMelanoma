#ifndef PROCESARIMAGEN_H
#define PROCESARIMAGEN_H

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

class ProcesarImagen
{
private:
    Mat imgOrigen;
    Mat imgBinaria;
    Mat imgSegmentada;
public:
    ProcesarImagen(Mat,Mat,Mat);
    virtual ~ProcesarImagen();
    void canalAzul(Mat&,Mat&);
    void metodoOtsu(Mat&,Mat&);
    void fillHoles(Mat&);
    void removeSmallBlobs(Mat&,int);
    void segmentacion(Mat&,Mat&);
};

#endif // PROCESARIMAGEN_H
