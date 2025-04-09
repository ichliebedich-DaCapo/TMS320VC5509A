//
// Created by fairy on 2025/4/9 18:15.
//
#ifndef ZQ_IMAGE_BASE_H
#define ZQ_IMAGE_BASE_H

namespace GUI
{
    namespace Image
    {
        typedef struct
        {
            const char *name; // 字符名称（如"8"、"中"）
            uint16_t width; // 图像宽度（像素）
            uint16_t height; // 图像高度（像素）
            const uint16_t *data; // 字模数据指针
        } ImageInfo;

    }
}
#endif //ZQ_IMAGE_BASE_H