//
//  UserPoints.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 13/07/17.
//
//

#ifndef UserPoints_hpp
#define UserPoints_hpp

#define PI 3.14159265


#include <Eigen/Dense>
//#include <glm/glm.hpp>

#include "Program.h"

namespace ebib {
    using namespace Eigen;
    
    static const int NUM_OF_USER_POINTS = 10;
    
    class UserPoints {
    private:
        GLuint gVBO;
        tdogl::Program* gProgram = NULL;
        
    public:
        UserPoints(tdogl::Program& gProgram);
        ~UserPoints();
        MatrixXf mUserPoints;
        void render();
    };
    
}


#endif /* UserPoints_hpp */
