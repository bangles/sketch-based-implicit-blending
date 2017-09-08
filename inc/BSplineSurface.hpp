//
//  BSplineSurface.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 10/07/17.
//
//

#ifndef BSplineSurface_hpp
#define BSplineSurface_hpp

#include <Eigen/Dense>
#include <GL/glew.h>

namespace ebib {
    using namespace Eigen;
    static const int NUM_SAMPLES = 40;
    static RowVectorXf DUMMY_WEIGHT(0);
    
    struct Patch {
        MatrixXf points;
        MatrixXi grid;
        MatrixXf vertices;
        MatrixXi triangles;
        MatrixXf weights;
    };
    
    class BSplineSurface {
    
    private:
        Patch* _patch = NULL;
        float* U = NULL;
        int k;
        int numPoints;
        float bSplineBasis(int i, int o, float u);
    public:
        BSplineSurface(Patch& patch, int noOfPoints, int k);
        ~BSplineSurface();
        void evaluateSurface();
        void evaluate(float u, float v, Vector3f& point, RowVectorXf& weight = DUMMY_WEIGHT);
    };
}

#endif /* BSplineSurface_hpp */
