//
//  Processor.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 22/07/17.
//
//

#ifndef Processor_h
#define Processor_h

#include "nnsearch.h"
#include "Template.hpp"
#include "Utils.h"
#include <igl/ismember.h>
#include <igl/sort.h>
#include <igl/slice.h>
#include <igl/unique.h>
#include <vector>
#include <math.h>

namespace ebib {
    using namespace Eigen;
    
    class Processor
    {
        
    private:
        Template* _template = NULL;
        MatrixXf _points;
        MatrixXf _vertices;
        MatrixXi _triangles;
        MatrixXf _weights;
        
        TrimeshSearcher<MatrixXf,MatrixXi> searcher;
        
    public:
        Processor(Template &inTemplate);
        ~Processor();
        void process(MatrixXf inQueries);
        void push(MatrixXi& MC, int& lastRow, VectorXi a, VectorXi b);
        void push(MatrixXi& MC, int& lastRow, int a, int b);
        std::vector<VectorXi> convertToArray(MatrixXi& MC, VectorXi& IX);
        void removeColumns(MatrixXf& R, VectorXi columns);
        MatrixXi mergedDOFs();
        void pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries);
        void tikhonovEnergy(MatrixXf& A, VectorXf& b);
        void spineSmoothEnergy(MatrixXf& A, VectorXf& b);
        void laplacianSliceEnergy(MatrixXf& A, VectorXf& b);
        

    };
}

#endif /* Processor_h */
