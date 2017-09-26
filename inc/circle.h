#ifndef CIRCLE_H
#define CIRCLE_H

#include <Eigen/Dense>
#include <MarchingSquares.h>
#include <igl/copyleft/marching_cubes.h>
#include <igl/unique.h>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

using namespace Eigen;

class Circle {

private:
  MatrixXf m_X;
  MatrixXf m_Y;
  MatrixXf gradient[2];

public:
  MatrixXf distanceField;
  Vector2f center;
  float r1, r2;
  //  MatrixXf vertices;
  //  MatrixXi faces;

  std::vector<Vector2d> line_segments, triangles;

  Circle(float x1, float y1, float in_r1, float in_r2, MatrixXf &X, MatrixXf &Y);
  void calculateDistanceField(MatrixXf &X, MatrixXf &Y, MatrixXf &df, MatrixXf (&g)[2]);
  void calculateDistanceToField(MatrixXf &df);
  void polygonize(float isoValue);
  void blend(Circle *circle);
};

#endif // CIRCLE_H
