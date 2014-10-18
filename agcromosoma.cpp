#include "agcromosoma.h"



AgCromosoma::AgCromosoma()
{

}

AgCromosoma::AgCromosoma(vector<int> vec)
{
    for(unsigned int m=0;m<(vec.size()-1);m++){
        float **a=new float*[vec[m]];
        for(int i=0;i<vec[m];i++){
            a[i]=new float[vec[m+1]];
        }
        data.push_back(a);
    }
    int n;

    for(unsigned int m=0;m<data.size();m++){
        for(int i=0;i<vec[m];i++){
            for(int j=0;j<vec[m+1];j++){
                n=rand()%4;
                (data[m])[i][j]=drand48()*((drand48()>0.5)?1.0:-1.0)+n;
            }
        }
    }

}



AgCromosoma::~AgCromosoma()
{
    //dtor
}


 void AgCromosoma::setData(vector<float**> _data)
 {
     data=_data;
 }
