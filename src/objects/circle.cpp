#include "circle.hpp"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Circle::Circle(float x1, float y1, float in_r1, float in_r2, MatrixXf &X, MatrixXf &Y) {
  center << x1, y1;
  r1 = in_r1;
  r2 = in_r2;
  m_X = X;
  m_Y = Y;
  bounds = QRectF(x1 - in_r1, y1 - in_r1, 2 * in_r1, 2 * in_r1);
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
  triangles = marchingSquares(m_X, m_Y, distanceField, isoValue, true);
  line_segments = marchingSquares(m_X, m_Y, distanceField, 0.01, false);
}

void Circle::blend(Circle *circle) {
  distanceField += circle->distanceField;
  polygonize(0.5);
}

void Circle::move(float deltaX, float deltaY) {
  center[0] += deltaX;
  center[1] += deltaY;

  bounds.adjust(deltaX, deltaY, deltaX, deltaY);

  triangles.row(0) = triangles.row(0).array() + deltaX;
  triangles.row(1) = triangles.row(1).array() + deltaY;

  line_segments.row(0) = line_segments.row(0).array() + deltaX;
  line_segments.row(1) = line_segments.row(1).array() + deltaY;
}
