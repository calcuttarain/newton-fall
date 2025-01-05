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

    //genereaza gradientii pentru fiecare nod din grid
    void generateGradients();

    //include logica generarii zgomotului pentru toate sample-urile
    void generateNoise();

    //logica generarii zgomotului intr-un singur sample
    float computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample);

public:
    PerlinNoise(b2Vec2 domain, unsigned int nodeCount, unsigned int sampleCount) : domain(domain), nodeCount(nodeCount), sampleCount(sampleCount) {}

    std::vector <b2Vec2> generate(unsigned int seed);
};
