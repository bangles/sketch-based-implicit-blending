#ifndef Sphere_H
#define Sphere_H

#include <Eigen/Dense>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <igl/copyleft/marching_cubes.h>
#include <igl/unique.h>

using namespace Eigen;

class Sphere {

private:
public:
  Tensor<float, 3> distanceField;
  Tensor<float, 3> gradient[3];
  Vector3f center;
  float r1, r2;

  Sphere(float x1, float y1, float z1, float in_r1, float in_r2, int res);
  void calculateDistanceField(int res);
};

#endif // Sphere_H
