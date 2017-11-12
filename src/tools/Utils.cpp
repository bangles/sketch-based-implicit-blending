//  Utils.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 04/08/17.

#include <Utils.hpp>
#include <fstream>

using namespace Eigen;

#define LOG(x) cout << x << endl

const IOFormat Utils::CSVFormat = IOFormat(StreamPrecision, DontAlignCols, ", ", "\n");

bool Utils::writeMatrixXiToCSVfile(string name, MatrixXi &matrix) {
  ofstream file(name.c_str());
  file << matrix.format(CSVFormat);

  return true;
}

bool Utils::writeMatrixXfToCSVfile(string name, MatrixXf &matrix) {
  ofstream file(name.c_str());
  file << matrix.format(CSVFormat);

  return true;
}

typedef Map<MatrixXf, 0, Stride<Dynamic, Dynamic>> EMap;
// Supported just for cubic 3d tensors
MatrixXf Utils::slice(Tensor<float, 3> tensor, int dimension, int index) {
  int size = tensor.dimension(0);
  MatrixXf result(0, 0);

  switch (dimension) {
  case 0:
    result = EMap(tensor.data() + index, size, size, Stride<Dynamic, Dynamic>(size, size * size));
    break;
  case 1:
    result = EMap(tensor.data() + index * size, size, size, Stride<Dynamic, Dynamic>(size * size, 1));
    break;
  case 2:
    result = EMap(tensor.data() + index * size * size, size, size, Stride<Dynamic, Dynamic>(size, 1));
    break;
  }

  return result;
}

typedef Map<MatrixXi, 0, Stride<Dynamic, Dynamic>> EMapI;
// Supported just for cubic 3d tensors
MatrixXi Utils::slice(Tensor<int, 3> tensor, int dimension, int index) {
  int size = tensor.dimension(0);
  MatrixXi result(0, 0);

  switch (dimension) {
  case 0:
    result = EMapI(tensor.data() + index, size, size, Stride<Dynamic, Dynamic>(size, size * size));
    break;
  case 1:
    result = EMapI(tensor.data() + index * size, size, size, Stride<Dynamic, Dynamic>(size * size, 1));
    break;
  case 2:
    result = EMapI(tensor.data() + index * size * size, size, size, Stride<Dynamic, Dynamic>(size, 1));
    break;
  }

  return result;
}

Vector2f Utils::mapPointToGL(QPoint point, int width, int height) { return Vector2f(2 * ((float)point.x() / width - 0.5), 2 * (0.5 - (float)point.y() / height)); }

Vector2f Utils::mapPointToGL(float x, float y, int width, int height) { return Vector2f(2 * (x / width - 0.5), 2 * (0.5 - y / height)); }
