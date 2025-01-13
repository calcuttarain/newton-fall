#include "PerlinNoise.h"
#include <box2d/math_functions.h>

std::vector<b2Vec2> PerlinNoise::generate(unsigned int seed)
{
    this->seed = seed;

    noise.clear();
    float sampleStep = std::abs(domain.x - domain.y) / sampleCount;
    for(int i = 1; i <= sampleCount; i++)
        noise.push_back((b2Vec2){0.f, sampleStep * i});

    for(int i = 0; i < octavesCount; i++)
        generateOctaveNoise(i);

    return this->noise;
}

void PerlinNoise::generateOctaveNoise(int currentOctave)
{
    int currentNodeCount = nodeCount * std::pow(2, currentOctave);
    float currentAmplitude = amplitude * std::pow(persistance, currentOctave);

    std::vector<int> gradients = generateGradients(currentNodeCount);
    float nodeStep = std::abs(domain.x - domain.y) / currentNodeCount;

    // Vector temporar pentru stocarea noise-ului curent
    std::vector<float> currentNoise(noise.size());
    float mini = 1000.f;
    float maxi = -1000.f;

    // Generăm noise-ul pentru octava curentă
    float leftNodePosition = 0.f;
    int leftGradientIndex = 0;
    for(int i = 0; i < noise.size(); i++)
    {
        float position = noise[i].y;

        while(position > leftNodePosition + nodeStep)
        {
            leftNodePosition += nodeStep;
            leftGradientIndex++;
        }

        float sampleNoise = computeSampleNoise(leftNodePosition, leftNodePosition + nodeStep, 
                                             gradients[leftGradientIndex], gradients[leftGradientIndex + 1], 
                                             position);
        
        currentNoise[i] = sampleNoise;
        if(sampleNoise > maxi) maxi = sampleNoise;
        if(sampleNoise < mini) mini = sampleNoise;
    }

    // Normalizăm și aplicăm noise-ul pentru fiecare sample
    float range = maxi - mini;
    for(int i = 0; i < noise.size(); i++) {
        float normalizedNoise = 2.0f * ((currentNoise[i] - mini) / range) - 1.0f;
        noise[i].x += normalizedNoise * currentAmplitude;
    }
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

float PerlinNoise::computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample)
{
    float dLeft = sample - leftNode;
    float dRight = sample - rightNode;

    float dotLeft = leftGradient * dLeft;
    float dotRight = rightGradient * dRight;

    float t = (sample - leftNode) / (rightNode - leftNode);
    float tSmooth = t * t * (3 - 2 * t);

    return (1 - tSmooth) * dotLeft + tSmooth * dotRight;  // Am eliminat clamp
}
