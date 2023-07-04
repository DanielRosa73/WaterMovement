#ifndef AMPLITUDE_H
#define AMPLITUDE_H

void calcAmplitude();
void calcWaveAmplitude(float currentTime, float frequency, float speed);
void calcWaveAmplitudeWithPerlinNoise(float currentTime, float frequency, float speed);
void calcMovementWithPerlinNoise(float currentTime, float frequency, float speed);
void stopAllWaves();




#endif