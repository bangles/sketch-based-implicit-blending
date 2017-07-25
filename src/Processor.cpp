//  Processor.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 18/07/17.

#include "Processor.hpp"
#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

using namespace ebib;


Processor::Processor(Patch inPatch){
    _patch = inPatch;
    searcher.build(inPatch.vertices, inPatch.triangles);
}

void Processor::process(MatrixXf inQueries) {
    MatrixXf A1;
    VectorXf b1;
    
    pointToPlaneEnergy(A1, b1, inQueries);
    
    
//    int n = inQueries.cols();
//    VectorXf w;
//    
//    w[0] = 1/n;
//    w = w.sqrt();
//    
//    MatrixXf A;
//    A.col(0) = A1 * w[0];
//    
//    
//    MatrixXf b;
//    b.col(0) = b1 * w[0];
//
//    
////    MatrixXf Af = inQueries * (A.transpose() * A) * inQueries.transpose();
////    MatrixXf bf = inQueries * A.transpose() * b;
//    
//    
//    MatrixXf x = A1.colPivHouseholderQr().solve(b1);
    
}


void Processor::pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries) {
    MatrixXf footpoints(3,inQueries.cols()),barycentric(3,inQueries.cols());
    VectorXi findex(inQueries.cols());
    
    searcher.closest_point(inQueries, footpoints, findex);
    searcher.barycentric(footpoints, findex, barycentric);
    
    A.resize(inQueries.cols(), _patch.points.cols() * 3);
    b.resize(inQueries.cols());
    
    for (int i = 0 ; i < inQueries.cols(); i++) {
        Vector3i vIdx = _patch.triangles.col(findex[i]);
        
        Vector3f n = footpoints.col(i) - inQueries.col(i);
        n.normalize();
        
        VectorXf w = barycentric.col(i);
        
        MatrixXf Wbi(_patch.points.cols(),3);
        Wbi.col(0) = _patch.weights.col(vIdx[0]);
        Wbi.col(1) = _patch.weights.col(vIdx[1]);
        Wbi.col(2) = _patch.weights.col(vIdx[2]);
        
        VectorXf Wbw = Wbi * w;

        A.row(i) << (Wbw * n[0]).transpose(), (Wbw * n[1]).transpose(), (Wbw * n[2]).transpose();
        b[i] = inQueries.col(i).dot(n);
    }
}
