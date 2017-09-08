#include "BSplineSurface.hpp"
#include <iostream>

using namespace ebib;

#define LOG(x) std::cout << x << std::endl

BSplineSurface::BSplineSurface(Patch& patch, int order, int noOfPoints) {
    _patch = &patch;
    k = order;
    numPoints = noOfPoints;
    U = new float[noOfPoints + k];
    
    for (int i = 0; i < noOfPoints + k; i++)
    {
        U[i] = 0;
        if (i < k)
        {
            U[i] = 0;
        }
        else if (i >= noOfPoints)
        {
            U[i] = 1;
        }
        else
        {
            U[i] = (float)(i - k + 1) / (noOfPoints - k + 1);
        }
    }
    
    evaluateSurface();
}

BSplineSurface::~BSplineSurface(){
    
}

float BSplineSurface::bSplineBasis(int i, int o, float u)
{
    float n = 0;
    if (o == 1)
    {
        if (u >= U[i] && u < U[i + 1])
            n = 1;
        else if (u == U[i+1] && u == 1)
            n = 1;
        else
            n = 0;
    }
    else
    {
        float n1 = bSplineBasis(i, o - 1, u);
        float n2 = bSplineBasis(i + 1, o - 1, u);
        
        float nume1 = n1 * (u - U[i]);
        float deno1 = U[i + o - 1] - U[i];
        float nume2 = n2 * (U[i + o] - u);
        float deno2 = U[i + o] - U[i + 1];
        
        if (deno1 == 0)
        {
            n = (nume2 / deno2);
        }
        else
        {
            if (deno2 == 0)
                n = (nume1 / deno1);
            else
                n = (nume1 / deno1) + (nume2 / deno2);
        }
    }
    return n;
}


void BSplineSurface::evaluateSurface() {
    float x[NUM_SAMPLES] = {};
    
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        x[i] = (i / (float)(NUM_SAMPLES-1));
    }
    
    for (int a = 0; a < NUM_SAMPLES; a++)
    {
        for (int b = 0; b < NUM_SAMPLES; b++)
        {
            RowVectorXf weight(numPoints * numPoints);
            Vector3f point(0,0,0);
            evaluate(x[a], x[b], point, weight);
            
            _patch->vertices.col(a * NUM_SAMPLES + b) = point;
            _patch->weights.row(a + NUM_SAMPLES * b) = weight;
        }
    }
}

void BSplineSurface::evaluate(float u, float v, Vector3f& point, RowVectorXf& weight){
    for (int i = 0; i < numPoints; i++)
    {
        float basis_1 = bSplineBasis(i, k, v);
        for (int j = 0; j < numPoints; j++)
        {
            float basis_2 = bSplineBasis(j, k, u);
            if(weight.size() != 0) {
                weight(j * numPoints + i) = basis_1 * basis_2;
            }
            point += basis_1 * basis_2 * _patch->points.col(numPoints * i + j);
        }
    }
    
}
