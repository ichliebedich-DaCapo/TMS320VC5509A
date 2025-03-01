//
// Created by fairy on 2025/3/1 21:22.
//
//-------------------------------相关头文件------------------------------------//
#include <stdio.h>

//-------------------------------声明----------------------------------//
#define BUF_SIZE 0x64
#define TRUE 1
#define FALSE 0

struct PARMS
{
    int Beta;
    int EchoPower;
    int ErrorPower;
    int Ratio;
    struct PARMS *Link;
};

//-------------------------------工作变量定义----------------------------------//
int inp_buffer[BUF_SIZE]; // 输入缓冲区
int out_buffer[BUF_SIZE]; // 输出缓冲区
int *input;
int *output;

int volume = 2;

// 没有别的作用，只是用于在变量窗口中查看
struct PARMS str =
{
    2934, 9432, 213, 9432, &str
};

//-------------------------------调用子程序规则--------------------------------//
int read_signals(int *input);

int write_buffer(const int *input, int *output, int count);

int output_signals(int *output);


//-----------------------------------主程序------------------------------------//
int main()
{
    // ======初始化======
    int i = 0;
    input = inp_buffer;
    output = out_buffer;

    // ======无限循环======
    while (TRUE)
    {
        read_signals(input); // 加软件断点和探针
        write_buffer(input, output,BUF_SIZE);
        output_signals(output);
        printf("Number: %d\n", i++);
    }
}

//---------------------------------子程序--------------------------------------//
// 读取输入信号
int read_signals(int *input)
{
    // 在此读取采集数据信号放到输入缓冲区input[]
    return (TRUE);
}

// 将数据进行处理后搬移到输出缓冲区
int write_buffer(const int *input, int *output, const int count)
{
    int i;
    for (i = 0; i < count; i++)
        output[i] = input[i] * volume; // 处理：将输入数据放大volume倍放到输出缓冲区
    return (TRUE);
}

// 输出处理后的信号
int output_signals(int *output)
{
    // 在此将输出缓冲区out_buffer中的数据发送到输出设备(比如DA)
    return (TRUE);
}
