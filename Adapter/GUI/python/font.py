import re
import math
from pathlib import Path


def find_H_ascii(N_ascii):
    """根据ASCII字符元素数量推断字高"""
    for H in range(1, 2*N_ascii + 1):
        W = H // 2
        required = H * math.ceil(W / 8)
        if required == N_ascii:
            return H
    raise ValueError(f"无法根据ASCII元素数量{N_ascii}推断字高")

def find_H_non(N_non):
    """根据非ASCII字符元素数量推断尺寸"""
    for H in range(1, 2*N_non + 1):
        required = H * math.ceil(H / 8)
        if required == N_non:
            return H
    raise ValueError(f"无法根据非ASCII元素数量{N_non}推断尺寸")

def format_data(elements, elements_per_line):
    """格式化数据为C数组格式"""
    formatted = []
    line = []
    for i, elem in enumerate(elements):
        line.append(elem)
        if (i + 1) % elements_per_line == 0:
            formatted.append(', '.join(line))
            line = []
    if line:
        formatted.append(', '.join(line))
    return ','.join(formatted)

def process_char(name, index, H, W, data_str):
    """生成C结构体"""
    return f'''// "{name}"
            {{
                .name = "{name}",
                .width = {W},
                .height = {H},
                .data = (const uint16_t[]){{ 
                    {data_str}
                }}
            }},'''

def generate_code(input_file,output_path):
    chars_data = []
    current_elements = []

    with open(input_file, 'r', encoding='gb2312') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue

            if "/*" in line:
                # 分割数据和注释部分
                data_part, comment_part = line.split("/*", 1)
                elements_data = re.findall(r'0x[0-9A-Fa-f]+', data_part)
                current_elements.extend(elements_data)

                # 处理注释内容
                comment_content = comment_part.split("*/")[0].strip()
                name_str, index_str = comment_content.split(",", 1)
                name = name_str.strip()[1:-1]
                index = int(index_str.strip())

                # 记录字符数据
                element_count = len(current_elements)
                chars_data.append( (name, index, element_count, current_elements.copy()) )
                current_elements = []
            else:
                # 提取元素
                elements = re.findall(r'0x[0-9A-Fa-f]+', line)
                current_elements.extend(elements)

    # 确定字高
    H_ascii = None
    for char in chars_data:
        name, _, count, _ = char
        if len(name) == 1 and ord(name) < 128:
            H_ascii = find_H_ascii(count)
            break
    if H_ascii is None:
        raise ValueError("未找到ASCII字符，无法确定字高")

    structs = []
    for name, index, count, elements in chars_data:
        # 计算宽高
        if len(name) == 1 and ord(name) < 128:
            # ASCII字符
            H = H_ascii
            W = H // 2
            bytes_per_row = math.ceil(W / 8)
        else:
            # 非ASCII字符
            H = find_H_non(count)
            W = H
            bytes_per_row = math.ceil(H / 8)

        # 验证元素数量
        required = H * bytes_per_row
        if len(elements) != required:
            raise ValueError(f"字符'{name}'数据数量错误：应为{required}，实际{len(elements)}")

        # 格式化数据
        elements_per_line = bytes_per_row
        data_str = format_data(elements, elements_per_line)

        # 生成结构体
        struct = process_char(name, index, H, W, data_str)
        structs.append(struct)

    # 生成输出文件名
    macro = f'ZQ_FONT_{H_ascii}X{H_ascii}_H'
    # output_filename = f'fonts_{H_ascii}x{H_ascii}.cpp'
    output_name = input_file.with_suffix(".h").name
    output_filename = f'{output_path}/{output_name}'

    # 输出文件校验 判断字体大小与文件名是否合理
    if output_name != f'fonts_{H_ascii}x{H_ascii}.h':
        raise ValueError(f"Fonts Name Error {output_name} -> fonts_{H_ascii}x{H_ascii}.cpp")

    # 生成最终代码
    output_code = f'''#ifndef {macro}
#define {macro}
#include<zq_font_base.h>

namespace GUI
{{
    namespace Font
    {{
        const FontChar font_{H_ascii}x{H_ascii}[] = {{
            {(""
                "").join(structs)}// end
            {{
                .name = nullptr,
                .width = 0,
                .height = 0,
                .data = nullptr
            }},  
        }};
    }}
}}
#endif
'''

    with open(output_filename, 'w', encoding='utf-8') as f:
        f.write(output_code)

if __name__ == "__main__":
    input_path = Path("../font")
    output_path = Path("../../../Middleware/Library/gui")


    # 遍历所有 .txt 文件
    for file in input_path.glob("*.TXT"):
        # 过滤文件名不含 "index" 的文件（不区分大小写可改为 .lower()）
        if "index" not in file.stem:
            # 调用你的生成函数
            generate_code(file, output_path)
            print(f"Process done: {file}")
