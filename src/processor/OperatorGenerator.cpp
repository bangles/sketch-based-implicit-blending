#include "OperatorGenerator.h"
#include "Utils.h"
#include <Eigen/IterativeLinearSolvers>
#include <unsupported/Eigen/SparseExtra>

#define LOG(x) std::cout << x << std::endl

OperatorGenerator::OperatorGenerator(Template *in_template) { m_template = in_template; }

Tensor3f OperatorGenerator::generateOperator(int S) {
  MatrixXf X = RowVectorXf::LinSpaced(S, 0, 1).replicate(S, 1);
  MatrixXf Y = VectorXf::LinSpaced(S, 0, 1).replicate(1, S);

  VectorXf V = findVs(S);

  int sampleCount = 501;

  Tensor3f G, G1(S, S, S), G2(S, S, S);
  Tensor3i mask, mask1(S, S, S), mask2(S, S, S);
  Tensor3i Gs, Gs1(S, S, S), Gs2(S, S, S);

  fillGrid(S, sampleCount, -1, X, Y, V, G1, mask1, Gs1);
  fillGrid(S, sampleCount, 1, X, Y, V, G2, mask2, Gs2);

  Gs = Gs1;
  G = G1;
  mask = mask1;

  for (int i = 0; i < S; i++) {
    for (int j = 0; j < S; j++) {
      for (int k = 0; k < S; k++) {
        mask(i, j, k) = mask1(i, j, k) | mask2(i, j, k);
        int maskB = mask1(i, j, k) & mask2(i, j, k);

        if (mask2(i, j, k))
          Gs(i, j, k) = Gs2(i, j, k);
        if (maskB)
          Gs(i, j, k) = max(Gs1(i, j, k), Gs2(i, j, k));

        G(i, j, k) = min(G1(i, j, k), G2(i, j, k));

        if (mask(i, j, k))
          G(i, j, k) = Gs(i, j, k) * G(i, j, k) + 0.5;
        else
          G(i, j, k) = 0;

        // Boundary conditions
        if (i == 0) {
          G(i, j, k) = 1.0 / (S - 1) * j;
          mask(i, j, k) = 1;
        }

        if (j == 0) {
          G(i, j, k) = 1.0 / (S - 1) * i;
          mask(i, j, k) = 1;
        }

        if (i == (S - 1) || j == (S - 1)) {
          G(i, j, k) = 1;
          mask(i, j, k) = 1;
        }

        if (i == 1 && j > 0 && j < (S - 1)) {
          G(i, j, k) = 1.0 / (S - 1) * j;
          mask(i, j, k) = 1;
        }

        if (j == 1 && i > 0 && i < (S - 1)) {
          G(i, j, k) = 1.0 / (S - 1) * i;
          mask(i, j, k) = 1;
        }

        if (i == S - 2 && j > 0 && j < (S - 1)) {
          G(i, j, k) = 1.0 / (S - 1) * (S - 2);
          mask(i, j, k) = 1;
        }

        if (j == S - 2 && i > 0 && i < (S - 1)) {
          G(i, j, k) = 1.0 / (S - 1) * (S - 2);
          mask(i, j, k) = 1;
        }
      }
    }
  }

  //  LOG("Starting solve");
  //  long int before = mach_absolute_time();
//  solve(G, mask, S);
  fillOnes(G, mask, S);

  //  long int after = mach_absolute_time();
  //  int elapsed = ((after - before) * 100) / (1000 * 1000 * 1000);

  //  LOG("solve ended, Time taken " << elapsed / 100.0);
  return G;
}

void OperatorGenerator::fillOnes(Tensor3f &G, Tensor3i &mask, int S) {
  for (int k = 0; k < S; k++) {
    for (int i = 0; i < S; i++) {
      for (int j = S - 1; j >= 0; j--) {
        if (!mask(i, j, k)) {
          G(i, j, k) = 1;
        } else {
          break;
        }
      }
    }

    for (int i = 0; i < S; i++) {
      for (int j = S - 1; j >= 0; j--) {
        if (!mask(j, i, k)) {
          G(j, i, k) = 1;
        } else {
          break;
        }
      }
    }
  }

  VectorXi imid;
  VectorXf r = VectorXf::LinSpaced(S, 0, 1);

  if (S % 2 == 0) {
    imid.resize(4);
    imid << -2, -1, 0, 1;
    imid = imid.array() + S / 2;
  } else {
    imid.resize(5);
    imid << -3, -2, 1, 0, 1;
    imid = imid.array() + (S + 1) / 2;
  }

  for (int k = 0; k < S; k++) {
    for (int a = 0; a < imid.size(); a++) {
      int imid_a = imid[a];

      G(0, imid_a, k) = r[imid_a];
      G(1, imid_a, k) = r[imid_a];

      G(imid_a, 0, k) = r[imid_a];
      G(imid_a, 1, k) = r[imid_a];
    }
  }
}

void OperatorGenerator::solve(Tensor3f &G, Tensor3i &mask, int S) {
  const int neighborhood[13] = {0, 1, -1, S, -S, -S - 1, S - 1, S + 1, -S + 1, 2, -2, 2 * S, -2 * S};
  const double kernel[13] = {20.0, -8.0, -8.0, -8.0, -8.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0};

  VectorXf b(S * S);
  VectorXf x(S * S);

  SpMat L(S * S, S * S);
  SparseLU<SpMat> solver;
  //  LeastSquaresConjugateGradient<SpMat> solver;
  //    ConjugateGradient<SpMat> solver;
  //    BiCGSTAB<SpMat> solver;

  vector<ETriplet> tripletList;

  for (int i = 0; i < S; i++) {

    MatrixXf G_slice = Utils::slice(G, 2, i);
    MatrixXi mask_slice = Utils::slice(mask, 2, i);

    for (int j = 0; j < S; j++) {
      for (int k = 0; k < S; k++) {
        int index = k + S * j;

        if (mask_slice(j, k)) {
          tripletList.push_back(ETriplet(index, index, 1.0));
          b[index] = G_slice(j, k);
        } else {
          for (int a = 0; a < 13; a++) {
            tripletList.push_back(ETriplet(index, index + neighborhood[a], kernel[a]));
          }
          b[index] = 0.0;
        }
      }
    }

    //    LOG(i);

    //    solver.compute(L);
    L.makeCompressed();
    L.setFromTriplets(tripletList.begin(), tripletList.end());
    solver.compute(L);
    x = solver.solve(b);

    //    Eigen::saveMarket(L, "filename.mtx");
    //    Eigen::saveMarket(L, "filename_SPD.mtx", Eigen::Symmetric); // if A is symmetric-positive-definite
    //    Eigen::saveMarketVector(b, "filename_b.mtx");

    for (int j = 0; j < S; j++) {
      for (int k = 0; k < S; k++) {
        G(j, k, i) = x(k + S * j);
      }
    }

    tripletList.clear();
  }
}

void OperatorGenerator::fillGrid(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, VectorXf V, Tensor3f &G, Tensor3i &mask, Tensor3i &Gs) {

  for (int i = 0; i < S; i++) {
    MatrixXf in_G = MatrixXf::Ones(S, S) * numeric_limits<float>::infinity();
    MatrixXi in_mask = MatrixXi::Zero(S, S);
    MatrixXi in_Gs = MatrixXi::Zero(S, S);

    fillSlice(S, sampleCount, Nfactor, X, Y, V[i], in_G, in_mask, in_Gs);

    for (int j = 0; j < S; j++) {
      for (int k = 0; k < S; k++) {
        G(j, k, i) = in_G(k, j);
        Gs(j, k, i) = in_Gs(k, j);
        mask(j, k, i) = in_mask(k, j);
      }
    }
  }
}

void OperatorGenerator::fillSlice(int S, int sampleCount, int Nfactor, MatrixXf X, MatrixXf Y, float v, MatrixXf &out_G, MatrixXi &out_mask, MatrixXi &out_Gs) {
  MatrixXf sp = MatrixXf::Zero(sampleCount, 3);
  VectorXf sv = VectorXf::LinSpaced(sampleCount, 0, 1);
  bool isPatch1 = Nfactor == -1;

  for (int i = 0; i < sampleCount; i++) {
    Vector3f point(0, 0, 0);
    m_template->evaluate(sv(i), v, isPatch1, point);
    sp.row(i) = point;
  }

  MatrixXf sp_2d = sp.block(0, 0, sampleCount, 2);

  MatrixXf sn(sampleCount, 2);
  sn.block(0, 0, sampleCount - 1, 2) = (sp.block(1, 0, sampleCount - 1, 2) - sp.block(0, 0, sampleCount - 1, 2)) * Nfactor;
  sn.row(sampleCount - 1) = (sp_2d.row(sampleCount - 1) - sp_2d.row(sampleCount - 2)) * Nfactor;

  MatrixXf sn2(sn.rows(), sn.cols());

  sn2.col(0) = -sn.col(1);
  sn2.col(1) = sn.col(0);

  sn2 = sn2.array() / sn2.array().pow(2).rowwise().sum().cwiseSqrt().replicate(1, 2);

  Vector4i neighKernel;
  neighKernel << -1, 0, 1, 2;

  for (int k = 0; k < sampleCount; k++) {
    Vector2f p = sp_2d.row(k);
    Vector2f n = sn2.row(k);

    //      TODO : Implement Hard Constraints and remove this
    //        p[0] = roundf(p[0] * 1000000) / 1000000.0;
    //        p[1] = roundf(p[1] * 1000000) / 1000000.0;

    Vector2i pc;
    Vector2f tmp = p.reverse() * (S - 1);
    igl::floor(tmp, pc);

    Vector4i nkx = neighKernel + Vector4i::Ones(4) * pc[0];
    Vector4i nky = neighKernel + Vector4i::Ones(4) * pc[1];

    for (int a = 0; a < 4; a++) {
      for (int b = 0; b < 4; b++) {
        Vector2f pn;
        pn << nkx(a), nky(b);

        if (pn[0]<0 | pn[1]<0 | pn[0]> S - 1 | pn[1]> S - 1) {
          continue;
        }

        Vector2f pg;
        pg << X(pn[0], pn[1]), Y(pn[0], pn[1]);
        Vector2f diff = pg - p;
        float dist = diff.norm();

        if (out_mask(pn[0], pn[1]) == 0 || out_G(pn[0], pn[1]) > dist) { //|| out_G(pn[0],pn[1]) < 0) {
          out_G(pn[0], pn[1]) = dist;
          out_mask(pn[0], pn[1]) = 1;
          out_Gs(pn[0], pn[1]) = n.dot(diff) > 0 ? 1 : n.dot(diff) < 0 ? -1 : 1;
        }
      }
    }
  }
}

VectorXf OperatorGenerator::findVs(int res) {
  int sampleSize = 1000;
  VectorXf t = VectorXf::LinSpaced(res, 0, 1);
  VectorXf ts = VectorXf::LinSpaced(sampleSize, 0, 1);

  MatrixXf sp = MatrixXf::Zero(sampleSize, 3);

  for (int i = 0; i < sampleSize; i++) {
    Vector3f point(0, 0, 0);
    m_template->evaluate(0, ts[i], false, point);
    sp.row(i) = point;
  }

  VectorXf spz = sp.col(2);
  VectorXf V = VectorXf::Zero(res);
  V[0] = 0.0;
  V[res - 1] = 1.0;

  int k = 1;

  for (int i = 0; i < sampleSize - 1; i++) {
    float a = spz[i];
    float b = spz[i + 1];
    float c = t[k];

    if (c >= a && c <= b) {
      float alpha = (c - a) / (b - a);
      V[k] = (1 - alpha) * ts[i] + alpha * ts[i + 1];
      k = k + 1;
    }
  }
  return V;
}

void OperatorGenerator::print(MatrixXf matrix) { LOG(matrix); }

void OperatorGenerator::print(MatrixXi matrix) { LOG(matrix); }

void OperatorGenerator::print(vector<MatrixXf> matrix) {
  if (matrix.size() < 1)
    return;

  int s1 = matrix.size();
  int s2 = matrix[0].rows();
  int s3 = matrix[0].cols();

  for (int i = 0; i < s3; i++) {
    std::cout << "\nmatrix(:,;," << i << ")\n\n";
    for (int j = 0; j < s1; j++) {
      for (int k = 0; k < s2; k++) {
        std::cout << matrix[j](k, i) << '\t';
      }
      std::cout << '\n';
    }
  }
}

void OperatorGenerator::print(vector<MatrixXi> matrix) {
  if (matrix.size() < 1)
    return;

  int s1 = matrix.size();
  int s2 = matrix[0].rows();
  int s3 = matrix[0].cols();

  for (int i = 0; i < s3; i++) {
    std::cout << "\nmatrix(:,;," << i << ")\n\n";
    for (int j = 0; j < s1; j++) {
      for (int k = 0; k < s2; k++) {
        std::cout << matrix[j](k, i) << '\t';
      }
      std::cout << '\n';
    }
  }
}
