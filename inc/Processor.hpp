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
#include <math.h> 

namespace ebib {
    using namespace Eigen;
    
    class Processor
    {
        
    private:
        Patch* _patches;
//        const MatrixXf _queries;
        TrimeshSearcher<MatrixXf,MatrixXi> searcher;
        
    public:
        Processor(Patch inPatches[]);
        ~Processor();
        void process(MatrixXf inQueries);
        void pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries);
        void laplacianSliceEnergy(MatrixXf& A, VectorXf& b);

    };
}

#endif /* Processor_h */
