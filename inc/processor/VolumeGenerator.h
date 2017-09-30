#ifndef VOLUMEGENERATOR_H
#define VOLUMEGENERATOR_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

static const Vector2i MIN(-1, -1);
static const Vector2i MAX(1, 1);

class VolumeGenerator {
private:

public:
    VolumeGenerator();
    ~VolumeGenerator();
    MatrixXf generate(MatrixXf &df1, MatrixXf &df2, MatrixXf &alpha, vector<MatrixXf> &G);
    MatrixXf interpolate3(MatrixXf &df1, MatrixXf &df2, MatrixXf &alpha, vector<MatrixXf> &G);
};

#endif // VOLUMEGENERATOR_H
