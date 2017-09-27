//  Pipeline.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 06/09/17.
//
//

#include <Pipeline.hpp>

#define LOG(x) std::cout<<x<<std::endl

using namespace std;

typedef Triplet<double, int> ETriplet;
typedef SparseMatrix<double> SpMat;

Pipeline::Pipeline(QOpenGLShaderProgram *program) {
    m_template = new Template(program);
    m_regProcessor = new RegistrationProcessor(m_template);
    m_opGenerator = new OperatorGenerator(m_template);
}

void Pipeline::start(MatrixXf userSamples) {
    m_regProcessor->registerPoints(userSamples);
    m_opGenerator->generateOperator(10);
}


