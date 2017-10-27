//  Pipeline.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#include "Utils.h"
#include <Pipeline.hpp>

#define LOG(x) std::cout << x << std::endl
using namespace std;

typedef Tensor<float, 3> Tensor3f;

Pipeline::Pipeline(QOpenGLShaderProgram *program) {
  m_template = new Template(program);
  m_regProcessor = new RegistrationProcessor(m_template);
  m_opGenerator = new OperatorGenerator(m_template);
  m_volGenerator = new VolumeGenerator();
  userPoints = new UserPoints(program);
  S = 100;

  MatrixXf X = RowVectorXf::LinSpaced(S, MIN[0], MAX[0]).replicate(S, 1);
  MatrixXf Y = VectorXf::LinSpaced(S, MIN[1], MAX[1]).replicate(1, S);

  circle1 = new Circle(0, -0.25, 0.25, 0.5, X, Y);
  circle2 = new Circle(0, 0.25, 0.25, 0.5, X, Y);

  result = new Result(X, Y, program);
  result3D = new Result3D(S, program);
}

void Pipeline::registerPoints() {
  m_regProcessor->registerPoints(userPoints->m_userPoints);
  m_template->updatePatches();
}

void Pipeline::initializeSpheres() {
  sphere1 = new Sphere(circle1->center[0], circle1->center[1], 0, circle1->r1, circle1->r2, S);
  sphere2 = new Sphere(circle2->center[0], circle2->center[1], 0, circle2->r1, circle2->r2, S);
}

void Pipeline::start() {
  Tensor3f G = m_opGenerator->generateOperator(20);
  initializeSpheres();
  Tensor3f alpha = calculateGradientAngles(sphere1->gradient, sphere2->gradient);
  Tensor3f distanceField = m_volGenerator->generate(sphere1->distanceField, sphere2->distanceField, alpha, G);
  result3D->setDistanceField(distanceField);

  //  MatrixXf alpha = calculateGradientAngles(circle1->gradient, circle2->gradient);
  //  MatrixXf distanceField = m_volGenerator->generate(circle1->distanceField, circle2->distanceField, alpha, G);
  //  result->setDistanceField(distanceField);
}

void Pipeline::mapSamplesToTemplate(vector<Vector2f> sampleVector) {
  Map<MatrixXf> samples(sampleVector.data()->data(), 2, sampleVector.size());

  MatrixXf f1, f2, g1[2], g2[2];
  MatrixXf X = samples.row(0);
  MatrixXf Y = samples.row(1);

  circle1->calculateDistanceField(X, Y, f1, g1);
  circle2->calculateDistanceField(X, Y, f2, g2);
  MatrixXf alpha = calculateGradientAngles(g1, g2);
  MatrixXf templatePoints(3, samples.cols());
  templatePoints.row(0) = f1;
  templatePoints.row(1) = f2;
  templatePoints.row(2) = alpha;
  userPoints->setUserPoints(templatePoints);
}

MatrixXf Pipeline::calculateGradientAngles(MatrixXf (&g1)[2], MatrixXf (&g2)[2]) {
  MatrixXf mag1 = (g1[0].array().pow(2) + g1[1].array().pow(2)).sqrt();
  MatrixXf mag2 = (g2[0].array().pow(2) + g2[1].array().pow(2)).sqrt();
  MatrixXf dotP = ((g1[0].array() * g2[0].array()) + (g1[1].array() * g2[1].array())) / (mag1.array() * mag2.array());
  MatrixXf alpha = dotP.array().acos() / M_PI;
  return alpha;
}

Tensor<float, 3> Pipeline::calculateGradientAngles(Tensor<float, 3> (&g1)[3], Tensor<float, 3> (&g2)[3]) {
  int res = g1[0].dimension(0);
  Tensor<float, 3> alpha(res, res, res);
  for (int i = 0; i < res; i++) {
    for (int j = 0; j < res; j++) {
      for (int k = 0; k < res; k++) {
        float mag1 = sqrt(pow(g1[0](i, j, k), 2) + pow(g1[1](i, j, k), 2) + pow(g1[2](i, j, k), 2));
        float mag2 = sqrt(pow(g2[0](i, j, k), 2) + pow(g2[1](i, j, k), 2) + pow(g2[2](i, j, k), 2));
        float dotP = ((g1[0](i, j, k) * g2[0](i, j, k)) + (g1[1](i, j, k) * g2[1](i, j, k))) / (mag1 * mag2);
        alpha(i, j, k) = acos(dotP) / M_PI;
      }
    }
  }
  return alpha;
}
