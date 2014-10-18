#include "agpoblacion.h"

AgPoblacion::AgPoblacion(int n,vector<int> nxc,RedNeuronal *_red)
{
    num_neu_capa=nxc;
    for(int i=0;i<n;i++){
        AgCromosoma *ag=new AgCromosoma(nxc);
        individuos.push_back(ag);
        apt_indiv.push_back(0.0);
    }
    red=_red;
}

AgPoblacion::~AgPoblacion()
{
    //dtor
}

float **matriz(int x,int y){
    float **p=new float*[x];
    for(int j=0;j<x;j++)
            p[j]=new float[y];
    return p;
}


vector<AgCromosoma*> AgPoblacion::cruzar(AgCromosoma *a,AgCromosoma *b)
{
    vector<AgCromosoma*> h;
    int x,y;
    float **h1,**h2;
    vector<float**> H1;
    vector<float**> H2;
    //Probabilidad de Cruce
    if(drand48()<0.8){
        for(unsigned int i=0;i<(num_neu_capa.size()-1);i++){

           //elegimos punto de cruce
            x=rand()%num_neu_capa[i];
            y=rand()%num_neu_capa[i+1];
            //intercambiamos informacion
            h1=matriz(num_neu_capa[i],num_neu_capa[i+1]);
            h2=matriz(num_neu_capa[i],num_neu_capa[i+1]);
            for(int m=0;m<x;m++)
                for(int n=0;n<y;n++){
                    h1[m][n]=(a->data[i])[m][n];
                    h2[m][n]=(b->data[i])[m][n];
                }
            for(int m=x;m<num_neu_capa[i];m++)
                for(int n=y;n<num_neu_capa[i+1];n++){
                    h1[m][n]=(b->data[i])[m][n];
                    h2[m][n]=(a->data[i])[m][n];
                }
            H1.push_back(h1);
            H2.push_back(h2);
        }
        //Creamos Hijos
        AgCromosoma *hijo1=new AgCromosoma();
        hijo1->setData(H1);
        AgCromosoma *hijo2=new AgCromosoma();
        hijo2->setData(H2);
        mutar(hijo1);
        mutar(hijo2);
        h.push_back(hijo1);
        h.push_back(hijo2);
    }


    return h;
}

        /*for(unsigned int i=0;i<individuos.size();i++)
            cout<<individuos[i]->data.size()<<endl;*/


void AgPoblacion::mutar(AgCromosoma *a)
{

    int x,y;
    //Probabilidad de Mutacion
    for(unsigned int i=0;i<(num_neu_capa.size()-1);i++){
        if(drand48()<1.0){
            //cantidad a mutar %
            for(int j=0;j<int(num_neu_capa[i]+num_neu_capa[i+1]);j++){
                //elegimos genes a mutar
                x=rand()%num_neu_capa[i];
                y=rand()%num_neu_capa[i+1];
                (a->data[i])[x][y]=drand48()*((drand48()>0.5)?(1.0):(-1.0));
            }
        }
    }
}

void AgPoblacion::calcularFitness(vector<pair<vector<float>,vector<int> > > patrones)
{
    float sum;
    vector<float> error;
    for(unsigned int i=0;i<individuos.size();i++){
        red->setPesos((individuos[i])->data);
        error=red->fitness(patrones);
        sum=0;
        for(unsigned int a=0;a<error.size();a++){
            sum+=error[a];
        }
        apt_indiv[i]=1-sum/error.size();
        (individuos[i])->aptitud=apt_indiv[i];
    }
}

void AgPoblacion::llenarRuleta()
{
    float total=0;
    for(unsigned int i=0;i<apt_indiv.size();i++){
        total+=apt_indiv[i];
    }
    apt_indiv[0]=apt_indiv[0]/total;
    for(unsigned int i=1;i<apt_indiv.size();i++){
        apt_indiv[i]=float(apt_indiv[i]/total+apt_indiv[i-1]);
    }

}


bool fun_sort(AgCromosoma *a,AgCromosoma *b)
{
    return (a->aptitud)<(b->aptitud);
}

void AgPoblacion::ordenarPorFitness()
{
    sort(individuos.begin(),individuos.end(),fun_sort);
}


AgCromosoma* AgPoblacion::seleccionarIndividuo(){
    int i=0;
    float ale=drand48();
    while(apt_indiv[i]<ale)i++;
    return individuos[i];
}


void AgPoblacion::generarHijos()
{
    //----------------------
    AgCromosoma *p1;
    AgCromosoma *p2;
    hijos.clear();
    vector<AgCromosoma*> aux;
    unsigned int c=0;
    while(c<(individuos.size()-2)){
        p1=seleccionarIndividuo();
        p2=seleccionarIndividuo();p1=p2;p2=p1;
        aux=cruzar(p1,p2);
        if(!aux.empty()){
            hijos.push_back(aux[0]);
            hijos.push_back(aux[1]);
            c=c+2;
        };
    }

}


void AgPoblacion::siguientePoblacion(){
    //nuevaGeneracion
    for(unsigned int i=0;i<individuos.size()-2;i++){
        delete individuos[i];
        individuos[i]=hijos[i];
    }

}

AgCromosoma* AgPoblacion::getMejor(){
    return individuos[individuos.size()-1];
}

