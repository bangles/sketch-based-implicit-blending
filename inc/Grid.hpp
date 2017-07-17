//
//  Grid.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 11/07/17.
//
//

#ifndef Grid_hpp
#define Grid_hpp

#include<vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Program.h"

namespace ebib {
    
    class Grid {
    private:
        GLuint gVBO;
        std::vector<glm::vec3> vertices;
        tdogl::Program* gProgram = NULL;
        
    public:
        Grid(int numIndices, tdogl::Program& gProgram);
        ~Grid();
        void renderGrid();
    };
    
}

#endif /* Grid_hpp */
