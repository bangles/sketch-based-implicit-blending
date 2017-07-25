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
    
    static const int NUM_SAMPLES = 100;
    
    
    struct Patch{
        MatrixXf points;
        MatrixXf vertices;
        MatrixXf weights;
        MatrixXi triangles;
    };
    
    class BSplineSurface {
    public:
        
        BSplineSurface();
        ~BSplineSurface();
    
        float bSplineBasis(float U[], int o, int i, float u, int num_samples, int max_value);
        Patch evaluateSurface(int k, MatrixXf points, int noOfPoints, int num_samples);
        
        private:
        
    };
    
}

#endif /* BSplineSurface_hpp */
