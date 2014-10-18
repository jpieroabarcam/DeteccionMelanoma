#ifndef AGALGORITMOGENETICO_H
#define AGALGORITMOGENETICO_H

#include "agpoblacion.h"

class AgAlgoritmoGenetico
{
    private:
        int num_generaciones;
        AgPoblacion *poblacion;
        vector<pair<vector<float>,vector<int> > > patron;
    public:
        AgAlgoritmoGenetico(int,vector<int>,RedNeuronal*);
        AgCromosoma *nextGeneracion();
        virtual ~AgAlgoritmoGenetico();
        void agregarPatron(pair<vector<float>,vector<int> >);

};

#endif // AGALGORITMOGENETICO_H
