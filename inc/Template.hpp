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
    static const int BSPLINE_ORDER = 3;
    
    struct MeshInfo{
        VectorXi line0;
        VectorXi line1;
        
        VectorXi spine0;
        VectorXi spine1;
        
        VectorXi line0o;
        VectorXi line1o;
        
        MatrixXi slices;
    };
    
    class Template {
    private:
        GLuint gVBO[5];
        tdogl::Program* gProgram = NULL;
        BSplineSurface *surface1, *surface2;
        void renderControlPoints();
        
    public:
        Template(tdogl::Program& gProgram);
        ~Template();
        
        Patch mPatches[2];
        MeshInfo meshInfo;
        void evaluateSingle(float u, float v, Vector3f &point);
        void render();
        void updatePatches();
        void processPoints();
    };
    
}

#endif /* Template_hpp */
