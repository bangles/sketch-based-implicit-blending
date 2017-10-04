#include "MarchingSquares.h"

namespace {
Eigen::Vector2d lerpVertex(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, double v1, double v2, double isovalue) {
  double mu = (isovalue - v1) / (v2 - v1);
  return p1 + mu * (p2 - p1);
}

struct GridCell {
  Eigen::Vector2d vertex[4];
  double value[4];
};

void genGeometry(const GridCell &gridCell, std::vector<Eigen::Vector2d> &vertices, double isovalue, bool fill) {
  // Identify which of the 4 corners of the square are within the isosurface.
  // Max 16 cases. Only 14 cases produce triangles and image edge line segments.
  // Corner vertex order: 03
  //                      12
  // e.g.: clockwise, as in OpenGL

  unsigned short int mask = 0;

  if (gridCell.value[0] >= isovalue)
    mask |= 1;
  if (gridCell.value[1] >= isovalue)
    mask |= 2;
  if (gridCell.value[2] >= isovalue)
    mask |= 4;
  if (gridCell.value[3] >= isovalue)
    mask |= 8;

  switch (mask) {
  case 1: {
    // 1:
    // 10
    // 00
    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);
    } else {
      vertices.push_back(b);
      vertices.push_back(c);
    }
    break;
  }
  case 2: {
    // 2:
    // 00
    // 10
    Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);
    } else {
      vertices.push_back(a);
      vertices.push_back(c);
    }
    break;
  }
  case 4: {
    // 4:
    // 00
    // 01
    Eigen::Vector2d a = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
    Eigen::Vector2d b = gridCell.vertex[2];
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);
    } else {
      vertices.push_back(a);
      vertices.push_back(c);
    }
    break;
  }
  case 8: {
    // 8:
    // 01
    // 00
    Eigen::Vector2d a = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
    Eigen::Vector2d c = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);
    } else {
      vertices.push_back(a);
      vertices.push_back(b);
    }
    break;
  }

  case 3: {
    // 3:
    // 10
    // 10
    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(d);

      vertices.push_back(d);
      vertices.push_back(b);
      vertices.push_back(c);
    } else {
      vertices.push_back(d);
      vertices.push_back(c);
    }
    break;
  }
  case 6: {
    // 6:
    // 00
    // 11
    Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = gridCell.vertex[2];
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(d);

      vertices.push_back(d);
      vertices.push_back(b);
      vertices.push_back(c);

    } else {
      vertices.push_back(a);
      vertices.push_back(d);
    }
    break;
  }
  case 9: {
    // 9:
    // 11
    // 00
    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
    Eigen::Vector2d d = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(d);

      vertices.push_back(d);
      vertices.push_back(b);
      vertices.push_back(c);

    } else {
      vertices.push_back(b);
      vertices.push_back(c);
    }
    break;
  }
  case 12: {
    // 12:
    // 01
    // 01

    Eigen::Vector2d a = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
    Eigen::Vector2d c = gridCell.vertex[2];
    Eigen::Vector2d d = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(d);

      vertices.push_back(d);
      vertices.push_back(b);
      vertices.push_back(c);

    } else {
      vertices.push_back(a);
      vertices.push_back(b);
    }

    break;
  }

  case 5: {
    // 5:
    // 10
    // 01

    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
    Eigen::Vector2d e = gridCell.vertex[2];
    Eigen::Vector2d f = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);

      vertices.push_back(d);
      vertices.push_back(e);
      vertices.push_back(f);

    } else {
      vertices.push_back(b);
      vertices.push_back(c);
      vertices.push_back(d);
      vertices.push_back(f);
    }
    break;
  }
  case 10: {
    // 10:
    // 01
    // 10

    Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);
    Eigen::Vector2d e = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
    Eigen::Vector2d f = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);

      vertices.push_back(d);
      vertices.push_back(e);
      vertices.push_back(f);

    } else {
      vertices.push_back(a);
      vertices.push_back(c);
      vertices.push_back(d);
      vertices.push_back(e);
    }
    break;
  }

  case 7: {
    // 7:
    // 10
    // 11

    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = gridCell.vertex[2];
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[2], gridCell.vertex[3], gridCell.value[2], gridCell.value[3], isovalue);
    Eigen::Vector2d e = lerpVertex(gridCell.vertex[0], gridCell.vertex[3], gridCell.value[0], gridCell.value[3], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(e);

      vertices.push_back(e);
      vertices.push_back(b);
      vertices.push_back(d);

      vertices.push_back(d);
      vertices.push_back(b);
      vertices.push_back(c);

    } else {
      vertices.push_back(e);
      vertices.push_back(d);
    }
    break;
  }
  case 11: {
    // 11:
    // 11
    // 10

    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[1], gridCell.vertex[2], gridCell.value[1], gridCell.value[2], isovalue);
    Eigen::Vector2d d = lerpVertex(gridCell.vertex[3], gridCell.vertex[2], gridCell.value[3], gridCell.value[2], isovalue);
    Eigen::Vector2d e = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);

      vertices.push_back(a);
      vertices.push_back(c);
      vertices.push_back(d);

      vertices.push_back(a);
      vertices.push_back(d);
      vertices.push_back(e);

    } else {
      vertices.push_back(c);
      vertices.push_back(d);
    }
    break;
  }
  case 13: {
    // 13:
    // 11
    // 01

    Eigen::Vector2d a = gridCell.vertex[0];
    Eigen::Vector2d b = lerpVertex(gridCell.vertex[0], gridCell.vertex[1], gridCell.value[0], gridCell.value[1], isovalue);
    Eigen::Vector2d c = lerpVertex(gridCell.vertex[2], gridCell.vertex[1], gridCell.value[2], gridCell.value[1], isovalue);
    Eigen::Vector2d d = gridCell.vertex[2];
    Eigen::Vector2d e = gridCell.vertex[3];

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(e);

      vertices.push_back(e);
      vertices.push_back(b);
      vertices.push_back(c);

      vertices.push_back(e);
      vertices.push_back(c);
      vertices.push_back(d);

    } else {
      vertices.push_back(b);
      vertices.push_back(c);
    }
    break;
  }
  case 14: {
    // 14:
    // 01
    // 11

    Eigen::Vector2d a = lerpVertex(gridCell.vertex[1], gridCell.vertex[0], gridCell.value[1], gridCell.value[0], isovalue);
    Eigen::Vector2d b = gridCell.vertex[1];
    Eigen::Vector2d c = gridCell.vertex[2];
    Eigen::Vector2d d = gridCell.vertex[3];
    Eigen::Vector2d e = lerpVertex(gridCell.vertex[3], gridCell.vertex[0], gridCell.value[3], gridCell.value[0], isovalue);

    if (fill) {
      vertices.push_back(a);
      vertices.push_back(b);
      vertices.push_back(c);

      vertices.push_back(a);
      vertices.push_back(c);
      vertices.push_back(e);

      vertices.push_back(e);
      vertices.push_back(c);
      vertices.push_back(d);

    } else {
      vertices.push_back(a);
      vertices.push_back(e);
    }
    break;
  }

  case 15: {
    // Case 15 (all inside of image area) produces no outlines.

    // 15:
    // 11
    // 11

    if (fill) {

      vertices.push_back(gridCell.vertex[0]);
      vertices.push_back(gridCell.vertex[1]);
      vertices.push_back(gridCell.vertex[3]);

      vertices.push_back(gridCell.vertex[3]);
      vertices.push_back(gridCell.vertex[1]);
      vertices.push_back(gridCell.vertex[2]);
    }
    break;
  }

  default: {
    // Case 0 (all outside of image area) produces no geometry.

    // 0:
    // 00
    // 00

    break;
  }
  }
}

} // namespace

void marchingSquares(const Eigen::MatrixXf &X, const Eigen::MatrixXf &Y, const Eigen::MatrixXf &S, double isoValue, std::vector<Eigen::Vector2d> &vertices, bool fill) {
  for (int x = 0; x < X.rows() - 1; x++) {
    for (int y = 0; y < Y.rows() - 1; y++) {
      GridCell gc;
      gc.vertex[0] = Eigen::Vector2d(X(x, y), Y(x, y));
      gc.vertex[1] = Eigen::Vector2d(X(x + 1, y), Y(x + 1, y));
      gc.vertex[2] = Eigen::Vector2d(X(x + 1, y + 1), Y(x + 1, y + 1));
      gc.vertex[3] = Eigen::Vector2d(X(x, y + 1), Y(x, y + 1));

      gc.value[0] = S(x, y);
      gc.value[1] = S(x + 1, y);
      gc.value[2] = S(x + 1, y + 1);
      gc.value[3] = S(x, y + 1);

      genGeometry(gc, vertices, isoValue, fill);
    }
  }
}
