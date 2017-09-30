#ifndef OPERATORGENERATOR_H
#define OPERATORGENERATOR_H

#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <Template.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <igl/floor.h>

using namespace Eigen;
using namespace std;

typedef Triplet<double, int> ETriplet;
typedef SparseMatrix<double> SpMat;

class OperatorGenerator {
private:
    Template* m_template;

public:
    OperatorGenerator(Template *in_template);
    ~OperatorGenerator();

    vector<MatrixXf> generateOperator(int S);
    VectorXf findVs(int res);
    void fillGrid(int S, int Sz, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, VectorXf V,vector<MatrixXf> &G, vector<MatrixXi> &mask, vector<MatrixXi> &Gs);
    void fillSlice(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, float v, MatrixXf &out_G, MatrixXi &out_mask, MatrixXi &out_Gs);
    void solve(MatrixXf &G, MatrixXi &mask, int S, int Sz);
    void print(MatrixXf matrix);
    void print(MatrixXi matrix);
    void print(vector<MatrixXf> matrix);
    void print(vector<MatrixXi> matrix);
};

#endif // OPERATORGENERATOR_H
