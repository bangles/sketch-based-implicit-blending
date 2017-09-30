//
//  Processor.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 22/07/17.
//
//

#ifndef RegistrationProcessor_h
#define RegistrationProcessor_h

#include "nnsearch.h"
#include "Template.hpp"
#include "Utils.h"
#include <igl/ismember.h>
#include <igl/sort.h>
#include <igl/slice.h>
#include <igl/unique.h>
#include <vector>
#include <math.h>

using namespace Eigen;

class RegistrationProcessor
{

private:
    Template* m_template;
    MatrixXf _points;
    MatrixXf _vertices;
    MatrixXi _triangles;
    MatrixXf _weights;
    int iter;

    TrimeshSearcher<MatrixXf,MatrixXi>* searcher;

public:
    RegistrationProcessor(Template *inTemplate);
    ~RegistrationProcessor();
    void registerPoints(MatrixXf inQueries);
    MatrixXf step(MatrixXf inQueries);
    void push(MatrixXi& MC, int& lastRow, VectorXi a, VectorXi b);
    void push(MatrixXi& MC, int& lastRow, int a, int b);
    std::vector<VectorXi> convertToArray(MatrixXi& MC, VectorXi& IX);
    void removeColumns(MatrixXf& R, VectorXi columns);
    MatrixXi mergedDOFs();
    void updateSearcher();
    void pointToPlaneEnergy(MatrixXf& A, VectorXf& b, MatrixXf inQueries);
    void tikhonovEnergy(MatrixXf& A, VectorXf& b);
    void spineSmoothEnergy(MatrixXf& A, VectorXf& b);
    void laplacianSliceEnergy(MatrixXf& A, VectorXf& b);
    void laplacianLineEnergy(MatrixXf& A, VectorXf& b);
    void fixedPointEnergy(MatrixXf& A, VectorXf& b);
};

#endif /* RegistrationProcessor_h */
