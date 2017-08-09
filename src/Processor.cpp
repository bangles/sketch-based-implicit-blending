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
    _points.resize(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS * 2);
    _points <<_template->mPatches[0].points, _template->mPatches[1].points;
    
    _vertices.resize(_template->mPatches[0].vertices.rows(), _template->mPatches[0].vertices.cols() * 2);
    _vertices <<_template->mPatches[0].vertices, _template->mPatches[1].vertices;
    
    _triangles.resize(_template->mPatches[0].triangles.rows(), _template->mPatches[0].triangles.cols() * 2);
    _triangles <<_template->mPatches[0].triangles, _template->mPatches[1].triangles;
    
    _weights.resize(_template->mPatches[0].weights.rows() * 2, _template->mPatches[0].weights.cols() * 2);
    MatrixXf zeroWeights = MatrixXf::Zero(_template->mPatches[0].weights.rows(),_template->mPatches[0].weights.cols());
    _weights <<_template->mPatches[0].weights, zeroWeights, zeroWeights, _template->mPatches[1].weights;
    
    searcher.build(_vertices, _triangles);
}

void Processor::process(MatrixXf inQueries) {
    MatrixXf A0, A1, A2, A3, A4, A5, A6;
    VectorXf b0, b1, b2, b3, b4, b5, b6;
    
    MatrixXi MC = mergedDOFs();
    MatrixXf R = MatrixXf::Identity(_points.size(), _points.size());
    
    if(MC.size() != 0) {
        for(int i = 0; i < MC.rows(); i++ ) {
            R(MC(i,1),MC(i,0)) = 1;
        }
        
        removeColumns(R, MC.col(1));
    }
    
    
    pointToPlaneEnergy(A0, b0, inQueries);
//    laplacianSliceEnergy(A2, b2);
    tikhonovEnergy(A3, b3);
    spineSmoothEnergy(A6, b6);
    
    int n = inQueries.cols();
    VectorXf w = VectorXf::Zero(7);
    w(0) = sqrt(1.0/n);
    w(3) = glm::pow(10, -3); // Tikhonov
    w(6) = glm::pow(10, -1); // Spine Smoothness
    
    int rowsA = A0.rows() + A3.rows() + A6.rows();
    int rowsB = b0.rows() + b3.rows() + b6.rows();
    
    MatrixXf A(rowsA, A0.cols());
    A <<    A0 * w[0],
            A3 * w[3],
            A6 * w[6];
    
    MatrixXf b(rowsB, b0.cols());
    b <<    b0 * w[0],
            b3 * w[3],
            b6 * w[6];
    
    A = A * R;
    MatrixXf x = A.colPivHouseholderQr().solve(b);
    x = R * x;
    Map<MatrixXf> solution(x.data(), 3, x.size()/3);
    
    LOG(solution);
    Utils::writeToCSVfile("solution", solution);
    
    _template->mPatches[0].points = solution.leftCols(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    _template->mPatches[1].points = solution.rightCols(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
}

void Processor::removeColumns(MatrixXf& R, VectorXi columns){
    
    VectorXi output,indexes;
    igl::unique(columns,output);
    columns = output;
    igl::sort(columns,1,false,output,indexes);
    
    std::vector<VectorXf> A;
    for(int i = 0; i < R.cols(); i++ ) {
        A.push_back(R.col(i));
    }
    
    for(int i = 0; i < columns.size(); i++ ) {
        A.erase(A.begin() + output(i));
    }
    
    R.resize(R.rows(),A.size());
    
    for(int i = 0; i < A.size(); i++ ) {
        R.col(i) = A[i];
    }
    
}

MatrixXi Processor::mergedDOFs() {
    int s = _points.cols();
    MatrixXi MC(170,2);
    
    VectorXi s1 = VectorXi::Ones(NUM_CONTROL_POINTS) * s;
    RowVectorXi s2 = RowVectorXi::Ones(NUM_CONTROL_POINTS * 2) * s;
    int end = NUM_CONTROL_POINTS - 1;
    int lastRow = 0;
    
    push(MC,lastRow,_template->meshInfo.spine0,_template->meshInfo.spine1);
    push(MC,lastRow,_template->meshInfo.spine0 + s1, _template->meshInfo.spine1  + s1);
    push(MC,lastRow,_template->meshInfo.spine0 + 2*s1, _template->meshInfo.spine1  + 2*s1);

    push(MC,lastRow,_template->meshInfo.line0, _template->meshInfo.line0o);
    push(MC,lastRow,_template->meshInfo.line0 + 2*s1, _template->meshInfo.line0o + 2*s1);
    
    push(MC,lastRow,_template->meshInfo.line1 + s1, _template->meshInfo.line1o + s1);
    push(MC,lastRow,_template->meshInfo.line1 + 2*s1, _template->meshInfo.line1o + 2*s1);
    
    push(MC,lastRow,_template->meshInfo.slices.row(0), _template->meshInfo.slices.row(1));
    push(MC,lastRow,_template->meshInfo.slices.row(0) + s2, _template->meshInfo.slices.row(1) + s2);

    push(MC,lastRow,_template->meshInfo.slices.row(end), _template->meshInfo.slices.row(end - 1));
    push(MC,lastRow,_template->meshInfo.slices.row(end) + s2, _template->meshInfo.slices.row(end - 1) + s2);

    for(int i = 0; i < NUM_CONTROL_POINTS; i++) {
        for(int j = 1; j < 2 * NUM_CONTROL_POINTS; j++) {
            push(MC,lastRow,_template->meshInfo.slices(i,1) + 2 * s, _template->meshInfo.slices(i,j) + 2 * s);
        }
        
        for(int j = 0; j < 2 * NUM_CONTROL_POINTS; j++) {
            push(MC,lastRow,_template->meshInfo.slices(i,j) + 2 * s, _template->meshInfo.slices(i,2 * NUM_CONTROL_POINTS - j - 1) +  s);
        }
    }

    VectorXi indexes, output;
    igl::sort(MC.col(1),1,true,output,indexes);
    std::vector<VectorXi> A = convertToArray(MC, indexes);
    
    for(int i = 0; i < A.size();i++) {
        for(int j = 0; j < A.size();j++) {
            if (j == i)
                continue;

            igl::ismember(A[i], A[j], indexes, output);

            if(indexes.array().any()) {
                VectorXi temp(A[i].size() + A[j].size());
                temp << A[i], A[j];
                A[i] = temp;
                A.erase(A.begin() + j);
            }
        }
    }

    MC.resize(159,2);
    
    lastRow = 0;
    for(int i = 0; i < A.size();i++) {
        igl::unique(A[i],output);
        for(int j = 1; j < output.size(); j++) {
            MC.row(lastRow++) << output[0],output[j];
        }
    }
    
    return MC;
}

std::vector<VectorXi> Processor::convertToArray(MatrixXi& MC, VectorXi& IX) {
    std::vector<VectorXi> A;
    for(int i = 0; i < IX.size(); i++ ) {
        A.push_back(MC.row(IX(i)));
    }
    return A;
}


void Processor::push(MatrixXi& MC, int& lastRow, VectorXi a, VectorXi b) {
    int count = a.size();
    MC.block(lastRow,0,count,2) << a,b;
    lastRow+=count;
}

void Processor::push(MatrixXi& MC, int& lastRow, int a, int b) {
    MC.block(lastRow,0,1,2) << a,b;
    lastRow++;
}


void Processor::pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries) {
    MatrixXf footpoints(3,inQueries.cols()),barycentric(3,inQueries.cols());
    VectorXi findex(inQueries.cols());
    
    searcher.closest_point(inQueries, footpoints, findex);
    searcher.barycentric(footpoints, findex, barycentric);
    
    A.resize(inQueries.cols(), _points.cols() * 3);
    b.resize(inQueries.cols());
    
    for (int i = 0 ; i < inQueries.cols(); i++) {
        Vector3i vIdx = _triangles.col(findex[i]);
        
        Vector3f n = footpoints.col(i) - inQueries.col(i);
        n.normalize();
        VectorXf w = barycentric.col(i);
        
        MatrixXf Wbi(3,_points.cols());
        Wbi <<  _weights.row(vIdx[0]),
                _weights.row(vIdx[1]),
                _weights.row(vIdx[2]);
        
//        Wbi.col(0) = _weights.row(vIdx[0]).transpose();
//        Wbi.col(1) = _weights.row(vIdx[1]).transpose();
//        Wbi.col(2) = _weights.row(vIdx[2]).transpose();
        VectorXf Wbw = w.transpose() * Wbi;
        
        VectorXf row(_points.cols() * 3);
        row << (Wbw * n[0]), (Wbw * n[1]), (Wbw * n[2]);
        A.row(i) = row;
        b[i] = inQueries.col(i).dot(n);
    }
}


void Processor::laplacianSliceEnergy(MatrixXf& A, VectorXf& b) {
    int count = 0;
    int size = _points.cols();
    int end = NUM_CONTROL_POINTS - 1;
    A = MatrixXf::Zero(NUM_CONTROL_POINTS* (NUM_CONTROL_POINTS - 2) * 6 ,size * 3);
    
    for(int i = 0; i< NUM_CONTROL_POINTS; i++)
        for(int j = 1; j< NUM_CONTROL_POINTS - 1; j++)
            for(int k = 0; k < 3; k++)  {
                A(count,_template->meshInfo.slices(i,j - 1) + k * size) = 1;
                A(count,_template->meshInfo.slices(i,j) + k * size) = -2;
                A(count++,_template->meshInfo.slices(i,j + 1) + k * size) = 1;
                
                A(count,_template->meshInfo.slices(i,end - j - 1) + k * size) = 1;
                A(count,_template->meshInfo.slices(i,end - j) + k * size) = -2;
                A(count++,_template->meshInfo.slices(i,end - j + 1) + k * size) = 1;
            }
    
    Utils::writeToCSVfile("laplacianSliceEnergy.csv", A);
    
    b = MatrixXf::Zero(count, 1);
    
}

void Processor::tikhonovEnergy(MatrixXf& A, VectorXf& b) {
    b = Map<MatrixXf>(_points.data(), _points.size(), 1);
    A = MatrixXf::Identity(_points.size(),_points.size());
}

void Processor::spineSmoothEnergy(MatrixXf& A, VectorXf& b) {
    int size = _points.cols();
    A = MatrixXf::Zero(NUM_CONTROL_POINTS * 3, size * 3);
    b = MatrixXf::Zero(NUM_CONTROL_POINTS * 3, 1);
    
    for(int i = 0; i< NUM_CONTROL_POINTS; i++)
        for(int k = 0; k < 3; k++)  {
            A(i + NUM_CONTROL_POINTS * k, _template->meshInfo.slices(i,NUM_CONTROL_POINTS - 1) + k * size) = 1;
            A(i + NUM_CONTROL_POINTS * k, _template->meshInfo.slices(i,NUM_CONTROL_POINTS) + k * size) = 1;
            A(i + NUM_CONTROL_POINTS * k, _template->meshInfo.slices(i,NUM_CONTROL_POINTS + 2) + k * size) = 1;
        }
}

