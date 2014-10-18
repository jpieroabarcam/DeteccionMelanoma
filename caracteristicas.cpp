#include "caracteristicas.h"

Caracteristicas::Caracteristicas()
{
}
Caracteristicas::~Caracteristicas()
{
    //dtr
}

Point Caracteristicas::calcCentroide(Mat &mask)
{
    Mat prueba = mask.clone();
    RNG rng(12345); //colour
    cv::Moments m = moments(mask, true);
    //cv:Point cnt = m[0];
    cv::Point center(m.m10/m.m00, m.m01/m.m00);
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

    circle( prueba, center, 4, color, -1, 8, 0 );
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", prueba );
    return center;
}

double Caracteristicas::calcDistEuclidiana(int x, int y, int a, int b)
{
    double diffx = a-x;
    double diffy = y-b;
    return sqrt(diffx*diffx + diffy*diffy);
}

double Caracteristicas::calcPerimetro(Mat &bin)
{
    Mat im = bin.clone();
    double perimeter =0.0;
    vector<vector<Point> > contours;

    // Encontrar contornos
    findContours( im, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for(unsigned int i=0; i<contours.size();i++)
    {
        perimeter = max(perimeter,arcLength(contours[i], true));
    }
    return perimeter;
}

double Caracteristicas::calcArea(Mat &bin)
{
    Mat im = bin.clone();
    double area =0.0;
    vector<vector<Point> > contours;

    // Encontrar contornos
    findContours( im, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for(unsigned int i=0; i<contours.size();i++)
    {
        area = max(area,contourArea(contours[i]));
    }
    return area;
}

double Caracteristicas::indiceIrregular(double area, double per)
{
    double indice = pow(per,2)/(4*M_PI*area);
    return indice;
}


float Caracteristicas::calcAngulo(Mat &bin)
{
    Mat im = bin.clone();
    RotatedRect imgWorked;
    float angulo;
    Mat lados = im.clone();
    vector<vector<Point> > contours;
    RNG rng(12345); //colour

    /// Encontrar contornos
    findContours( im, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for(unsigned int i=0; i<contours.size();i++)
    {
         imgWorked = fitEllipse(contours[i]);
    }
    //orientacion de la imagen
    angulo = imgWorked.angle;

    Point2f rect_points[4];
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    imgWorked.points( rect_points );
    for (int i=0; i<4;i++) {
        cout<<rect_points[i].x<<" "<<rect_points[i].y<<endl;
        circle( lados, rect_points[i], 4, color, -1, 8, 0 );
    }

    cv::line(lados, rect_points[0], rect_points[2], color, 1, 8, 0);
    cv::line(lados, rect_points[1], rect_points[3], color, 1, 8, 0);
    namedWindow( "lados", CV_WINDOW_AUTOSIZE );
    imshow( "lados", lados );
    return angulo;

}

vector<double> Caracteristicas::calcAsimetria(Mat &im)
{
    Mat regiones = im.clone();
    vector<vector<Point> > contours;
    Mat cen = im.clone();
    Point centro = calcCentroide(cen);

    int row = im.rows;
    int col = im.cols;

    /// Encontrar contornos
    findContours( im, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Mat mascaraFondo = Mat::zeros(row,col, CV_8UC1);

    Mat BW1 = mascaraFondo.clone();
    Mat BW2 = mascaraFondo.clone();
    Mat BW3 = mascaraFondo.clone();
    Mat BW4 = mascaraFondo.clone();

    //BW1 arriba
    for (int i = 0; i<row-1 ; i++) {
        for(int j = 0 ; j<col-1 ; j++){
            if((i-centro.y) < 0)
                BW1.at<uchar>(i,j) = 1;
        }
    }

    //BW2 abajo
    for (int i = 0; i<row-1 ; i++) {
        for(int j = 0 ; j<col-1 ; j++){
            if((i-centro.y) > 0)
                BW2.at<uchar>(i,j) = 1;
        }
    }

    //BW3 izquierda
    for (int i = 0; i<row-1 ; i++) {
        for(int j = 0 ; j<col-1 ; j++){
            if((j-centro.x) < 0)
                BW3.at<uchar>(i,j) = 1;
        }
    }

    //BW4 derecha
    for (int i = 0; i<row-1 ; i++) {
        for(int j = 0 ; j<col-1 ; j++){
            if((j-centro.x) > 0)
                BW4.at<uchar>(i,j) = 1;
        }
    }

    /// recorte basada en las mascaras definidas
    Mat imgRegion1 = regiones.clone().mul(BW1);
    Mat imgRegion2 = regiones.clone().mul(BW2);
    Mat imgRegion3 = regiones.clone().mul(BW3);
    Mat imgRegion4 = regiones.clone().mul(BW4);

    double area1 = calcArea(imgRegion1);
    double area2 = calcArea(imgRegion2);
    double area3 = calcArea(imgRegion3);
    double area4 = calcArea(imgRegion4);

    double areaTotal = calcArea(regiones);

    double difAreaHorizontal = abs(area1 -area2);
    double difAreaVertical = abs(area3 - area4);

    double porcAreaHor  = (difAreaHorizontal/areaTotal)*100;
    double porcAreaVer = (difAreaVertical/areaTotal)*100;

    vector<double> percent;
    percent.push_back(porcAreaHor);
    percent.push_back(porcAreaVer);
    cout << percent[0] << " --- " <<percent[1]<<endl;

    return percent;
}

Mat Caracteristicas::rotarImagen(Mat &bin, float angle,int border=20)
{
    Mat im = bin.clone();
    Mat bordered_source;
    int top,bottom,left,right;
    top=bottom=left=right=border;
    copyMakeBorder( im, bordered_source, top, bottom, left, right, BORDER_CONSTANT,cv::Scalar() );
    Point2f src_center(bordered_source.cols/2.0F, bordered_source.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(bordered_source, dst, rot_mat, bordered_source.size());
    return dst;
}

vector<double> Caracteristicas::variacionColor(Mat &bin, Mat &src)
{
    Mat source = src.clone();
    Mat binary = bin.clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Encontrar contornos
    findContours( binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    RotatedRect elipse;
    for(unsigned int i=0; i<contours.size();i++)
        {
             elipse = fitEllipse(contours[i]);
        }
        Point2f rect_points[4];
        elipse.points( rect_points );

    /// Obtener la Region de interes (ROI)
    //calculo de punto minimo
    int min_x,min_y;
    min_x = rect_points[0].x;
    min_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        min_x = min(min_x,(int)rect_points[i].x);
        min_y = min(min_y,(int)rect_points[i].y);
    }
    //calculo de punto maximo
    int max_x,max_y;
    max_x = rect_points[0].x;
    max_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        max_x = max(max_x,(int)rect_points[i].x);
        max_y = max(max_y,(int)rect_points[i].y);
    }

    double roiWidth = calcDistEuclidiana(min_x,min_y,max_x,(max_y-min_y));
    double roiHeight = calcDistEuclidiana(min_x,min_y,(max_x-min_x),max_y);


    cv::Rect roi = cv::Rect( (int)min_x, (int)min_y, roiWidth, roiHeight );

    //copies input image in roi
    cv::Mat image_roi = source( roi );

        namedWindow( "roi", CV_WINDOW_AUTOSIZE );
        imshow( "roi", image_roi );

    //computes mean over roi
    //cv::Scalar avgPixelIntensity = cv::mean( image_roi );
    cv::Scalar avgPixelIntensity, stdROI;
    cv::meanStdDev(image_roi,stdROI,avgPixelIntensity ); //BRG


    //prints out only .val[0] since image was grayscale
//    cout << "Pixel intensity over ROI = " << avgPixelIntensity.val[0] << " " <<
//            avgPixelIntensity.val[1] << " "<<avgPixelIntensity.val[2] << " "<< endl;

    vector<double> variance;
    variance.push_back(avgPixelIntensity.val[0]);
    variance.push_back(avgPixelIntensity.val[1]);
    variance.push_back(avgPixelIntensity.val[2]);

    return variance;
}

vector<double> Caracteristicas::cromRelativa(Mat &img, Mat &bin)
{
    Mat binary = bin.clone();
    Mat tumor = img.clone();
    Mat piel = img.clone();
    for(int i = 0 ; i < tumor.rows; i++)
    {
        for(int j = 0; j < tumor.cols; j++)
        {
            if(binary.data[binary.step*i + binary.channels()*j + 0] == 0) {

                tumor.data[tumor.step*i + tumor.channels()*j + 0] = 0;
                tumor.data[tumor.step*i + tumor.channels()*j + 1] = 0;
                tumor.data[tumor.step*i + tumor.channels()*j + 2] = 0;

            }

        }
    }

    for(int i = 0 ; i < piel.rows; i++)
    {
        for(int j = 0; j < piel.cols; j++)
        {
            if(binary.data[binary.step*i + binary.channels()*j + 0] == 255) {

                piel.data[piel.step*i + piel.channels()*j + 0] = 0;
                piel.data[piel.step*i + piel.channels()*j + 1] = 0;
                piel.data[piel.step*i + piel.channels()*j + 2] = 0;

            }

        }
    }

    cv::Scalar avgTumor =cv::mean( tumor );
    cv::Scalar avgPiel =cv::mean( piel );

    Scalar relative;

    relative.val[0] = (avgTumor.val[0]/(avgTumor.val[0]+avgTumor.val[1]+avgTumor.val[2])) -
            (avgPiel[0]/(avgPiel[0]+avgPiel[1]+avgPiel[2]));
    relative.val[1] = (avgTumor.val[1]/(avgTumor.val[0]+avgTumor.val[1]+avgTumor.val[2])) -
            (avgPiel[1]/(avgPiel[0]+avgPiel[1]+avgPiel[2]));
    relative.val[2] = (avgTumor.val[2]/(avgTumor.val[0]+avgTumor.val[1]+avgTumor.val[2])) -
            (avgPiel[2]/(avgPiel[0]+avgPiel[1]+avgPiel[2]));

    cout << "Relative Chromacity = " << relative.val[0] << " " <<
            relative.val[1] << " "<<relative.val[2] << " "<< endl;
    vector<double> result;
    result.push_back(relative.val[0]);
    result.push_back(relative.val[1]);
    result.push_back(relative.val[2]);

    return result;
}

vector<double> Caracteristicas::cordEsferica(Mat &bin, Mat &src)
{
    Mat source = src.clone();
    Mat binary = bin.clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Encontrar contornos
    findContours( binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    RotatedRect elipse;
    for(unsigned int i=0; i<contours.size();i++)
    {
        elipse = fitEllipse(contours[i]);
    }
    Point2f rect_points[4];
    elipse.points( rect_points );

    /// Obtener la Region de interes (ROI)
    //calculo de punto minimo
    int min_x,min_y;
    min_x = rect_points[0].x;
    min_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        min_x = min(min_x,(int)rect_points[i].x);
        min_y = min(min_y,(int)rect_points[i].y);
    }
    //calculo de punto maximo
    int max_x,max_y;
    max_x = rect_points[0].x;
    max_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        max_x = max(max_x,(int)rect_points[i].x);
        max_y = max(max_y,(int)rect_points[i].y);
    }

    double roiWidth = calcDistEuclidiana(min_x,min_y,max_x,(max_y-min_y));
    double roiHeight = calcDistEuclidiana(min_x,min_y,(max_x-min_x),max_y);


    cv::Rect roi = cv::Rect( (int)min_x, (int)min_y, roiWidth, roiHeight );

    //copies input image in roi
    cv::Mat image_roi = source( roi );



    for(int i = 0 ; i < image_roi.rows; i++)
    {
        for(int j = 0; j < image_roi.cols; j++)
        {
            int B = (int)image_roi.data[image_roi.step*i + image_roi.channels()*j + 0];
            int G = (int)image_roi.data[image_roi.step*i + image_roi.channels()*j + 1];
            int R = (int)image_roi.data[image_roi.step*i + image_roi.channels()*j + 2];

            double val_L = sqrt(B*B+ G*G+ R*R);
            double val_AngleA = acos(B/val_L)* 180.0 / PI;
            double val_AngleB = acos(G/(val_L*sin(val_AngleA* 180.0 / PI)))* 180.0 / PI;

            image_roi.data[image_roi.step*i + image_roi.channels()*j + 0] = (int)val_AngleA;
            image_roi.data[image_roi.step*i + image_roi.channels()*j + 1] = (int)val_AngleB;
            image_roi.data[image_roi.step*i + image_roi.channels()*j + 2] =(int)val_L;
        }
    }
    namedWindow( "roi2", CV_WINDOW_AUTOSIZE );
    imshow( "roi2", image_roi );

    cv::Scalar avgSphericalCord, stdROI;
    cv::meanStdDev(image_roi,avgSphericalCord,stdROI ); //BRG


    //prints out only .val[0] since image was grayscale
    cout << "Pixel intensity over ROI = " << avgSphericalCord.val[0] << " " <<
            avgSphericalCord.val[1] << " "<<avgSphericalCord.val[2] << " "<< endl;
    vector<double> result;
    result.push_back(avgSphericalCord.val[0]);
    result.push_back(avgSphericalCord.val[1]);
    result.push_back(avgSphericalCord.val[2]);

    return result;
}

vector<double> Caracteristicas::transfLab(Mat &bin, Mat &src)
{
    Mat source = src.clone();
    Mat binary = bin.clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Encontrar contornos
    findContours( binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    RotatedRect elipse;
    for(unsigned int i=0; i<contours.size();i++)
        {
             elipse = fitEllipse(contours[i]);
        }
        Point2f rect_points[4];
        elipse.points( rect_points );

    /// Obtener la Region de interes (ROI)
    //calculo de punto minimo
    int min_x,min_y;
    min_x = rect_points[0].x;
    min_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        min_x = min(min_x,(int)rect_points[i].x);
        min_y = min(min_y,(int)rect_points[i].y);
    }
    //calculo de punto maximo
    int max_x,max_y;
    max_x = rect_points[0].x;
    max_y = rect_points[0].y;
    for(int i = 1;  i< 4; i++) {
        max_x = max(max_x,(int)rect_points[i].x);
        max_y = max(max_y,(int)rect_points[i].y);
    }

    double roiWidth = calcDistEuclidiana(min_x,min_y,max_x,(max_y-min_y));
    double roiHeight = calcDistEuclidiana(min_x,min_y,(max_x-min_x),max_y);


    cv::Rect roi = cv::Rect( (int)min_x, (int)min_y, roiWidth, roiHeight );

    //copies input image in roi
    cv::Mat image_roi = source( roi );
    cv::Mat lab;

    image_roi *= 100./255;
    cvtColor(image_roi, lab, CV_BGR2Lab);

    namedWindow( "lab", CV_WINDOW_AUTOSIZE );
    imshow( "lab", lab );

    //computes mean over roi
    //cv::Scalar avgPixelIntensity = cv::mean( image_roi );
    cv::Scalar avgLab, stdROI;
    cv::meanStdDev(image_roi,avgLab,stdROI ); //BRG


    //prints out only .val[0] since image was grayscale
    cout << "Pixel intensity over ROI = " << avgLab.val[0] << " " <<
            avgLab.val[1] << " "<<avgLab.val[2] << " "<< endl;

    vector<double> result;
    result.push_back(avgLab.val[0]);
    result.push_back(avgLab.val[1]);
    result.push_back(avgLab.val[2]);

    return result;
}




