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

namespace ebib {
    using namespace Eigen;
    
    class Processor
    {
        
    private:
        Patch _patch;
//        const MatrixXf _queries;
        TrimeshSearcher<MatrixXf,MatrixXi> searcher;
        
    public:
        Processor(Patch inPatch);
        ~Processor();
        void process(MatrixXf inQueries);
        void pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries);
    };
}

#endif /* Processor_h */
