#pragma once

#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>

class PerlinNoise {
private:
    unsigned int seed;
    b2Vec2 domain;
    unsigned int nodeCount;
    unsigned int sampleCount;
    unsigned int octavesCount;
    float amplitude;
    float persistance;

    std::vector <b2Vec2> noise;

    //genereaza gradientii pentru fiecare nod din grid
    std::vector<int> generateGradients(unsigned int gradientsCount);

    //include logica generarii zgomotului pentru toate sample-urile
    void generateOctaveNoise(int currentOctave);

    //logica generarii zgomotului intr-un singur sample
    float computeSampleNoise(float leftNode, float rightNode, int leftGradient, int rightGradient, float sample);

public:
    //primeste parametrii care raman fixati. 
    PerlinNoise(b2Vec2 domain, unsigned int nodeCount, unsigned int sampleCount, unsigned int octavesCount, float amplitude, float persistance) : domain(domain), nodeCount(nodeCount), sampleCount(sampleCount), octavesCount(octavesCount), amplitude(amplitude), persistance(persistance)  {}

    //pentru un zid diferit, genereaza mapa diferita, dar cu parametrii fixati initial
    std::vector <b2Vec2> generate(unsigned int seed);
};
