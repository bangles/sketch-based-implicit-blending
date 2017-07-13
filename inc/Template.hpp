//
//  Template.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.
//
//

#ifndef Template_hpp
#define Template_hpp

#include<vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Program.h"
#include "BSplineSurface.hpp"


namespace ebib {
    static const int NUM_CONTROL_POINTS = 5;
    
    class Template {
    private:
        GLuint gVBO[6];
        tdogl::Program* gProgram = NULL;
        std::vector<GLuint> indices;
        BSplineSurface surface;
        void renderControlPoints();
        
    public:
        Template(tdogl::Program& gProgram);
        ~Template();
        void initPoints(std::vector<glm::vec3> points1, std::vector<glm::vec3> points2);
        void render();
        
    };
    
}

#endif /* Template_hpp */
