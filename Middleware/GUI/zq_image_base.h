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
            const unsigned char *data; // 字模数据指针
        } ImageInfo;

        // 根据名称查找字符
        inline const ImageInfo *find_image_by_name(const ImageInfo image_table[], const char *name)
        {
            for (uint16_t i = 0; image_table[i].name != nullptr; ++i)
            {
                if (std::strcmp(image_table[i].name, name) == 0)
                {
                    return &image_table[i];
                }
            }
            return nullptr; // 未找到
        }

    }
}
#endif //ZQ_IMAGE_BASE_H
