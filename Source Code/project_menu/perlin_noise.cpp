#include "perlin_noise.h"
#include <random>
#include <math.h>

#define pi 3.1415
#define Y_SCALE 5
#define X_SCALE 10


void initMapPara();
void GenerateMap();
void PaintMap();

static int hashNum[]={208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};
//另一个噪声值产生方式

using namespace std;

PerlinNoise::PerlinNoise()
{
    qDebug()<<"Initialized"<<endl;

    //赋予初始值
    float persistence = 0.5;
    //float x = 0.0;
    int NumOfOctaves = 10;
}
//注意x使用了多少次,x为输入的


//**********************************************************************//
//噪声函数每个整数上算一个噪声值
float PerlinNoise::NoiseFunction(long int posx,int id)
{
    switch(id)
    {
        case 1:
            {
                //int temp = (posx*posx*posx+seed*posx);
                posx = (id<<13)^id + posx*posx + seed % 99053;
                float R = (1.0 - ((posx * (posx * posx * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
                return R;
            }
        case 2:
            {
                int temp = (posx+seed)*cos(posx);
                return sin(temp);
            }
        case 3:
            {
                int temp = (((posx+7)*seed)%17+seed)%256;
                return ((float)hashNum[temp])/200;
            }
        return CHUNK_BASE;
    }
    //return sin((long double)t_seed)/2;
}





//*********************************************************************//
//噪声柔化对每个整数上的噪声值柔化
float PerlinNoise::SmoothedNoise(int posx,int id)
{
    float t_1=NoiseFunction(posx,id)/2;
    float t_2=NoiseFunction(posx+1,id)/4;
    float t_3=NoiseFunction(posx-1,id)/4;
    return(t_1+t_2+t_3);
}



//插值函数的定义: 线性插值，正余弦插值，三次函数等
float PerlinNoise::Interpolate(float a, float b, float posx)
{
    //这里posx是属于0-1的浮点数  这里要插值两个临近整数之间的值

    {
        //余弦插值
        float t_xpi=posx*pi;
        float t_fx=(1-cos(t_xpi))*0.5;
        return a*(1-t_fx)+b*t_fx;
    }

//    {
//    	//Perlin插值
//		float y=6*pow(posx,5)-15*pow(posx,4)+10*pow(posx,3);
//		return a*(1-y)+b*y;
//	}


//    {
//        //线性插值
//        return (a*(1-posx)+b*posx);
//    }


}

//一个集成的函数，直接获得了某一个浮点数处的插值最终结果
float PerlinNoise::InterpolatedNoise(float t_x,int id)
{
    int integer_x=(int)(t_x);//取整

    float noiseA=SmoothedNoise(integer_x,id);
    float noiseB=SmoothedNoise(integer_x+1,id);//算出了两个整数噪声并柔化

    float t_h=Interpolate(noiseA,noiseB,t_x-integer_x);//获得了最后的噪声值

    return t_h;
}

//下面得到多次迭代 类似傅里叶级数 的最终结果
float PerlinNoise::FinalValue(float t_x,int id)
{
    //先给类里面的值赋值 持久度、坐标变元x、累加次数
    //persistence小于1
    int i;
    float p=persistence,amplitude,frequency;
    float sum=0.0;

    for(i=0;i<NumOfOctaves;i++)
    {
        frequency=pow(2,i);
        amplitude=pow(p,i);
        sum+=InterpolatedNoise(t_x*frequency,id)*amplitude;
    }

    return sum;
}
//输出最终结果

void PerlinNoise::SetNumOfOctaves(int t_NumOfOctaves)
{
    NumOfOctaves=t_NumOfOctaves;
}
void PerlinNoise::SetPersistence(float t_persistence)
{
    persistence=t_persistence;
}
void PerlinNoise::SetSeed(long int sd)
{
    seed=sd;
}



int PerlinNoise::getBlockId(int ChunkId, int x, int z)
{
    //qDebug() << "getBlockId";
    //qDebug() << seed;
    //获得了全局横坐标
    float posx = ChunkId + ((float)x)/X_SCALE;

    int level1 = CHUNK_BASE - 15 - Y_SCALE*FinalValue(posx,1);
    int level2 = CHUNK_BASE + Y_SCALE*FinalValue(posx,2);
    int level3 = CHUNK_HEIGHT -5 - Y_SCALE*FinalValue(posx,3)/2;
    //有一个强制类型转换

    if( 0<=z && z<=level1)
    {
        return 0;
    }
    else if((level1<=z)&&(z<=level2))
    {
        return 1;
    }
    else if((level2<=z)&&(z<=level3))
    {
        return 0;
    }
    else if((level3<=z)&&(z<=CHUNK_HEIGHT))
    {
        return 1;
    }


}











//========================下面为测试代码==========================//


PerlinNoise Map_0;
int Map[100][180];

void initMapPara()
{
    float t_persistence;
    long int sd;
    int t_NumOfOctaves;
    cout<<"请输入地图生成的种子(Seed):"<<endl;
    cin>>sd;
    Map_0.SetSeed(sd);

    cout<<"请输入持续度(Persistence):"<<endl;
    cin>>t_persistence;
    Map_0.SetPersistence(t_persistence);

    cout<<"请输入迭代次数(Octaves):"<<endl;
    cin>>t_NumOfOctaves;
    Map_0.SetNumOfOctaves(t_NumOfOctaves);
}

void GenerateMap()
{
    int i,dx,y;
    for(dx=0;dx<180;dx++)
    {
        y=50-(int)(15*(Map_0.FinalValue(((float)dx)/10,1)));
        for(i=y;i<100;i++)
        {
            Map[i][dx]=1;
        }
    }
}

void PaintMap()
{
    system("cls");

    int i,j;
    for(i=0;i<100;i++)
    {
        for(j=0;j<180;j++)
        {
            if(Map[i][j]==0&&i<55)
            {
                printf(" ");
            }
            else
            {
                printf("%d",Map[i][j]);
            }
        }
        cout<<endl;
    }
}
