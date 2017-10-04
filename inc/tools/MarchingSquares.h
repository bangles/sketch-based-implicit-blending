#ifndef MARCHING_SQUARES_H
#define MARCHING_SQUARES_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <functional>
#include <vector>

void marchingSquares(
    const Eigen::MatrixXf &X, const Eigen::MatrixXf &Y,
    const Eigen::MatrixXf &S, double isoValue,
    std::vector<Eigen::Vector2d> &vertices,
    bool fill);

#endif
