#ifndef VOLUMEGENERATOR_H
#define VOLUMEGENERATOR_H

#include <Eigen/Dense>
#include <vector>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>

using namespace Eigen;
using namespace std;

static const Vector2i MIN(-1, -1);
static const Vector2i MAX(1, 1);

typedef Tensor<float, 3> Tensor3f;

class VolumeGenerator {
private:

public:
    VolumeGenerator();
    ~VolumeGenerator();
    MatrixXf generate(MatrixXf &df1, MatrixXf &df2, MatrixXf &alpha, vector<MatrixXf> &G);
    MatrixXf interpolate3(MatrixXf &df1, MatrixXf &df2, MatrixXf &alpha, vector<MatrixXf> &G);


    Tensor3f generate(Tensor3f &df1, Tensor3f &df2, Tensor3f &alpha, Tensor3f &G);
    Tensor3f interpolate3(Tensor3f &df1, Tensor3f &df2, Tensor3f &alpha, Tensor3f &G);
};

#endif // VOLUMEGENERATOR_H
