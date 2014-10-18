#ifndef CARACTERISTICAS_H
#define CARACTERISTICAS_H

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#define PI 3.14159265

using namespace std;
using namespace cv;

class Caracteristicas
{
private:
    double calcDistEuclidiana(int,int,int,int);
    Point calcCentroide(Mat &);
public:
    Caracteristicas();
    virtual ~Caracteristicas();
    double calcPerimetro(Mat&);
    double calcArea(Mat&);
    double indiceIrregular(double,double);
    float calcAngulo(Mat &);
    Mat rotarImagen(Mat&,float,int);
    vector<double> calcAsimetria(Mat&);
    vector<double> variacionColor(Mat&, Mat&);
    vector<double> cromRelativa(Mat&, Mat&);
    vector<double> cordEsferica(Mat&, Mat&);
    vector<double> transfLab(Mat&, Mat&);
};


#endif // CARACTERISTICAS_H
