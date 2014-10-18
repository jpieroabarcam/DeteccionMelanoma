#include "redneuronal.h"

#include<iostream>

RedNeuronal::RedNeuronal(vector<int> vec)
{
    num_capas=vec.size();
    numNeuroPorCapa=vec;
    for(unsigned int i=0;i<vec.size();i++){
        Capa capa(i,vec[i],NULL,NULL);
        capas.push_back(capa);
    }
    //Agregamos matrices de pesos
    for(unsigned int i=0;i<vec.size()-1;i++){
        float **p=new float*[vec[i]];
        for(int j=0;j<vec[i];j++)
            p[j]=new float[vec[i+1]];
        capas[i].der=p;
        capas[i+1].izq=p;
    }
}

RedNeuronal::~RedNeuronal()
{
    //dtor
}


vector<float> RedNeuronal::evaluar(vector<float> in)
{
    float suma;
    //colocamos la entrada como valor neto de neuronas de entrada
    for(int i=0;i<capas[0].num_neuronas;i++){
        capas[0].neuronas[i].setNeto(in[i]);
    }
    //Calculamos la salida PROPAGACION HACIA ADELANTE
    for(int c=1;c<num_capas;c++){
        //Hallamos el valor neto de cada neurona de la capa
        for(int n1=0;n1<capas[c].num_neuronas;n1++){
            suma=0;
            for(int n0=0;n0<capas[c-1].num_neuronas;n0++){
                //cout<<"*:"<<capas[c-1].neuronas[n0].salida()<<endl;
                //cout<<"+:"<<capas[c-1].der[n0][n1];

                suma+=capas[c-1].neuronas[n0].salida()*capas[c-1].der[n0][n1];
            }//cout<<"s:"<<suma<<endl;
            capas[c].neuronas[n1].setNeto(suma);
        }
    }
    //Extraemos la salida
    //float *salida;
    //salida=new float[capas[num_capas-1].num_neuronas];
    //for(int i=0;i<capas[num_capas-1].num_neuronas;i++)
    //   salida[i]=capas[num_capas-1].neuronas[i].salida();
    //return salida;

    vector<float> salida;
    for(int i=0;i<capas[num_capas-1].num_neuronas;i++)
        salida.push_back(capas[num_capas-1].neuronas[i].salida());
    return salida;
}


void RedNeuronal::pesosAleatorios()
{
    int a,b;
    for(int i=0;i<num_capas-1;i++){
        a=numNeuroPorCapa[i];
        b=numNeuroPorCapa[i+1];
        for(int _a=0;_a<a;_a++){
            for(int _b=0;_b<b;_b++){
                capas[i].der[_a][_b]=(drand48()>0.5)?drand48():(-1*drand48());
            }
        }
    }
}

vector<float> RedNeuronal::entrenar(vector<pair<vector<float>,vector<int> > > in)
{
    vector<vector<float> > error;
    vector<float> salida;
    vector<float> newerror;
    vector<float> er;
    vector<float> error_total;
    float aux,p;

    for(unsigned int i=0;i<in.size();i++){
        error.clear();
        //Calculamos la salida para una entrada
        salida=evaluar(in[i].first);
        //Error capa de salida
        er.clear();
        for(int s=0;s<capas[num_capas-1].num_neuronas;s++){
            aux=(in[i].second[s]-salida[s])*salida[s]*(1-salida[s]);
            er.push_back(aux);
        }
        //calculamos error total
        p=0;
        for(unsigned int e=0;e<er.size();e++){
            p=p+er[e]*er[e];
        }
        error_total.push_back(p*0.5);

        error.insert(error.begin(),er);
        //Error para capas ocultas
        for(int s=num_capas-2;s>0;s--){
            for(int n=0;n<capas[s].num_neuronas;n++){
                aux=0;
                for(int w=0;w<capas[s+1].num_neuronas;w++){
                    aux=aux+capas[s].der[n][w]*er[w];
                }
                newerror.push_back(aux*capas[s].neuronas[n].salida()*(1-capas[s].neuronas[n].salida()));
            }
            error.insert(error.begin(),newerror);
            er=newerror;
            newerror.clear();
        }
        error.insert(error.begin(),newerror);
        //Actualizamos los nuevos pesos
        for(int c=0;c<num_capas-2;c++){
            for(int n=0;n<capas[c].num_neuronas;n++){
                for(int w=0;w<capas[c+1].num_neuronas;w++){
                    aux=(error[c+1])[w]*capas[c].neuronas[n].salida(); //Tasa de Crecimiento
                    capas[c].der[n][w]=capas[c].der[n][w]+aux;
                }
            }
        }

    }
    return error_total;
}

void RedNeuronal::setPesos(vector<float**> m){
    for(unsigned int c=0;c<m.size();c++){
        capas[c].der=m[c];
        capas[c+1].izq=m[c];
    }
}


vector<float> RedNeuronal::fitness(vector<pair<vector<float>,vector<int> > > in){
    vector<float> salida;
    vector<float> error_total;
    vector<float> er;
    float aux,p;
    for(unsigned int i=0;i<in.size();i++){
        //Calculamos la salida para una entrada
        salida=evaluar(in[i].first);
        //Error capa de salida
        er.clear();
        for(int s=0;s<capas[num_capas-1].num_neuronas;s++){
            //aux=(in[i].second[s]-salida[s])*salida[s]*(1-salida[s]);cout<<aux<<endl;
            aux=(in[i].second[s]-salida[s]);
            aux=aux*aux;
            er.push_back(aux);
        }
        //calculamos error total
        p=0;
        for(unsigned int e=0;e<er.size();e++){
            p=p+er[e]*er[e];
        }
        error_total.push_back(p*0.5);
    }
    return error_total;
}
