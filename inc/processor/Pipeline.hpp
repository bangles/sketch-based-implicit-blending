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
#include "VolumeGenerator.h"
#include <igl/floor.h>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <UserPoints.hpp>
#include "circle.h"
#include "Result.h"

class Pipeline {
private:
    RegistrationProcessor *m_regProcessor;
    OperatorGenerator *m_opGenerator;
    VolumeGenerator *m_volGenerator;

public:
    Pipeline(QOpenGLShaderProgram *program);
    ~Pipeline();
    Circle *circle1, *circle2;
    Result *result;
    Template* m_template;
    UserPoints* userPoints;

    void mapSamplesToTemplate(MatrixXf samples);
    MatrixXf calculateGradientAngles(MatrixXf (&g1)[2], MatrixXf (&g2)[2]);
    void registerPoints();
    void start();
};


#endif /* Pipeline_h */
