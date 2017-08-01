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
    
    struct Patch{
        MatrixXf points;
        MatrixXi grid;
        MatrixXf vertices;
        MatrixXi triangles;
        MatrixXf weights;
    };
    
    class BSplineSurface {
    
    private:
        float bSplineBasis(float U[], int o, int i, float u, int num_samples, int max_value);
    public:
        BSplineSurface();
        ~BSplineSurface();
        void evaluateSurface(int k, Patch& patch, int noOfPoints, int num_samples);
    };
}

#endif /* BSplineSurface_hpp */
