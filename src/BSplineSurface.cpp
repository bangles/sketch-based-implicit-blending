#include "BSplineSurface.hpp"

#include <stdexcept>

using namespace ebib;


BSplineSurface::BSplineSurface(){
    
}

BSplineSurface::~BSplineSurface(){
    
}

float BSplineSurface::bSplineBasis(float U[], int o, int i, float u, int num_samples, int max_value)
{
    float n = 0;
    if (o == 1)
    {
        if (u >= U[i] && u < U[i + 1])
            n = 1;
        else if (u == U[i+1] && u == max_value)
            n = 1;
        else
            n = 0;
    }
    else
    {
        float n1 = bSplineBasis(U, o - 1, i, u, num_samples,max_value);
        float n2 = bSplineBasis(U, o - 1, i + 1, u, num_samples,max_value);
        
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


std::vector<glm::vec3> BSplineSurface::evaluateSurface(int k, std::vector<glm::vec3> points, int noOfPoints, int num_samples) {
    
    float x[NUM_SAMPLES] = {};
    
    float *U = new float[noOfPoints + k];
    int max_value = noOfPoints - k + 1;
    
    for (int i = 0; i < noOfPoints + k; i++)
    {
        U[i] = 0;
        if (i < k)
        {
            U[i] = 0;
        }
        else if (i >= noOfPoints)
        {
            U[i] = max_value;
        }
        else
        {
            U[i] = i - k + 1;
        }
    }
    
    for (int i = 0; i < num_samples; i++)
    {
        x[i] = (i / (float)(num_samples-1)) * (noOfPoints - k + 1);
        
    }
    
    std::vector<glm::vec3> vertices(num_samples * num_samples);
    for (int a = 0; a < num_samples; a++)
    {
        for (int b = 0; b < num_samples; b++)
        {
            for (int i = 0; i < noOfPoints; i++)
            {
                glm::vec3 p2;
                for (int j = 0; j < noOfPoints; j++)
                {
                    float basis_1 = bSplineBasis(U, k, j, x[b], num_samples, max_value);
                    p2 += basis_1 * points[noOfPoints*i + j];
                }
                float basis_2 = bSplineBasis(U, k, i, x[a], num_samples, max_value);
                vertices[(b + num_samples * a)] += basis_2 * p2;
            }
        }
    }
    return vertices;
}

