//  Pipeline.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#include <Pipeline.hpp>
#define LOG(x) std::cout << x << std::endl
using namespace std;

typedef Triplet<double, int> ETriplet;
typedef SparseMatrix<double> SpMat;

Pipeline::Pipeline(QOpenGLShaderProgram *program) {
  m_template = new Template(program);
  m_regProcessor = new RegistrationProcessor(m_template);
  m_opGenerator = new OperatorGenerator(m_template);
  m_volGenerator = new VolumeGenerator();
  userPoints = new UserPoints(program);

  int S = 100;

  MatrixXf X = RowVectorXf::LinSpaced(S, MIN[0], MAX[0]).replicate(S, 1);
  MatrixXf Y = VectorXf::LinSpaced(S, MIN[1], MAX[1]).replicate(1, S);

  circle1 = new Circle(0, -0.35, 0.25, 0.6, X, Y);
  circle2 = new Circle(0, 0.35, 0.25, 0.6, X, Y);
  result = new Result(X, Y, program);
}

void Pipeline::registerPoints() {
  m_regProcessor->registerPoints(userPoints->m_userPoints);
  m_template->updatePatches();
}


void Pipeline::start() {
  vector<MatrixXf> G = m_opGenerator->generateOperator(30);
  MatrixXf alpha = calculateGradientAngles(circle1->gradient, circle2->gradient);
  MatrixXf distanceField = m_volGenerator->generate(circle1->distanceField, circle2->distanceField, alpha, G);
  result->setDistanceField(distanceField);
}

void Pipeline::mapSamplesToTemplate(MatrixXf samples) {
  //    sAlpha = gradAngle(gf{1}, gf{2});
  //    samples = [f{1} f{2} sAlpha];
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
