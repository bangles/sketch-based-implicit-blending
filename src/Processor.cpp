//  Processor.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 18/07/17.

#include "Processor.hpp"
#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

using namespace ebib;


Processor::Processor(Template &inTemplate){
    _template = &inTemplate;
    searcher.build(_template->mPatches[0].vertices, _template->mPatches[0].triangles);
}

void Processor::process(MatrixXf inQueries) {
    MatrixXf points(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS * 2);
    
    points <<   _template->mPatches[0].points,
                _template->mPatches[1].points;
    
    MatrixXf A1, A2;
    VectorXf b1, b2;
    
//    pointToPlaneEnergy(A1, b1, inQueries);
    laplacianSliceEnergy(A2, b2);
    
    int n = inQueries.cols();
    VectorXf w(1);
    w << sqrt(1.0/n);
    
    MatrixXf A(A1.rows(),A1.cols());
    A << A1 * w[0];
    
    MatrixXf b(b1.rows(),b1.cols());
    b <<  b1 * w[0];
    
    //    MatrixXf x = A.colPivHouseholderQr().solve(b);
//    MatrixXf x = (A.transpose() * A).ldlt().solve(A.transpose() * b);
    
//    Map<MatrixXf> points(_patches[0].points.data(), _patches[0].points.size(), 1);
//    MatrixXf x1 = points.cwiseProduct(x);
//    
//    Map<MatrixXf> solution(x1.data(), 3, x1.size()/3);
//    LOG(solution);
    
    //    _patch->points = solution;
}


void Processor::pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries) {
    MatrixXf footpoints(3,inQueries.cols()),barycentric(3,inQueries.cols());
    VectorXi findex(inQueries.cols());
    
    searcher.closest_point(inQueries, footpoints, findex);
    searcher.barycentric(footpoints, findex, barycentric);
    
    A.resize(inQueries.cols(), _template->mPatches[0].points.cols() * 3);
    b.resize(inQueries.cols());
    
    for (int i = 0 ; i < inQueries.cols(); i++) {
        Vector3i vIdx = _template->mPatches[0].triangles.col(findex[i]);
        
        Vector3f n = footpoints.col(i) - inQueries.col(i);
        n.normalize();
        VectorXf w = barycentric.col(i);
        
        MatrixXf Wbi(3,_template->mPatches[0].points.cols());
        Wbi.row(0) = _template->mPatches[0].weights.col(vIdx[0]);
        Wbi.row(1) = _template->mPatches[0].weights.col(vIdx[1]);
        Wbi.row(2) = _template->mPatches[0].weights.col(vIdx[2]);
        VectorXf Wbw = w.transpose() * Wbi;
        
        VectorXf row(_template->mPatches[0].points.cols() * 3);
        row << (Wbw * n[0]), (Wbw * n[1]), (Wbw * n[2]);
        A.row(i) = row;
        b[i] = inQueries.col(i).dot(n);
    }
}


void Processor::laplacianSliceEnergy(MatrixXf& A, VectorXf& b) {
    int count = 0;
    A.resize(0, _template->mPatches[0].points.cols() * 3);
//    b.resize(inQueries.cols());
    
    for(int i = 0; i< NUM_CONTROL_POINTS;i++)
        for(int j = 1; j< NUM_CONTROL_POINTS - 1; j++)
            for(int k = 0; k < 2; k++)  {
                A(count,_template->meshInfo.slices(i,j-1)) = 1;
                A(count,_template->meshInfo.slices(i,j)) = -2;
                A(count,_template->meshInfo.slices(i,j+1)) = 1;
                
                A(++count,_template->meshInfo.slices(i,j-1)) = 1;
                A(count,_template->meshInfo.slices(i,j)) = -2;
                A(count,_template->meshInfo.slices(i,j+1)) = 1;
            }
    
    b.resize(count,1);
    b = MatrixXi::Zero(count, 1);
    
}
