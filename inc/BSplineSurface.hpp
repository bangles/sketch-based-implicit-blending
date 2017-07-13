//
//  BSplineSurface.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 10/07/17.
//
//

#ifndef BSplineSurface_hpp
#define BSplineSurface_hpp

#include <glm/glm.hpp>
#include <vector>


namespace ebib {
    static const int NUM_SAMPLES = 100;
    
    class BSplineSurface {
    public:
        
        BSplineSurface();
        ~BSplineSurface();
    
        float bSplineBasis(float U[], int o, int i, float u, int num_samples, int max_value);
        std::vector<glm::vec3> evaluateSurface(int k, std::vector<glm::vec3> points, int noOfPoints, int num_samples);
        
        private:
        
    };
    
}

#endif /* BSplineSurface_hpp */
