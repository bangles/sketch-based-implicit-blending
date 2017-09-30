#ifndef Sphere_H
#define Sphere_H

#include <Eigen/Dense>
#include <MarchingSquares.h>
#include <igl/copyleft/marching_cubes.h>
#include <igl/unique.h>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

using namespace Eigen;

class Sphere {

private:
  MatrixXf m_X;
  MatrixXf m_Y;
  MatrixXf m_Z;

public:
  MatrixXf distanceField;
  Vector3f center;
  float r1, r2;
  MatrixXf gradient[2];
  //  MatrixXf vertices;
  //  MatrixXi faces;

  std::vector<Vector2d> line_segments, triangles;

  Sphere(float x1, float y1, float z1, float in_r1, float in_r2, MatrixXf &X, MatrixXf &Y, MatrixXf &Z);
  void calculateDistanceField(MatrixXf &X, MatrixXf &Y, MatrixXf &Z, MatrixXf &df, MatrixXf (&g)[2]);
  void calculateDistanceToField(MatrixXf &df);
  void polygonize(float isoValue);
  void blend(Sphere *sphere);
};

#endif // Sphere_H
