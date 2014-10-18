#ifndef AGCROMOSOMA_H
#define AGCROMOSOMA_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class AgCromosoma{
    private:
        vector<float**>data;
        float aptitud;
    public:
        AgCromosoma();
        AgCromosoma(vector<int>);
        void setData(vector<float**>);
        virtual ~AgCromosoma();
    friend class AgPoblacion;
    friend bool fun_sort(AgCromosoma*,AgCromosoma*);
    friend int main();
    friend int main2();
    friend class MainWindow;
};

#endif // AGCROMOSOMA_H
