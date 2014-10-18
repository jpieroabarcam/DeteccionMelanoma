#ifndef CAPA_H
#define CAPA_H

#include "neurona.h"
#include <vector>

using namespace std;

class Capa
{
    private:
        int num_capa;
        int num_neuronas;
        float **izq;
        float **der;
        vector<Neurona> neuronas;
    public:
        Capa(int,int,float**,float**);
        virtual ~Capa();
    friend class RedNeuronal;
};

#endif // CAPA_H
