#ifndef AGPOBLACION_H
#define AGPOBLACION_H
#include "agcromosoma.h"
#include "backpropagation/redneuronal.h"
#include <algorithm>

class AgPoblacion
{
    private:
        vector<AgCromosoma*> individuos;
        vector<AgCromosoma*> hijos;
        vector<float> apt_indiv;
        vector<int> num_neu_capa;
        RedNeuronal *red;
    public:
        AgPoblacion(int,vector<int>,RedNeuronal*);
        virtual ~AgPoblacion();
        vector<AgCromosoma*> cruzar(AgCromosoma*,AgCromosoma*);
        void mutar(AgCromosoma*);
        void calcularFitness(vector<pair<vector<float>,vector<int> > >);
        AgCromosoma* seleccionarIndividuo();
        void llenarRuleta();
        void ordenarPorFitness();
        void generarHijos();
        void siguientePoblacion();
        AgCromosoma* getMejor();
};
#endif // AGPOBLACION_H
