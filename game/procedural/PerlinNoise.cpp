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

    /*normalizeFirstOctaveNoise(0.f, 30.f);*/

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
    float mini = 1000.f;
    float maxi = -1000.f;
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

        float currentSampleNoise = computeSampleNoise(leftNodePosition, leftNodePosition + nodeStep, gradients[leftGradientIndex], gradients[leftGradientIndex + 1], position);

        noise[i].x = noise[i].x + currentAmplitude * currentSampleNoise;

        if(currentSampleNoise > maxi)
            maxi = currentSampleNoise;
        else if(currentSampleNoise < mini)
            mini = currentSampleNoise;
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

    return std::clamp((1 - tSmooth) * dotLeft + tSmooth * dotRight, -1.0f, 1.0f);

}

void PerlinNoise::normalizeFirstOctaveNoise(float minValue, float maxValue)
{
    float min_x = noise[0].x;
    float max_x = noise[0].x;

    for (const auto& point : noise) {
        min_x = std::min(min_x, point.x);
        max_x = std::max(max_x, point.x);
    }

    for (auto& point : noise) {
        float normalized_x = minValue + ((point.x - min_x) * (maxValue - minValue)) / (max_x - min_x);
        point.x = normalized_x;
    }
}
