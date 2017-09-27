//
//  Pipeline.h
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#ifndef Pipeline_h
#define Pipeline_h

#include "RegistrationProcessor.hpp"
#include "OperatorGenerator.h"
#include <igl/floor.h>
#include <vector>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>


class Pipeline {
private:
    RegistrationProcessor *m_regProcessor;
    OperatorGenerator *m_opGenerator;

public:
    Pipeline(QOpenGLShaderProgram *program);
    ~Pipeline();
    Template* m_template;

    void start(MatrixXf userSamples);
};


#endif /* Pipeline_h */
