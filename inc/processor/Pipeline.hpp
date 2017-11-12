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
#include "OperatorGenerator.hpp"
#include "VolumeGenerator.hpp"
#include <igl/floor.h>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <UserPoints.hpp>
#include "circle.hpp"
#include "sphere.hpp"
#include "Result.hpp"
#include "Result3D.hpp"

class Pipeline {
private:
    RegistrationProcessor *m_regProcessor;
    OperatorGenerator *m_opGenerator;
    VolumeGenerator *m_volGenerator;
    int S;

public:
    Pipeline(QOpenGLShaderProgram *program);
    ~Pipeline();
    Circle *circle1, *circle2;
    Sphere *sphere1, *sphere2;
    Result *result;
    Result3D *result3D;
    Template* m_template;
    UserPoints* userPoints;

    void mapSamplesToTemplate(vector<Vector2f> sampleVector);
    MatrixXf calculateGradientAngles(MatrixXf (&g1)[2], MatrixXf (&g2)[2]);
    Tensor<float,3> calculateGradientAngles(Tensor<float,3> (&g1)[3], Tensor<float,3> (&g2)[3]);
    void initializeSpheres();
    void registerPoints();
    void resetRegistration();
    void start(QOpenGLShaderProgram *program);
};


#endif /* Pipeline_h */
