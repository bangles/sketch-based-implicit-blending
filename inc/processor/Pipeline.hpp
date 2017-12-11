//
//  Pipeline.h
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#ifndef Pipeline_h
#define Pipeline_h

#include "OperatorGenerator.hpp"
#include "RegistrationProcessor.hpp"
#include "Result.hpp"
#include "Result3D.hpp"
#include "VolumeGenerator.hpp"
#include "circle.hpp"
#include "sphere.hpp"
#include <UserPoints.hpp>
#include <algorithm>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <igl/floor.h>
#include <igl/slice_mask.h>

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
  Template *m_template;
  UserPoints *userPoints;

  void map(vector<Vector2f>& samples);
  void registerPoints();
  void generate(QOpenGLShaderProgram *program);
  void automate(vector<Vector2f>& samples, QOpenGLShaderProgram *program);
  MatrixXf preprocessSamples(MatrixXf& samples);

  MatrixXf calculateGradientAngles(MatrixXf (&g1)[2], MatrixXf (&g2)[2]);
  Tensor<float, 3> calculateGradientAngles(Tensor<float, 3> (&g1)[3], Tensor<float, 3> (&g2)[3]);
  void initializeSpheres();
  void resetRegistration();
};

#endif /* Pipeline_h */
