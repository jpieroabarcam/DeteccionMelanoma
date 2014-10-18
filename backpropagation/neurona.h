#ifndef NEURONA_H
#define NEURONA_H

#include<math.h>

class Neurona{
    private:
        float neto;
        int icapa;
        int ineur;
    public:
        Neurona(int,int);
        virtual ~Neurona();
        float salida();
        void setNeto(float);
        float getNeto();
};

#endif // NEURONA_H
