#ifndef REDNEURONAL_H
#define REDNEURONAL_H

#include <vector>
#include "capa.h"
#include <stdlib.h>
#include <utility>
#include <math.h>
using namespace std;

class RedNeuronal
{
    private:
        int num_capas;
        vector<int> numNeuroPorCapa;
        vector<Capa> capas;
    public:
        RedNeuronal(vector<int>);
        virtual ~RedNeuronal();
        vector<float> evaluar(vector<float>);
        void setPesos(vector<float**>);
        void pesosAleatorios();
        vector<float> entrenar(vector<pair<vector<float>,vector<int> > > );
        vector<float> fitness(vector<pair<vector<float>,vector<int> > > );
};

#endif // REDNEURONAL_H
