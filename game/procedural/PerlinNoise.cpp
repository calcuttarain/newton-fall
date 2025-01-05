#include "PerlinNoise.h"
#include <box2d/math_functions.h>

void PerlinNoise::generate(b2Vec2 domain, unsigned int nodeCount, unsigned int sampleCount)
{
    this->domain = domain;
    this->nodeCount = nodeCount;
    this->sampleCount = sampleCount;

    generateGradients();
    
    generateNoise();
}

void PerlinNoise::generateGradients()
{
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 1);

    if(!gradients.empty())
        gradients.clear();

    for (int i = 0; i < nodeCount; ++i) {
        int gradient = distrib(gen);  
        //momentan doar valori pentru directie -1 si 1
        gradients.push_back(gradient == 0 ? -1 : 1);  
    }
}

void PerlinNoise::generateNoise()
{
    float nodeStep = std::abs(domain.x - domain.y) / nodeCount;
    float sampleStep = std::abs(domain.x - domain.y) / sampleCount;

    if(!noise.empty())
        noise.clear();

    //dau push in vectorul rezultat la coordonata de pe axa OY unde se afla sample-urile
    for(int i = 1; i <= sampleCount; i++)
        //noise-ul pt zid e pe axa OX
        noise.push_back((b2Vec2){0.f, sampleStep * i});

    //pentru fiecare sample calculez noise
    float leftNodePosition = 0.f;
    int leftGradientIndex = 0;
    for(int i = 0; i < noise.size(); i++)
    {
        float position = noise[i].y;

        //incadrez sample-ul intre doua noduri din grid
        while(position > leftNodePosition + nodeStep)
        {
            leftNodePosition += nodeStep;
            leftGradientIndex++;
        }

        noise[i].x = computeSampleNoise(leftNodePosition, leftNodePosition + nodeStep, gradients[leftGradientIndex], gradients[leftGradientIndex + 1], position);
    }
}


float PerlinNoise::computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample)
{
    float dLeft = sample - leftNode;
    float dRight = sample - rightNode;

    float dotLeft = leftGradient * dLeft;
    float dotRight = rightGradient * dRight;

    float t = (sample - leftNode) / (rightNode - leftNode);

    float tSmooth = t * t * (3 - 2 * t);

    return  (1 - tSmooth) * dotLeft + tSmooth * dotRight;

}

std::vector <b2Vec2> PerlinNoise::getNoise()
{
    return this->noise;
}
