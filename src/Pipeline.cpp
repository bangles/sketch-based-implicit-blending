//
//  Pipeline.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#include <Pipeline.hpp>

#define LOG(x) std::cout<<x<<std::endl

using namespace ebib;

Pipeline::Pipeline(Template &inTemplate) {
        _template = &inTemplate;
}

void Pipeline::generateOperator() {
    VectorXf V = findVs(200);
    LOG(V);
}

VectorXf Pipeline::findVs(int res) {
    int sampleSize = 1000;
    VectorXf t = VectorXf::LinSpaced(res,0,1);
    VectorXf ts = VectorXf::LinSpaced(sampleSize,0,1);
    
    MatrixXf sp = MatrixXf::Zero(sampleSize,3);
    
    for(int i=0; i<sampleSize; i++ ){
        Vector3f point(0,0,0);
        _template->evaluateSingle(0, ts[i], point);
        sp.row(i) = point;
    }
    
    VectorXf spz = sp.col(2);
    VectorXf V = VectorXf::Zero(res);
    V[0] = 0.0;
    V[res - 1] = 1.0;
    
    int k = 1;
    
    for(int i = 0; i < sampleSize - 1 ; i++ ) {
        float a = spz[i];
        float b = spz[i+1];
        float c = t[k];
        
        if(c >= a && c <= b) {
            float alpha = (c-a)/(b-a);
            V[k] = (1-alpha) * ts[i] + alpha * ts[i+1];
            k = k + 1;
        }
    }
    
    return V;
}

void Pipeline::start() {
    generateOperator();
}

