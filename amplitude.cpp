#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <array> 

#include "config.h"
#include "user.h"
#include "amplitude.h"



extern float WaveAmp[2][scene][scene];
extern int source, target;
bool stop = false;

void calcNewAmplitude(int x, int y, float neighborSum, float invAttenuationRate)
{
    float n = neighborSum - WaveAmp[target][x][y];
    n -= n * invAttenuationRate;
    WaveAmp[target][x][y] = n;
}

void calcAmplitude()
{
    int x, y;
    float invAttenuationRate = 1.0 / attenuationRate;



    for(y = 1; y < scene-1; y++)
    {
        for(x = 1; x < scene-1; x++)
        {
            calcNewAmplitude(x, y, (WaveAmp[source][x-1][y] + WaveAmp[source][x+1][y] + WaveAmp[source][x][y-1] + WaveAmp[source][x][y+1]) / 2, invAttenuationRate);
        }
    }

    for(x = 1; x < scene-1; x++)
    {
        calcNewAmplitude(x, 0, (WaveAmp[source][x-1][0] + WaveAmp[source][x+1][0] + WaveAmp[source][x][1]) / 2, invAttenuationRate);
        calcNewAmplitude(x, scene-1, (WaveAmp[source][x-1][scene-1] + WaveAmp[source][x+1][scene-1] + WaveAmp[source][x][scene-2]) / 2, invAttenuationRate);
    }
    
    for(y = 1; y < scene-1; y++)
    {
        calcNewAmplitude(0, y, (WaveAmp[source][1][y] + WaveAmp[source][0][y-1] + WaveAmp[source][0][y+1]) / 2, invAttenuationRate);
        calcNewAmplitude(scene-1, y, (WaveAmp[source][scene-2][y] + WaveAmp[source][scene-1][y-1] + WaveAmp[source][scene-1][y+1]) / 2, invAttenuationRate);
    }
}



void calcWaveAmplitude(float currentTime, float frequency, float speed)
{
    int x, y;
    float waveLength = speed / frequency; 
    float damping = 0.0001f;
    float amplitudeScale = 10.0f;

    for(y = 0; y < scene; y++)
    {
        for(x = 0; x < scene; x++)
        {
            float timeDelay = (float)x / scene; 
            double currentTimeWithDelay = getCurrentTime() - timeDelay;
            if(currentTimeWithDelay < 0)
                WaveAmp[target][x][y] = 0;
            else
                WaveAmp[target][x][y] = amplitudeScale * sinf(2.0f * M_PI * ((x / waveLength) - (speed * currentTimeWithDelay))) * exp(-damping * timeDelay); // Damping reduces the amplitude over time
        }
    }
}



float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float y, float z) 
{
    int h = hash & 15;
    float u = h < 8 ? x : y,
          v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y, float z) 
{
    static const int permutation[] = {
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    std::array<int, 512> p;
    for (int i=0; i < 256 ; i++) 
        p[256+i] = p[i] = permutation[i];
    std::copy(std::begin(permutation), std::end(permutation), p.begin());
    std::copy(std::begin(permutation), std::end(permutation), p.begin()+256);

    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x),
          v = fade(y),
          w = fade(z);
    int A = p[X] + Y;
    int AA = p[A] + Z; 
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y; 
    int BA = p[B] + Z; 
    int BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                   grad(p[BA], x - 1, y, z)),
                           lerp(u, grad(p[AB], x, y - 1, z),
                                   grad(p[BB], x - 1, y - 1, z))),
                   lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                   grad(p[BA + 1], x - 1, y, z - 1)),
                           lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                   grad(p[BB + 1], x - 1, y - 1, z - 1))));
}


void calcWaveAmplitudeWithPerlinNoise(float currentTime, float frequency, float speed)
{
    int x, y;
    float waveLength = speed / frequency; 
    float damping = 0.0001f;
    float amplitudeScale = 10.0f;

    for(y = 0; y < scene; y++)
    {
        for(x = 0; x < scene; x++)
        {
            float timeDelay = (float)x / scene;
            double currentTimeWithDelay = getCurrentTime() - timeDelay;
            if(currentTimeWithDelay < 0)
                WaveAmp[target][x][y] = 0;
            else
            {
                float noise = perlin(x / waveLength, y / waveLength, currentTimeWithDelay);
                WaveAmp[target][x][y] = amplitudeScale * noise * sinf(2.0f * M_PI * ((x / waveLength) - (speed * currentTimeWithDelay))) * exp(-damping * timeDelay);
            }
        }
    }
}


float scale = 0.1f;
float noiseAmplitude = 0.5f;
float noiseFrequency = 0.2f; 

float amplitudeScale = 10.0f;
float speedScale = 0.01f;


void calcMovementWithPerlinNoise(float currentTime, float frequency, float speed)
{
    int x, y;
    float waveLength = speed / frequency; 
    float damping = 0.0001f;
    currentTime = 1;

    for(y = 0; y < scene; y++)
    {
        for(x = 0; x < scene; x++)
        {
            float noise = 0.0;
            float amp = 0.5;
            float freq = 1.0;

            for(int i = 0; i < 4; i++)
            {
                noise += amp * perlin(x * freq / waveLength, y * freq / waveLength, speedScale * currentTime);
                freq *= 2.0;
                amp *= 0.5;
            }
            WaveAmp[target][x][y] = amplitudeScale * noise * sinf(2.0f * M_PI * ((x / waveLength) - (speed * currentTime))) * exp(-damping * currentTime);
        }
    }
}

void stopAllWaves()
{
    for( int i = 0; i < scene; i++)
    {
        for( int j = 0; j < scene; j++)
        {
            WaveAmp[0][j][i] = 0;
            WaveAmp[1][j][i] = 0;
        }
    }
}