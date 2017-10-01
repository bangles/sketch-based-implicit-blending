//  Utils.h
//  ebib_demo
//  Created by Ishmeet Singh Kohli on 04/08/17.

#ifndef Utils_h
#define Utils_h

#include <Eigen/Dense>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <iostream>

using namespace std;
using namespace Eigen;

class Utils {
private:
  const static IOFormat CSVFormat;

public:
  static bool writeMatrixXiToCSVfile(string name, MatrixXi &matrix);
  static bool writeMatrixXfToCSVfile(string name, MatrixXf &matrix);
  static MatrixXf slice(Tensor<float, 3> tensor,int dimension, int index);
  static MatrixXi slice(Tensor<int, 3> tensor,int dimension, int index);
};

#endif /* Utils_h */
