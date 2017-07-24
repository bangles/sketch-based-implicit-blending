//
//  Processor.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 18/07/17.
//
//

#include "Processor.hpp"
#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

using namespace ebib;


Processor::Processor(Patch inPatch){
    searcher.build(inPatch.vertices, inPatch.triangles);
}

MatrixXf Processor::process(MatrixXf inQueries) {
    MatrixXf footpoints(3,inQueries.cols()),barycentric(3,inQueries.cols());
    VectorXi findex(inQueries.cols());
    
    LOG(inQueries.rows() << 'x' << inQueries.cols());
    
    searcher.closest_point(inQueries, footpoints, findex);
    searcher.barycentric(footpoints, findex, barycentric);
    
    return footpoints;
    
}



