#include "capa.h"

Capa::Capa(int _nc,int _nn,float **_izq,float **_der)
{
    num_capa=_nc;
    num_neuronas=_nn;
    izq=_izq;
    der=_der;
    for(int i=0;i<_nn;i++){
        Neurona neu(_nc,i);
        neuronas.push_back(neu);
    }
}

Capa::~Capa()
{

}
