#ifndef OPERATORGENERATOR_H
#define OPERATORGENERATOR_H

#include "Utils.hpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <Template.hpp>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <igl/floor.h>

using namespace Eigen;
using namespace std;

typedef Triplet<float, int> ETriplet;
typedef SparseMatrix<float> SpMat;
typedef Tensor<float, 3> Tensor3f;
typedef Tensor<int, 3> Tensor3i;

class OperatorGenerator {
private:
  Template *m_template;

public:
  OperatorGenerator(Template *in_template);
  ~OperatorGenerator();

  Tensor3f generateOperator(int S);
  VectorXf findVs(int res);
  void fillGrid(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, VectorXf V, Tensor3f &G, Tensor3i &mask, Tensor3i &Gs);
  void fillSlice(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, float v, MatrixXf &out_G, MatrixXi &out_mask, MatrixXi &out_Gs);
  void solve(Tensor3f &G, Tensor3i &mask, int S);
  void fillOnes(Tensor3f &G, Tensor3i &mask, int S);
  void print(MatrixXf matrix);
  void print(MatrixXi matrix);
  void print(vector<MatrixXf> matrix);
  void print(vector<MatrixXi> matrix);
};

#endif // OPERATORGENERATOR_H
