#include "PerlinNoise.h"
#include <box2d/math_functions.h>

std::vector<b2Vec2> PerlinNoise::generate(unsigned int seed)
{
    this->seed = seed;

    noise.clear();
    //dau push in vectorul rezultat la coordonata de pe axa OY unde se afla sample-urile
    float sampleStep = std::abs(domain.x - domain.y) / sampleCount;
    for(int i = 1; i <= sampleCount; i++)
        //noise-ul pt zid e pe axa OX
        noise.push_back((b2Vec2){0.f, sampleStep * i});

    for(int i = 1; i <= octavesCount; i++)
        generateOctaveNoise(i);

    return this->noise;
}

std::vector<int> PerlinNoise::generateGradients(unsigned int gradientsCount)
{
    std::vector<int> gradients;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 1);

    for (int i = 0; i < gradientsCount; ++i) {
        int gradient = distrib(gen);  
        gradients.push_back(gradient == 0 ? -1 : 1);  
    }
    return gradients;
}

void PerlinNoise::generateOctaveNoise(int currentOctave)
{
    int currentNodeCount = nodeCount * std::pow(2, currentOctave);
    float currentAmplitude = amplitude * std::pow(persistance, currentOctave);
    if(currentOctave == 1)
    {
        currentNodeCount = nodeCount;
        currentAmplitude = amplitude;
    }
    std::vector<int> gradients = generateGradients(currentNodeCount);
    float nodeStep = std::abs(domain.x - domain.y) / currentNodeCount;
    float sampleStep = std::abs(domain.x - domain.y) / sampleCount;

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

        noise[i].x = noise[i].x + currentAmplitude * computeSampleNoise(leftNodePosition, leftNodePosition + nodeStep, gradients[leftGradientIndex], gradients[leftGradientIndex + 1], position);
    }
}


float PerlinNoise::computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample)
{
    float dLeft = sample - leftNode;
    float dRight = sample - rightNode;

    float dotLeft = leftGradient * dLeft;
    float dotRight = rightGradient * dRight;

    float t = (sample - leftNode) / (rightNode - leftNode);

    float tSmooth = t * t * t * (10 - 15 * t + 6 * t * t);

    return  (1 - tSmooth) * dotLeft + tSmooth * dotRight;

}
