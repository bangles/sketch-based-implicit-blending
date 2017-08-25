//
//  Utils.h
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 04/08/17.
//
//

#ifndef Utils_h
#define Utils_h

#include <Eigen/Dense>

namespace ebib {
    using namespace std;
    using namespace Eigen;
    
    class Utils {
    private:
        const static IOFormat CSVFormat;
    public:
        static bool writeMatrixXiToCSVfile(string name, MatrixXi& matrix);
        static bool writeMatrixXfToCSVfile(string name, MatrixXf& matrix);
    };
}


#endif /* Utils_h */
