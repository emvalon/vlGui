# @file file2header.py
# @author Weilong Shen (valonshen@foxmail.com)
# @brief
# @version 0.1
# @date 2022-07-26
#
# Copyright © 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
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

import os
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-f", "--file", help="Input your file name", type=str, default="input")
parser.add_argument("-o", "--out", help="output file name", type=str, default="out")
args = parser.parse_args()


def save_to_file(outfile, data):
    name = outfile + ".h"
    with open(name, "wb") as header:
        header.write(bytes("/* length: " + str(len(data)) + " */\n", encoding="utf-8"))
        header.write(bytes("const unsigned char " + args.out + "[] = {\n\t", encoding="utf-8"))
        cnt = 0

        while cnt < len(data):
            header.write(bytearray(str(data[cnt]), encoding="utf-8"))
            header.write(bytearray(",\t", encoding="utf-8"))
            cnt += 1
            if not cnt % 10:
                header.write(bytearray("\n\t", encoding="utf-8"))

        header.write(bytes("\n}", encoding="utf-8"))

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    with open(args.file, "rb") as f:
        hexData = f.read()
        print("len:%d", len(hexData))

    save_to_file(args.out, hexData)

