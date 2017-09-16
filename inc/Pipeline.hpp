//
//  Pipeline.h
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#ifndef Pipeline_h
#define Pipeline_h

#include "Processor.hpp"
#include <igl/floor.h>
#include <vector>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

namespace ebib {
    
    class Pipeline {
    private:
        Template* _template = NULL;
        
    public:
        Pipeline(Template &inTemplate);
        ~Pipeline();

        void start();
        void generateOperator(int S);
        VectorXf findVs(int res);
        void fillGrid(int S, int Sz, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, VectorXf V,vector<MatrixXf> &G, vector<MatrixXi> &mask, vector<MatrixXi> &Gs);
        void fillSlice(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, float v, MatrixXf &out_G, MatrixXi &out_mask, MatrixXi &out_Gs);
        void solve(MatrixXf &G, MatrixXi &mask, int S, int Sz);
        void print(MatrixXf matrix);
        void print(MatrixXi matrix);
        void print(vector<MatrixXf> matrix);
        void print(vector<MatrixXi> matrix);
    };
}


#endif /* Pipeline_h */
