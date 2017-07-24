//
//  Template.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.
//
//

#ifndef Template_hpp
#define Template_hpp

//#include<vector>
#include <GL/glew.h>
//#include <glm/glm.hpp>

#include "Program.h"
#include "BSplineSurface.hpp"


namespace ebib {
    static const int NUM_CONTROL_POINTS = 5;
    
    class Template {
    private:
        GLuint gVBO[6];
        tdogl::Program* gProgram = NULL;
        BSplineSurface surface;
        void renderControlPoints();
        
    public:
        Template(tdogl::Program& gProgram);
        ~Template();
        
        Patch mPatch1;
        Patch mPatch2;
        void initPoints(MatrixXf* points1, MatrixXf* points2);
        void render();
        
    };
    
}

#endif /* Template_hpp */
