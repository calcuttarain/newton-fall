#pragma once

#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

class PerlinNoise {
private:
    unsigned int seed;
    b2Vec2 domain;
    unsigned int nodeCount;
    unsigned int sampleCount;

    std::vector<int> gradients;
    std::vector <b2Vec2> noise;

    void generateGradients();
    void generateNoise();
    float computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample);

public:
    PerlinNoise(unsigned int seed) : seed(seed) {}

    void generate(b2Vec2 domain, unsigned int nodeCount, unsigned int sampleCount);

    std::vector <b2Vec2> getNoise();
};
