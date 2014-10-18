#include "GUI.h"
#include "ui_GUI.h"
#import <QFileDialog>
#import <QImage>
#import <QString>

#include "procesarimagen.h"
#include "caracteristicas.h"


#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include "agalgoritmogenetico.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#define PI 3.14159265

using namespace std;
using namespace cv;

/// Variables Globales

Mat src;
Mat Original;
Mat imgBinary;
Mat imgSegment;

Mat dst;

RNG rng(12345); //colour


double perimetro, area, indexIrregular;

//char name[]="/home/piero/lenag.jpg";
char path[]="/home/piero/ImagenesProyecto/Dysplacic";


ifstream myfile ("/home/piero/caracteristicas.txt");
int cont = 0;

//Info de red
vector<int> info;


//para entrenamiento
vector<pair<vector<float>,vector<int> > > p;

AgCromosoma *pcro=NULL;
vector<float> er;
int op;

RedNeuronal *red;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    info.push_back(15);
    info.push_back(30);
    info.push_back(1);

    //Red Neuronal
    red=new RedNeuronal(info);
    red->pesosAleatorios();

    //Algoritmo Genetico
    AgAlgoritmoGenetico ag(100,info,red);

    string line;
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {

          //Patron
          vector<float> in1;
          vector<int> sa1;
          pair<vector<float>,vector<int> > p1;

              cout << line <<'\n';
              stringstream ss(line);
              float oneValue;
              while (ss >> oneValue) {
                  in1.push_back(oneValue);
              }

          getline (myfile,line);
              cout << line <<'\n';
              stringstream ss2(line);
              int snValue;
              ss2 >> snValue;
              sa1.push_back(snValue);

          p1.first=in1;p1.second=sa1;
          p.push_back(p1);
          ag.agregarPatron(p1);
          //cont++;
      }
      myfile.close();
      cout<<p.size()<<endl;
      //cout<<cont<<endl;
    }
    else {
        cout << "Unable to open file";
    }

    op = 500;
    for(int i=0;i<op;i++)
        er=red->entrenar(p);
    cout<<er[0]<<"  "<<er[1]<<endl;

    op = 20;
    for(int i=0;i<op;i++){
        pcro=ag.nextGeneracion();}
    cout<<pcro->aptitud<<endl;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ImagenButton_clicked()
{
    /*
    connect(ui->pushButton,SIGNAL(clicked(),
            ui->pushButton,SLOT(close()));
            */
    QString imagefilename = QFileDialog::getOpenFileName( this,"imagen.jpg",  path,"Images (*.bmp *.png *.xpm *.jpg *.tif)");    

    cout<<imagefilename.toStdString()<<endl;
    if(imagefilename == NULL) return;
    /// cargar una imagen
    src = imread( imagefilename.toStdString());
    Original = src.clone();
    Mat azul;

    ProcesarImagen proc(Original,imgBinary,imgSegment);

    proc.canalAzul(src,azul);
    proc.metodoOtsu(azul,dst);
    proc.fillHoles(dst);
    proc.removeSmallBlobs(dst,1000);
    proc.segmentacion(dst,src);

    imgBinary = dst.clone();
    imgSegment = dst.clone();

    //ProcesarImagen procesados(Original,imgBinary,src);
//    imshow("Imagen Original",Original);
//    imshow("Imagen Binaria",imgBinary);
    imwrite( "orig.jpg", Original );
    imwrite( "bin.jpg", imgBinary );
    imwrite( "seg.jpg", src);


    QString ori = "orig.jpg";
    QImage   QImagen1;
    QImagen1.load (ori);
    QImagen1 = QImagen1.scaled(350,200);
    ui->imagen1->setPixmap(QPixmap::fromImage(QImagen1));

    QString bin = "bin.jpg";
    QImage   QImagen2;
    QImagen2.load (bin);
    QImagen2 = QImagen2.scaled(350,200);
    ui->imagen2->setPixmap(QPixmap::fromImage(QImagen2));

    QString seg = "seg.jpg";
    QImage   QImagen3;
    QImagen3.load (seg);
    QImagen3 = QImagen3.scaled(350,200);
    ui->imagen3->setPixmap(QPixmap::fromImage(QImagen3));
}

void MainWindow::on_btnProcesar_clicked()
{
    /// Calculo de la primera caracteristica de la imagen Indice irregular
    Caracteristicas car;
    perimetro = car.calcPerimetro(imgBinary);
    area = car.calcArea(imgBinary);
    indexIrregular = car.indiceIrregular(area,perimetro);
    cout<<"area: "<<area<<" per: "<<perimetro<<endl;
    cout<<indexIrregular<<" ***\n "<<"***\n";

    /// Calculo de la segunda caracteristica (Assimetria)
    float imgWorkedProps;
    Mat destino;
    imgWorkedProps = car.calcAngulo(imgBinary);
    destino = car.rotarImagen(imgBinary,imgWorkedProps,NULL);
    vector<double> assimetry = car.calcAsimetria(destino);
            //= calcAssymmetry(destino);

    /// Calculo de la 3ra,4ta y 5ta caracteristica (variacion de color)
    vector<double> varianceColor =car.variacionColor(imgBinary,Original);
            //= getVarianceColor(imgBinary,Original);

    /// Calculo de la 6ta,7ma y 8va caracteristica (cromacion relativa)
    vector<double> relative = car.cromRelativa(Original,imgBinary);

    /// Calculo de la 9na,10ma y 11va caracteristica (cord esfericas)
    vector<double> spherical = car.cordEsferica(imgBinary,Original);

    /// Calculo de la 12va, 13va y 14va caracteristica (transformacion L* a* b*)
    vector<double> lab = car.transfLab(imgBinary,Original);

    ofstream mytest;
      mytest.open ("test.txt");
      mytest << indexIrregular << " " << assimetry[0]<< " " << assimetry[1]<< " " <<

                varianceColor[0]<<" "<<varianceColor[1]<<" "<<varianceColor[2]<<" "<<
                relative[0] << " " <<relative[1] << " " << relative[2] << " " <<
                spherical[0] <<" " << spherical[1] << " " << spherical[2]<<" "<<
                lab[0] <<" "<< lab[1]<<" "<<lab[2]<<"\n";
      //myfile << 1 <<"\n";
      mytest.close();

      ostringstream strs;
      strs << indexIrregular;
      string str = strs.str();
      ui->res1->setText(QString::fromStdString(str));

      ostringstream s1;
      s1 << assimetry[0];
      string str2 = s1.str();
      ostringstream s2;
      s2 << assimetry[1];
      str2 = str2 + " " + s2.str();
      ui->res2->setText(QString::fromStdString(str2));

      ostringstream va1;
      va1 << varianceColor[0];
      string str3 = va1.str();
      ostringstream va2;
      va2 << varianceColor[1];
      str3 = str3 + " " + va2.str();
      ostringstream va3;
      va3 << varianceColor[2];
      str3 = str3 + " " + va3.str();
      ui->res3->setText(QString::fromStdString(str3));

      ostringstream re1;
      re1 << relative[0];
      string str4 = re1.str();
      ostringstream re2;
      re2 << relative[1];
      str4 = str4 + " " + re2.str();
      ostringstream re3;
      va3 << relative[2];
      str4= str4 + " " + re3.str();
      ui->res4->setText(QString::fromStdString(str4));

      ostringstream sp1;
      sp1 << spherical[0];
      string str5 = sp1.str();
      ostringstream sp2;
      sp2 << spherical[1];
      str5 = str5 + " " + sp2.str();
      ostringstream sp3;
      sp3 << spherical[2];
      str5= str5 + " " + sp3.str();
      ui->res5->setText(QString::fromStdString(str5));

      ostringstream la1;
      la1 << lab[0];
      string str6 = la1.str();
      ostringstream la2;
      la2 << lab[1];
      str6 = str6 + " " + la2.str();
      ostringstream la3;
      la3 << lab[2];
      str6= str6 + " " + la3.str();
      ui->res6->setText(QString::fromStdString(str6));

      string line;
      ifstream mytest2 ("test.txt");
      vector<float> test1;
      if (mytest2.is_open())
      {
          getline (mytest2,line);
          stringstream ss(line);
          float oneValue;
          while (ss >> oneValue) {
              test1.push_back(oneValue);
          }
        mytest2.close();
      }
      else {
          cout << "Unable to open file";
      }
      er=red->evaluar(test1);cout<<er[0]<<"  "<<er[1]<<endl;

      ostringstream er1;
      er1 << er[1];
      string err = er1.str();
      ui->res7->setText(QString::fromStdString(err));

      if (er[0]>0.5)
      {
          cout<<"Melanoma Maligno"<<endl;
          QString sad = "sad.jpg";
          QImage   triste;
          triste.load (sad);
          triste = triste.scaled(100,60);
          ui->diag->setPixmap(QPixmap::fromImage(triste));
      }
      else
      {
          cout<<"Marca Cutanea"<<endl;
          QString happy = "happy.jpg";
          QImage   feliz;
          feliz.load (happy);
          feliz = feliz.scaled(100,60);
          ui->diag->setPixmap(QPixmap::fromImage(feliz));
      }

      //ui->label->setText("Mi primer interfaz");

    /// Crear una matriz del mismo tamaño y tipo como el primero(para dst)
//    dst.create( src.size(), src.type() );

    /// Convertir la imagen a escala de grises
//    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// Crear una ventana
//    namedWindow( window_name, CV_WINDOW_AUTOSIZE );

    /// Mostrar la imagen
    do{
        imshow("Imagen Segmentada",src);
    }while(waitKey(30)<0);
     destroyAllWindows();

/// esperar hasta que el usuario presione una tecla
   //waitKey(0);
    cout<<"imagen " <<endl;
}
