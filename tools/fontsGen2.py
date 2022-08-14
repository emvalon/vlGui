# @file fontsGen.py
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

import argparse
import os.path

import PIL.ImageFont
from PIL import Image

parse = argparse.ArgumentParser()
parse.add_argument("-s", "--size", type=int, default=16, help="Set size of font")
parse.add_argument("-e", "--encode", default='utf-8', choices=['utf-8', 'gb2312'], help="Select encoding format")
parse.add_argument("-c", "--config", default="None", help="configure file name")
parse.add_argument("-f", "--font", default="None", help="select font")
parse.add_argument("-b", "--bold", action='store_true', help="Bold")
parse.add_argument("-r", "--review", action='store_true', help="don't generate file. Show generated demo")

codeRange = ['b0a1-b0fe', 'b1a1-b1fe', 'b2a1-b2fe', 'b3a1-b3fe', 'b4a1-b4fe', 'b5a1-b5fe', 'b6a1-b6fe', 'b7a1-b7fe',
             'b8a1-b8fe', 'b9a1-b9fe',
             'baa1-bafe', 'bba1-bbfe', 'bca1-bcfe', 'bda1-bdfe', 'bea1-befe', 'bfa1-bffe', 'c0a1-c0fe', 'c1a1-c1fe',
             'c2a1-c2fe', 'c3a1-c3fe',
             'c4a1-c4fe', 'c5a1-c5fe', 'c6a1-c6fe', 'c7a1-c7fe', 'c8a1-c8fe', 'c9a1-c9fe', 'caa1-cafe', 'cba1-cbfe',
             'cca1-ccfe', 'cda1-cdfe',
             'cea1-cefe', 'cfa1-cffe', 'd0a1-d0fe', 'd1a1-d1fe', 'd2a1-d2fe', 'd3a1-d3fe', 'd4a1-d4fe', 'd5a1-d5fe',
             'd6a1-d6fe', 'd7a1-d7f9',
             ]

# Type
PCF_PROPERTIES = (1 << 0)
PCF_ACCELERATORS = (1 << 1)
PCF_METRICS = (1 << 2)
PCF_BITMAPS = (1 << 3)
PCF_INK_METRICS = (1 << 4)
PCF_BDF_ENCODINGS = (1 << 5)
PCF_SWIDTHS = (1 << 6)
PCF_GLYPH_NAMES = (1 << 7)
PCF_BDF_ACCELERATORS = (1 << 8)
# format
PCF_DEFAULT_FORMAT = 0x00000000
PCF_INKBOUNDS = 0x00000200
PCF_ACCEL_W_INKBOUNDS = 0x00000100
PCF_COMPRESSED_METRICS = 0x00000100


class BmpTable:
    def __init__(self, buffer):
        print(buffer[:20])
        self.format = int.from_bytes(buffer[:4], 'little', signed=False)
        self.glyph_count = int.from_bytes(buffer[4:8], 'big', signed=False)
        # glyph offsets
        self.offsets = []
        buffer_index = 8
        for i in range(self.glyph_count):
            self.offsets.append(int.from_bytes(buffer[buffer_index:buffer_index + 4], 'big', signed=False))
            buffer_index = buffer_index + 4
        # bitmap Sizes
        self.bitmap_size = []
        for i in range(4):
            self.bitmap_size.append(int.from_bytes(buffer[buffer_index:buffer_index + 4], 'big', signed=False))
            buffer_index = buffer_index + 4

    def print(self):
        print("format:" + str(self.format))
        print("glyph count:" + str(self.glyph_count))
        print("offsets:")
        print(self.offsets[:10])
        print("bitmap size:")
        print(self.bitmap_size)

class TocEntry:
    entries = []

    def __init__(self, buffer, count):
        for i in range(count):
            entry_type = int.from_bytes(buffer[(16 * i):(16 * i) + 4:], 'little', signed=False)
            entry_format = int.from_bytes(buffer[(16 * i) + 4:(16 * i) + 8:], 'little', signed=False)
            entry_size = int.from_bytes(buffer[(16 * i) + 8:(16 * i) + 12:], 'little', signed=False)
            entry_offset = int.from_bytes(buffer[(16 * i) + 12:(16 * i) + 16:], 'little', signed=False)
            item = {'type': entry_type, 'format': entry_format, 'size': entry_size, 'offset': entry_offset}
            self.entries.append(item)

    def getEntries(self):
        return self.entries


class PcfHeader:
    def __init__(self, buffer):
        print(buffer[:60])
        self.header = buffer[:4]
        self.table_count = int.from_bytes(buffer[4:8], 'little', signed=False)
        self.entries = TocEntry(buffer[8:], self.table_count).getEntries()

    def print(self):
        print("header:" + str(self.header))
        print("table count:" + str(self.table_count))
        print(self.entries)


class TableItem:
    end = 0
    num = 0
    pattern = []

    def __init__(self, max):
        self.end = max

    def getPatternLength(self):
        return self.num

    def appendPattern(self, m):
        self.pattern.append(m)
        self.num = self.num + 1


def mapToArray(map):
    array = []
    v = 0
    c = 0
    for p in map:
        v = v << 1
        if p != 0:
            v = v + 1
        c = c + 1
        if c >= 8:
            array.append(v)
            v = 0
            c = 0
    if c != 0:
        array.append(v)
    return array


def save(file, table):
    with open(file, 'w', encoding="utf-8") as f:
        incDefine = "_" + str(file).split('.')[0].upper() + "_H_"
        f.writelines("/* Generated by vlonGui tool v0.0.1\n")
        f.writelines(" */\n")
        f.writelines("#ifndef " + incDefine + "\n")
        f.writelines("#define " + incDefine + "\n\n")
        f.writelines("const unsigned char " + str(file).split('.')[0] + "[] = {\n")

        f.write(hex(args.size & 0xff) + ',\t//the width of font\n')
        f.write(hex(len(table) & 0xff) + ", ")
        f.write(hex((len(table) >> 8) & 0xff) + ",\t //table length\n")
        for item in table:
            f.write(hex(item.end & 0xff) + ', ')
            f.write(hex((item.end >> 8) & 0xff) + ', ')
            f.write(hex(item.getPatternLength() & 0xff) + ', ')
            f.write(hex((
                                item.getPatternLength() >> 8) & 0xff) + ',\t//the ending of this segment, the length of this segment\n')

        for item in table:
            for array in item.pattern:
                for v in array:
                    f.write(hex(v) + ', ')
                f.write('\n')

        f.writelines("\;\n\n#endif // " + incDefine)


if __name__ == '__main__':
    table = []

    args = parse.parse_args()
    # print all of supported fronts. Then exit.
    if not os.path.exists(args.font):
        print("font doesn't exist");
        exit(-1)

    with open(args.font, mode='rb') as f:
        fontBin = f.read()
        print(type(fontBin))
        header = PcfHeader(fontBin)
        header.print();

        for table in header.entries:
            if table['type'] == PCF_BITMAPS:
                print(table)
                print(table['offset'])
                bitmap_table = BmpTable(fontBin[table['offset']:])
                bitmap_table.print()

    if (args.review):
        print("review")
        exit(0)

    # for entry in codeRange:
    #     r = entry.split('-')
    #     min = int(r[0], 16)
    #     max = int(r[1], 16)
    #
    #     item = TableItem(max)
    #     table.append(item)
    #
    #     for value in range(min, max + 1):
    #         # 待转换文字
    #         code = value.to_bytes(2, 'big')
    #         text = code.decode(args.encode)
    #         print(text)

    # save(selectedFont + '_' + str(args.size) + "_" + args.encode + '.c', table)
    print('done!!!')
