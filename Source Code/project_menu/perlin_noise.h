#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <QDebug>

#include "mapgenerating.h"

//柏林噪声算法，应该包括以下部分
//噪声函数-优化算法-插值-计算
class PerlinNoise
{
public:
    PerlinNoise();
    float FinalValue(float t_x,int id);//最终计算出函数值
    float NoiseFunction(long int t_seed,int id);//噪声函数
    int getBlockId(int ChunkId,int x,int y);
    void SetSeed(long int sd);
    void SetPersistence(float t_persistence);
    void SetNumOfOctaves(int t_NumOfOctaves);
private:
    float persistence = 0.6;//持续度
    int NumOfOctaves = 10;//一共迭代几次
    long int seed;
    //float x;         //浮点坐标
protected:
    float SmoothedNoise(int seed,int id);//柔化每一点的噪声值
    float InterpolatedNoise(float t_x,int id);//算非整数的噪声值 相邻整数之间插值
    float Interpolate(float a,float b,float noise_x);//插值函数
};
#endif
