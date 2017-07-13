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

#include<vector>
//#include <GL/glew.h>
//#include <glm/glm.hpp>

#include "Program.h"

namespace ebib {
    static const int NUM_OF_USER_POINTS = 30;
    
    class UserPoints {
    private:
        GLuint gVBO;
        tdogl::Program* gProgram = NULL;
        
    public:
        UserPoints(tdogl::Program& gProgram);
        ~UserPoints();
        void render();
    };
    
}


#endif /* UserPoints_hpp */
