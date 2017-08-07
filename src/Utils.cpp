//
//  Utils.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 04/08/17.
//
//

#include <Utils.h>
#include <fstream>

using namespace ebib;
using namespace Eigen;

const IOFormat Utils::CSVFormat = IOFormat(StreamPrecision, DontAlignCols,", ","\n");

bool Utils::writeToCSVfile(string name, MatrixXf matrix)
{
    ofstream file(name.c_str());
    file << matrix.format(CSVFormat);
    
    return true;
}

