#include "procesarimagen.h"

ProcesarImagen::ProcesarImagen(Mat src, Mat imgBin, Mat imgSeg)
{
    imgOrigen = src.clone();
    imgBinaria = imgBin.clone();
    imgSegmentada = imgSeg.clone();
}

ProcesarImagen::~ProcesarImagen()
{

}

void ProcesarImagen::canalAzul(Mat &src, Mat &blue)
{
    Mat channel[3];
    Mat origen = src.clone();
    //
    //dividimos a la imagen en canales de color
    split(origen,channel);
    //obtenemos en canal azul
    blue=channel[0];
}

void ProcesarImagen::metodoOtsu(Mat &blue, Mat &dst)
{
    //procesar la imagen pasar a limites de 0 a 255
    threshold(blue,dst,0,255,THRESH_BINARY|THRESH_OTSU);
    //invertir los colores
    dst = ~dst;
}

void ProcesarImagen::fillHoles(Mat &dst)
{
    //se asume que la entrada es imagen Blanco y Negro(0 or 255)
    Mat holes=dst.clone();
    floodFill(holes,cv::Point2i(0,0),cv::Scalar(255));
    for(int i=0;i<dst.rows*dst.cols;i++)
    {
        if(holes.data[i]==0)
            dst.data[i]=255;
    }
}

void ProcesarImagen::removeSmallBlobs(Mat &dst, int size)
{
    // Only accept CV_8UC1
    if (dst.channels() != 1 || dst.type() != CV_8U)
        return;

    // Find all contours
    vector<vector<Point> > contours;
    findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++)
    {
        // Calculamos el area del contorno
        double area = contourArea(contours[i]);

        // removemos los pequeños objetos y los llenams con color negro
        if (area > 0 && area <= size)
            drawContours(dst, contours, i, CV_RGB(0,0,0), -1);
    }
}

void ProcesarImagen::segmentacion(Mat &dst, Mat &src)
{
    Mat imgSeg;
    //Aplicamos Filtro de la mediana para la imagen (suavizado)
    medianBlur(dst, imgSeg, 15);
    dst = imgSeg.clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345); //colour

    // Encontrar contornos
    findContours( imgSeg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // Dibujar Contornos
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), 0, rng.uniform(0,255) );
        drawContours( src, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
}



