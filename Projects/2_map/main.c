#include<app_conf.h>
#if (PROGRAM_INDEX == 2)
#include<stdint.h>
#include<math.h>

// 全局函数（可以不使用extern就能外部链接）
extern int add(int x, int y);

#define PI 3.14159265358979

/**
 * @brief  计算函数
 * @param a 弧度制
 * @param b 弧度制
 * @return 数值
 */
double calculate(const double a, const double b)
{
    return sin(a) + cos(b);
}

/**
 * @brief 计算函数
 * @param a 角度制
 * @param b 角度制
 * @return 数值
 */
double calculate_reg(double a, double b)
{
    a = a*PI/180.0;
    b = b*PI/180.0;
    return sin(a) + cos(b);
}


int main(int argc, char *argv[])
{
    double x, y, z;
    x = 2.3*PI;
    y = 1.7*PI;

    while (1)
    {
        // z = x + y;
        // z = add(x, y);
        z = calculate(x, y);
    }
}


#endif
