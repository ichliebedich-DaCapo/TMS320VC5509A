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
    return ', '.join(formatted)

def process_char(name, index, H, W, array_name):
    """生成C结构体"""
    return f'''// "{name}"
            {{
                .name = "{name}",
                .width = {W},
                .height = {H},
                .data = {array_name}
            }},'''

def add_to_header(header_file, include_name):
    with open(header_file, 'r') as f:
        lines = f.readlines()

    include_line = f'#include<{include_name}>\n'
    if include_line in lines:
        return  # 已存在，无需操作

    ifndef_line = None
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('#ifndef'):
            ifndef_line = stripped
            break
    if ifndef_line is None:
        return

    macro_name = ifndef_line.split()[1]

    define_line = f'#define {macro_name}\n'
    define_pos = None
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped.startswith('#define') and stripped.endswith(macro_name):
            define_pos = i
            break
    if define_pos is None:
        return

    include_indices = [i for i, line in enumerate(lines) if line.startswith('#include')]
    if include_indices:
        insert_pos = include_indices[-1] + 1
    else:
        insert_pos = define_pos + 1

    endif_pos = None
    for i, line in reversed(list(enumerate(lines))):
        stripped = line.strip()
        if stripped.startswith('#endif'):
            endif_pos = i
            break
    if insert_pos > endif_pos:
        insert_pos = endif_pos - 1

    lines.insert(insert_pos, include_line)
    with open(header_file, 'w') as f:
        f.writelines(lines)

def generate_code(input_file, output_path):
    chars_data = []
    current_elements = []

    with open(input_file, 'r', encoding='gb2312') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue

            if "/*" in line:
                data_part, comment_part = line.split("/*", 1)
                elements_data = re.findall(r'0x[0-9A-Fa-f]+', data_part)
                current_elements.extend(elements_data)

                comment_content = comment_part.split("*/")[0].strip()
                name_str, index_str = comment_content.split(",", 1)
                name = name_str.strip()[1:-1]
                index = int(index_str.strip())

                element_count = len(current_elements)
                chars_data.append((name, index, element_count, current_elements.copy()))
                current_elements = []
            else:
                elements = re.findall(r'0x[0-9A-Fa-f]+', line)
                current_elements.extend(elements)

    H_ascii = None
    for char in chars_data:
        name, _, count, _ = char
        if len(name) == 1 and ord(name) < 128:
            H_ascii = find_H_ascii(count)
            break
    if H_ascii is None:
        raise ValueError("未找到ASCII字符，无法确定字高")

    structs = []
    data_arrays = []

    macro = f'ZQ_FONT_{H_ascii}X{H_ascii}_H'
    output_name = input_file.with_suffix(".h").name
    font_name = Path(output_name).stem
    output_filename = f'{output_path}/{output_name}'

    for name, index, count, elements in chars_data:
        if len(name) == 1 and ord(name) < 128:
            H = H_ascii
            W = H // 2
            bytes_per_row = math.ceil(W / 8)
        else:
            H = find_H_non(count)
            W = H
            bytes_per_row = math.ceil(H / 8)

        required = H * bytes_per_row
        if len(elements) != required:
            raise ValueError(f"字符'{name}'数据数量错误：应为{required}，实际{len(elements)}")

        elements_per_line = bytes_per_row
        data_str = format_data(elements, elements_per_line)

        # 处理特殊字符，确保数组名合法
        name_part = name.replace(' ', '_').replace('\"', '_').replace("'", '_').replace(',', '_')
        array_name = f"{font_name}_{index}"

        data_array = f"    const unsigned char {array_name}[] = {{ {data_str} }};"
        data_arrays.append(data_array)

        struct = process_char(name, index, H, W, array_name)
        structs.append(struct)



    if output_name != f'fonts_{H_ascii}x{H_ascii}.h':
        raise ValueError(f"Fonts Name Error {output_name} -> fonts_{H_ascii}x{H_ascii}.cpp")

    font_header_file = f'{output_path}/zq_font.h'
    add_to_header(font_header_file, output_name)

    output_code = f'''#ifndef {macro}
#define {macro}
#include<zq_font_base.h>

namespace GUI
{{
    namespace Font
    {{
        // 所有字模数据数组定义
        {("""
        """).join(data_arrays)}  // 每个数组占一行

        const FontChar {font_name}[] = {{
            {"".join(structs)}
            // 结束标记
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

    for file in input_path.glob("*.TXT"):
        if "index" not in file.stem:
            generate_code(file, output_path)
            print(f"Process done: {file}")