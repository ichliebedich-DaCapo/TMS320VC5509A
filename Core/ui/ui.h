//
// Created by fairy on 2025/3/3 12:14.
//
#ifndef UI_H
#define UI_H


#include<zq_conf.h>
    // 结构体定义


// 提供一些ui接口，由于是C风格，那么采样前缀ui_的_形式
namespace GUI
{
    class SpectrumDisplay
    {
    public:
        enum ZoomLevel
        {
            ZOOM_1X,
            ZOOM_2X,
            ZOOM_4X,
            ZOOM_8X
        };

        static void Init();

        static void DrawAxes();

        static void DrawSpectrum(const float spectrum[]);

        static void SetZoom(ZoomLevel level);

        static void Pan(bool moveLeft);

        static void SetAmplitudeRange(float min, float max);

    private:
        static float startFreq;
        static float endFreq;
        static float minAmpl;
        static float maxAmpl;
        static ZoomLevel currentZoom;

        static const uint16_t AXIS_LEFT = 15;
        static const uint16_t AXIS_BOTTOM = 63;
        static const uint16_t PLOT_WIDTH = 101;
        static const uint16_t PLOT_HEIGHT = 48;
        static const uint16_t FFT_SIZE = 1024;
        static const float SAMPLE_RATE;

        static void DrawHorizontalAxis();

        static void DrawVerticalAxis();

        static int FreqToIndex(float freq);

        static float IndexToFreq(int index);
    };
}


#endif //UI_H