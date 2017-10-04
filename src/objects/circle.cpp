#include "circle.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Circle::Circle(float x1, float y1, float in_r1, float in_r2, MatrixXf &X, MatrixXf &Y) {
  center << x1, y1;
  r1 = in_r1;
  r2 = in_r2;
  m_X = X;
  m_Y = Y;
  calculateDistanceField(m_X, m_Y, distanceField, gradient);
  polygonize(0.5);
}

void Circle::calculateDistanceField(MatrixXf &X, MatrixXf &Y, MatrixXf &df, MatrixXf (&g)[2]) {
  df = ((X.array() - center[0]).pow(2) + (Y.array() - center[1]).pow(2)).sqrt();
  g[0] = (X.array() - center[0]) / df.array();
  g[1] = (Y.array() - center[1]) / df.array();
  calculateDistanceToField(df);
}

void Circle::calculateDistanceToField(MatrixXf &df) {
  df = 0.5 - (df.array() - r1) / (r2 * 2);
  df = df.cwiseMax(0);
  df = df.cwiseMin(1);
}

void Circle::polygonize(float isoValue) {
  line_segments.clear();
  triangles.clear();
  marchingSquares(m_X, m_Y, distanceField, isoValue, triangles, true);
  marchingSquares(m_X, m_Y, distanceField, 0.01, line_segments, false);
}

void Circle::blend(Circle *circle) {
  distanceField += circle->distanceField;
  polygonize(0.5);
}
