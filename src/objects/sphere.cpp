#include "sphere.h"
#include "VolumeGenerator.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Sphere::Sphere(float x1, float y1, float z1, float in_r1, float in_r2, int res) {
  center << x1, y1, z1;
  r1 = in_r1;
  r2 = in_r2;
  calculateDistanceField(res);
}

void Sphere::calculateDistanceField(int res) {
  distanceField.resize(res, res, res);
  gradient[0].resize(res, res, res);
  gradient[1].resize(res, res, res);
  gradient[2].resize(res, res, res);
  VectorXf resMap = VectorXf::LinSpaced(res, MIN[0], MAX[0]);
  for (int i = 0; i < res; i++) {
    for (int j = 0; j < res; j++) {
      for (int k = 0; k < res; k++) {
        distanceField(i, j, k) = sqrt(pow((resMap[i] - center[0]), 2) + pow((resMap[j] - center[1]), 2) + pow((resMap[k] - center[2]), 2));
        gradient[0](i, j, k) = (resMap[i] - center[0]) / distanceField(i, j, k);
        gradient[1](i, j, k) = (resMap[j] - center[1]) / distanceField(i, j, k);
        gradient[2](i, j, k) = (resMap[k] - center[2]) / distanceField(i, j, k);

        distanceField(i, j, k) = std::min(1.0, std::max(0.0, 0.5 - ((distanceField(i, j, k) - r1) / (r2 * 2))));

//        distanceField(i, j, k) = 0.5 - ((distanceField(i, j, k) - r1) / (r2 * 2));
      }
    }
  }
}

// void Sphere::polygonize(float isoValue) {
//  //  int res_x = m_X.rows(), res_y = m_Y.rows(), res_z = 2;
//  MatrixXf GV(res_x * res_y * res_z, 3);
//  ////  VectorXf S(distanceField.rows() * distanceField.cols() * 2);

//  //  VectorXf S1(Map<VectorXf>(distanceField.data(), distanceField.cols()*distanceField.rows()));

//  //  VectorXf S(S1.size() * 2);
//  //  S << S1,S1;
//  //  S = S.array() - isoValue;

//  //  for (int z = 0; z < res_z; z++) {
//  //    for (int y = 0; y < res_y; y++) {
//  //      for (int x = 0; x < res_x; x++) {
//  //        GV.row(x + res_x * (y + res_y * z)) = RowVector3f(m_X(x, y), m_Y(x, y), 0.02 * z);
//  //        S[x + res_x * (y + res_y * z)] = distanceField(x,y) - isoValue;
//  //      }
//  //    }
//  //  }

//  igl::copyleft::marching_cubes(S, GV, res_x, res_y, res_z, vertices, faces);
//  //  vertices.col(2).fill(0);
//  //  MatrixXf uniqueVertices;
//  //  VectorXi ia,ic;
//  //  igl::unique_rows(vertices, uniqueVertices, ia, ic);
//  //  vertices = uniqueVertices;
//  //  vertices.transposeInPlace();
//  //  faces.transposeInPlace();

//  //    std::vector<Vector2d> line_segments, triangles;
//  line_segments.clear();
//  triangles.clear();
//  marchingSquares(m_X, m_Y, distanceField, isoValue, line_segments, triangles);
//}
