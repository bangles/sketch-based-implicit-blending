//  RegistrationProcessor.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 18/07/17.

#include "RegistrationProcessor.hpp"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

RegistrationProcessor::RegistrationProcessor(Template *inTemplate) {
  m_template = inTemplate;
  iter = 0;

  _points.resize(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS * 2);
  _vertices.resize(m_template->mPatches[0].vertices.rows(), m_template->mPatches[0].vertices.cols() * 2);
  _triangles.resize(m_template->mPatches[0].triangles.rows(), m_template->mPatches[0].triangles.cols() * 2);
  _triangles << m_template->mPatches[0].triangles, m_template->mPatches[1].triangles;
  _weights.resize(m_template->mPatches[0].weights.rows() * 2, m_template->mPatches[0].weights.cols() * 2);
  zeroWeights = MatrixXf::Zero(m_template->mPatches[0].weights.rows(), m_template->mPatches[0].weights.cols());
  updateSearcher();

  initialize();
}

void RegistrationProcessor::initialize(){
    MC = mergedDOFs();
    R = MatrixXf::Identity(_points.size(), _points.size());

    if (MC.size() != 0) {
      for (int i = 0; i < MC.rows(); i++) {
        R(MC(i, 1), MC(i, 0)) = 1;
      }

      removeColumns(R, MC.col(1));
    }

    laplacianSliceEnergy(A1, b1);
    laplacianLineEnergy(A2, b2);
    fixedPointEnergy(A4, b4);
    spineSmoothEnergy(A6, b6);
}

void RegistrationProcessor::updateSearcher() {
  _points << m_template->mPatches[0].points, m_template->mPatches[1].points;
  _vertices << m_template->mPatches[0].vertices, m_template->mPatches[1].vertices;
  _weights << m_template->mPatches[0].weights, zeroWeights, zeroWeights, m_template->mPatches[1].weights;
  searcher = new TrimeshSearcher<MatrixXf, MatrixXi>();
  searcher->build(_vertices, _triangles);
}

void RegistrationProcessor::registerPoints(MatrixXf inQueries) {
  //  while (iter < 10) {
  MatrixXf x = step(inQueries);
  Map<Matrix<float, Dynamic, Dynamic, RowMajor>> solution(x.data(), 3, x.size() / 3);

  m_template->mPatches[0].points = solution.leftCols(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
  m_template->mPatches[1].points = solution.rightCols(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
  updateSearcher();
  iter++;
  //  }
  //  iter = 0;
}

MatrixXf RegistrationProcessor::step(MatrixXf inQueries) {
  pointToPlaneEnergy(A0, b0, inQueries);
  tikhonovEnergy(A3, b3);

  int n = inQueries.cols();
  VectorXf w = VectorXf::Zero(7);
  w(0) = 1.0 / n;
  w(1) = max(1000 * pow(0.05f, iter), pow(10, -3)); // Slice smoothness
  w(2) = max(1000 * pow(0.05f, iter), pow(10, -3)); // Line smoothness
  w(3) = pow(10, -3);                               // Tikhonov
  w(4) = pow(10, 1);                                // Fixed point energy
  w(6) = pow(10, -1);                               // Spine Smoothness

  w = w.cwiseSqrt();

  int rowsA = A0.rows() + A1.rows() + A2.rows() + A3.rows() + A4.rows() + A6.rows();
  int rowsB = b0.rows() + b1.rows() + b2.rows() + b3.rows() + b4.rows() + b6.rows();

  MatrixXf A(rowsA, A0.cols());
  A << A0 * w[0], A1 * w[1], A2 * w[2], A3 * w[3], A4 * w[4], A6 * w[6];

  MatrixXf b(rowsB, b0.cols());
  b << b0 * w[0], b1 * w[1], b2 * w[2], b3 * w[3], b4 * w[4], b6 * w[6];

  A = A * R;
  MatrixXf x = A.colPivHouseholderQr().solve(b);
  x = R * x;

  return x;
}

void RegistrationProcessor::removeColumns(MatrixXf &R, VectorXi columns) {
  VectorXi output, indexes;
  igl::unique(columns, output);
  columns = output;
  igl::sort(columns, 1, false, output, indexes);

  std::vector<VectorXf> A;
  for (int i = 0; i < R.cols(); i++) {
    A.push_back(R.col(i));
  }

  for (int i = 0; i < columns.size(); i++) {
    A.erase(A.begin() + output(i));
  }

  R.resize(R.rows(), A.size());

  for (int i = 0; i < A.size(); i++) {
    R.col(i) = A[i];
  }
}

MatrixXi RegistrationProcessor::mergedDOFs() {
  int s = _points.cols();
  MatrixXi MC(170, 2);

  VectorXi s1 = VectorXi::Ones(NUM_CONTROL_POINTS) * s;
  RowVectorXi s2 = RowVectorXi::Ones(NUM_CONTROL_POINTS * 2) * s;
  int end = NUM_CONTROL_POINTS - 1;
  int lastRow = 0;

  push(MC, lastRow, m_template->meshInfo.spine0, m_template->meshInfo.spine1);
  push(MC, lastRow, m_template->meshInfo.spine0 + s1, m_template->meshInfo.spine1 + s1);
  push(MC, lastRow, m_template->meshInfo.spine0 + 2 * s1, m_template->meshInfo.spine1 + 2 * s1);

  push(MC, lastRow, m_template->meshInfo.line0, m_template->meshInfo.line0o);
  push(MC, lastRow, m_template->meshInfo.line0 + 2 * s1, m_template->meshInfo.line0o + 2 * s1);

  push(MC, lastRow, m_template->meshInfo.line1 + s1, m_template->meshInfo.line1o + s1);
  push(MC, lastRow, m_template->meshInfo.line1 + 2 * s1, m_template->meshInfo.line1o + 2 * s1);

  push(MC, lastRow, m_template->meshInfo.slices.row(0), m_template->meshInfo.slices.row(1));
  push(MC, lastRow, m_template->meshInfo.slices.row(0) + s2, m_template->meshInfo.slices.row(1) + s2);

  push(MC, lastRow, m_template->meshInfo.slices.row(end), m_template->meshInfo.slices.row(end - 1));
  push(MC, lastRow, m_template->meshInfo.slices.row(end) + s2, m_template->meshInfo.slices.row(end - 1) + s2);

  for (int i = 0; i < NUM_CONTROL_POINTS; i++) {
    for (int j = 1; j < 2 * NUM_CONTROL_POINTS; j++) {
      push(MC, lastRow, m_template->meshInfo.slices(i, 0) + 2 * s, m_template->meshInfo.slices(i, j) + 2 * s);
    }

    for (int j = 0; j < NUM_CONTROL_POINTS; j++) {
      push(MC, lastRow, m_template->meshInfo.slices(i, j), m_template->meshInfo.slices(i, NUM_CONTROL_POINTS + j) + s);
      push(MC, lastRow, m_template->meshInfo.slices(i, j) + s, m_template->meshInfo.slices(i, NUM_CONTROL_POINTS + j));
    }
  }

  VectorXi indexes, output;
  igl::sort(MC.col(1), 1, true, output, indexes);
  std::vector<VectorXi> A = convertToArray(MC, indexes);
  std::vector<int> duplicateIndexes;

  for (int i = 0; i < A.size(); i++) {
    duplicateIndexes.clear();

    for (int j = 0; j < A.size(); j++) {
      if (j == i)
        continue;

      igl::ismember(A[i], A[j], indexes, output);

      if (indexes.array().any()) {
        VectorXi temp(A[i].size() + A[j].size());
        temp << A[i], A[j];
        A[i] = temp;
        duplicateIndexes.push_back(j);
      }
    }

    for (int j = duplicateIndexes.size() - 1; j >= 0; j--) {
      A.erase(A.begin() + duplicateIndexes[j]);
    }
  }

  duplicateIndexes.clear();
  MC.resize(124, 2);

  lastRow = 0;
  for (int i = 0; i < A.size(); i++) {
    igl::unique(A[i], output);
    for (int j = 1; j < output.size(); j++) {
      MC.row(lastRow++) << output[0], output[j];
    }
  }

  return MC;
}

std::vector<VectorXi> RegistrationProcessor::convertToArray(MatrixXi &MC, VectorXi &IX) {
  std::vector<VectorXi> A;
  for (int i = 0; i < IX.size(); i++) {
    A.push_back(MC.row(IX(i)));
  }
  return A;
}

void RegistrationProcessor::push(MatrixXi &MC, int &lastRow, VectorXi a, VectorXi b) {
  int count = a.size();
  MC.block(lastRow, 0, count, 2) << a, b;
  lastRow += count;
}

void RegistrationProcessor::push(MatrixXi &MC, int &lastRow, int a, int b) {
  MC.block(lastRow, 0, 1, 2) << a, b;
  lastRow++;
}

void RegistrationProcessor::pointToPlaneEnergy(MatrixXf &A, VectorXf &b, MatrixXf inQueries) {
  MatrixXf footpoints(3, inQueries.cols()), barycentric(3, inQueries.cols());
  VectorXi findex(inQueries.cols());

  searcher->closest_point(inQueries, footpoints, findex);
  searcher->barycentric(footpoints, findex, barycentric);

  A.resize(inQueries.cols(), _points.cols() * 3);
  b.resize(inQueries.cols());

  for (int i = 0; i < inQueries.cols(); i++) {
    Vector3i vIdx = _triangles.col(findex[i]);

    Vector3f n = footpoints.col(i) - inQueries.col(i);
    n.normalize();
    VectorXf w = barycentric.col(i);

    MatrixXf Wbi(3, _points.cols());
    Wbi << _weights.row(vIdx[0]), _weights.row(vIdx[1]), _weights.row(vIdx[2]);

    VectorXf Wbw = w.transpose() * Wbi;

    VectorXf row(_points.cols() * 3);
    row << (Wbw * n[0]), (Wbw * n[1]), (Wbw * n[2]);
    A.row(i) = row;
    b[i] = inQueries.col(i).dot(n);
  }
}

void RegistrationProcessor::laplacianSliceEnergy(MatrixXf &A, VectorXf &b) {
  int count = 0;
  int size = _points.cols();
  int end = NUM_CONTROL_POINTS - 1;
  A = MatrixXf::Zero(NUM_CONTROL_POINTS * (NUM_CONTROL_POINTS - 2) * 6, size * 3);

  for (int i = 0; i < NUM_CONTROL_POINTS; i++)
    for (int j = 1; j < NUM_CONTROL_POINTS - 1; j++)
      for (int k = 0; k < 3; k++) {
        A(count, m_template->meshInfo.slices(i, j - 1) + k * size) = 1;
        A(count, m_template->meshInfo.slices(i, j) + k * size) = -2;
        A(count++, m_template->meshInfo.slices(i, j + 1) + k * size) = 1;

        A(count, m_template->meshInfo.slices(i, end - j - 1) + k * size) = 1;
        A(count, m_template->meshInfo.slices(i, end - j) + k * size) = -2;
        A(count++, m_template->meshInfo.slices(i, end - j + 1) + k * size) = 1;
      }

  b = MatrixXf::Zero(count, 1);
}

void RegistrationProcessor::laplacianLineEnergy(MatrixXf &A, VectorXf &b) {
  int count = 0;
  int size = _points.cols();
  A = MatrixXf::Zero((NUM_CONTROL_POINTS - 2) * 2 * NUM_CONTROL_POINTS * 3, size * 3);

  for (int i = 1; i < NUM_CONTROL_POINTS - 1; i++)
    for (int j = 0; j < 2 * NUM_CONTROL_POINTS; j++)
      for (int k = 0; k < 3; k++) {
        A(count, m_template->meshInfo.slices(i - 1, j) + k * size) = 1;
        A(count, m_template->meshInfo.slices(i, j) + k * size) = -2;
        A(count++, m_template->meshInfo.slices(i + 1, j) + k * size) = 1;
      }

  b = MatrixXf::Zero(count, 1);
}

void RegistrationProcessor::tikhonovEnergy(MatrixXf &A, VectorXf &b) {
  Matrix<float, Dynamic, Dynamic, RowMajor> Z(_points);
  b = Map<MatrixXf>(Z.data(), Z.size(), 1);
  A = MatrixXf::Identity(_points.size(), _points.size());
}

void RegistrationProcessor::spineSmoothEnergy(MatrixXf &A, VectorXf &b) {
  int size = _points.cols();
  A = MatrixXf::Zero(NUM_CONTROL_POINTS * 3, size * 3);
  b = MatrixXf::Zero(NUM_CONTROL_POINTS * 3, 1);

  for (int i = 0; i < NUM_CONTROL_POINTS; i++)
    for (int k = 0; k < 3; k++) {
      A(i + NUM_CONTROL_POINTS * k, m_template->meshInfo.slices(i, NUM_CONTROL_POINTS - 2) + k * size) = 1;
      A(i + NUM_CONTROL_POINTS * k, m_template->meshInfo.slices(i, NUM_CONTROL_POINTS - 1) + k * size) = -2;
      A(i + NUM_CONTROL_POINTS * k, m_template->meshInfo.slices(i, 2 * NUM_CONTROL_POINTS - 2) + k * size) = 1;
    }
}

void RegistrationProcessor::fixedPointEnergy(MatrixXf &A, VectorXf &b) {
  int size = _points.cols();
  A = MatrixXf::Zero(4 * NUM_CONTROL_POINTS + 4, _points.size());
  b = MatrixXf::Zero(4 * NUM_CONTROL_POINTS + 4, 1);

  for (int i = 0; i < NUM_CONTROL_POINTS; i++) {
    //      Line0 x and y
    A(i * 4, m_template->meshInfo.line0[i]) = 1;
    A(i * 4 + 1, m_template->meshInfo.line0[i] + size) = 1;
    b[i * 4] = 0.5;
    b[i * 4 + 1] = 0;

    //      Line1 x and y
    A(i * 4 + 2, m_template->meshInfo.line1[i]) = 1;
    A(i * 4 + 3, m_template->meshInfo.line1[i] + size) = 1;
    b[i * 4 + 2] = 0;
    b[i * 4 + 3] = 0.5;
  }

  //  Line0 z
  A(4 * NUM_CONTROL_POINTS, m_template->meshInfo.line0[0] + 2 * size) = 1;
  b[4 * NUM_CONTROL_POINTS] = 0;
  A(4 * NUM_CONTROL_POINTS + 1, m_template->meshInfo.line0[NUM_CONTROL_POINTS - 1] + 2 * size) = 1;
  b[4 * NUM_CONTROL_POINTS + 1] = 1;

  //  Line1 z
  A(4 * NUM_CONTROL_POINTS + 2, m_template->meshInfo.line1[0] + 2 * size) = 1;
  b[4 * NUM_CONTROL_POINTS + 2] = 0;
  A(4 * NUM_CONTROL_POINTS + 3, m_template->meshInfo.line1[NUM_CONTROL_POINTS - 1] + 2 * size) = 1;
  b[4 * NUM_CONTROL_POINTS + 3] = 1;
}
