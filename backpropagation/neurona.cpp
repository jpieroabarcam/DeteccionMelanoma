#include "neurona.h"

Neurona::Neurona(int a,int b)
{
    icapa=a;
    ineur=b;
}

Neurona::~Neurona()
{
    //dtor
}

float Neurona::salida(){
    if(icapa==0)return neto;
    else return 1.0/(1.0+exp(-1.0*neto));
}

float Neurona::getNeto()
{
    return neto;
}

void Neurona::setNeto(float _neto)
{
    neto=_neto;
}
