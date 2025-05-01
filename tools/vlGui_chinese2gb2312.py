# 
# @author Weilong Shen (valonshen@foxmail.com)
# @brief
# @version 0.2
# @date 2025-05-02
#
# Copyright © 2021 - 2025 Weilong Shen (valonshen@foxmail.com)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys

def chinese_to_gb2312_bytes(text: str) -> bytes:
    try:
        encoded = text.encode("gb2312")
        print("输入汉字：", text)

        print("===================== 小端存储 =====================")
        # 字符串形式（小端）
        hex_pairs = [f'\\x{encoded[i+1]:02X}\\x{encoded[i]:02X}' for i in range(0, len(encoded), 2)]
        result = ''.join(hex_pairs)
        print("GB2312 小端字符串：")
        print(f'"{result}"') 
        # 数组形式（小端）
        hex_pairs = [f'0x{encoded[i+1]:02X},0x{encoded[i]:02X},' for i in range(0, len(encoded), 2)]
        result = ''.join(hex_pairs)
        print("GB2312 小端数组：")
        print(f'"{result}"') 

        print("===================== 大端存储 =====================")
        hex_pairs = [f'\\x{encoded[i]:02X}\\x{encoded[i+1]:02X}' for i in range(0, len(encoded), 2)]
        result = ''.join(hex_pairs)
        print("GB2312 小端字符串：")
        print(f'"{result}"') 
        # 数组形式（小端）
        hex_pairs = [f'0x{encoded[i]:02X},0x{encoded[i+1]:02X},' for i in range(0, len(encoded), 2)]
        result = ''.join(hex_pairs)
        print("GB2312 小端数组：")
        print(f'"{result}"') 

        
    except UnicodeEncodeError as e:
        print(f"编码错误: {e}")
        return ""

# 示例：转换汉字
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("help: python vlGui_chinese2gb2312.py <chiness characters>")
    else:
        text = sys.argv[1]
        chinese_to_gb2312_bytes(text)
