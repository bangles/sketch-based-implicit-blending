#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <functional>
#include <vector>

Eigen::MatrixXd marchingSquares(const Eigen::MatrixXf &X, const Eigen::MatrixXf &Y, const Eigen::MatrixXf &S, double isoValue, bool fill);

#endif
