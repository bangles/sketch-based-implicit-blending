#include "sphere.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Sphere::Sphere(float x1, float y1, float z1, float in_r1, float in_r2, MatrixXf &X, MatrixXf &Y, MatrixXf &Z); {
  center << x1, y1;
  r1 = in_r1;
  r2 = in_r2;
  m_X = X;
  m_Y = Y;
  calculateDistanceField(m_X, m_Y, distanceField, gradient);
  polygonize(0.5);
}

void Sphere::calculateDistanceField(MatrixXf &X, MatrixXf &Y, MatrixXf &df, MatrixXf (&g)[2]) {
    df = ((X.array() - center[0]).pow(2) + (Y.array() - center[1]).pow(2)).sqrt();
    g[0] = (X.array() - center[0]) / df.array();
    g[1] = (Y.array() - center[1]) / df.array();
    calculateDistanceToField(df);
}

void Sphere::calculateDistanceToField(MatrixXf &df) {
  df = 0.5 - (df.array() - r1) / (r2 * 2);
  df = df.cwiseMax(0);
  df = df.cwiseMin(1);
}

void Sphere::polygonize(float isoValue) {
  //  int res_x = m_X.rows(), res_y = m_Y.rows(), res_z = 2;
  //  MatrixXf GV(res_x * res_y * res_z, 3);
  ////  VectorXf S(distanceField.rows() * distanceField.cols() * 2);

  //  VectorXf S1(Map<VectorXf>(distanceField.data(), distanceField.cols()*distanceField.rows()));

  //  VectorXf S(S1.size() * 2);
  //  S << S1,S1;
  //  S = S.array() - isoValue;

  //  for (int z = 0; z < res_z; z++) {
  //    for (int y = 0; y < res_y; y++) {
  //      for (int x = 0; x < res_x; x++) {
  //        GV.row(x + res_x * (y + res_y * z)) = RowVector3f(m_X(x, y), m_Y(x, y), 0.02 * z);
  ////        S[x + res_x * (y + res_y * z)] = distanceField(x,y) - isoValue;
  //      }
  //    }
  //  }

  //  igl::copyleft::marching_cubes(S, GV, res_x, res_y, res_z, vertices, faces);
  //  vertices.col(2).fill(0);
  //  MatrixXf uniqueVertices;
  //  VectorXi ia,ic;
  //  igl::unique_rows(vertices, uniqueVertices, ia, ic);
  //  vertices = uniqueVertices;
  //  vertices.transposeInPlace();
  //  faces.transposeInPlace();

  //    std::vector<Vector2d> line_segments, triangles;
  line_segments.clear();
  triangles.clear();
  marchingSquares(m_X, m_Y, distanceField, isoValue, line_segments, triangles);
}

void Sphere::blend(Sphere *sphere) {
  distanceField += sphere->distanceField;
  polygonize(0.5);
}
