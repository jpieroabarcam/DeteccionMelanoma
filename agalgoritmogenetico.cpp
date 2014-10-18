#include "agalgoritmogenetico.h"

AgAlgoritmoGenetico::AgAlgoritmoGenetico(int n,vector<int> v,RedNeuronal *_red)
{
    poblacion=new AgPoblacion(n,v,_red);
}

AgAlgoritmoGenetico::~AgAlgoritmoGenetico()
{
    //dtor
}

AgCromosoma *AgAlgoritmoGenetico::nextGeneracion(){
    poblacion->calcularFitness(patron);
    poblacion->llenarRuleta();
    poblacion->generarHijos();
    poblacion->ordenarPorFitness();
    poblacion->siguientePoblacion();
    return poblacion->getMejor();
}

void AgAlgoritmoGenetico::agregarPatron(pair<vector<float>,vector<int> > p){
    patron.push_back(p);
}
