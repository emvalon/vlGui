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
import datetime

VERSION = 0X01

parse = argparse.ArgumentParser()
parse.add_argument("-e", "--encode", default='utf-8', choices=['utf-8', 'gb2312'], help="Select encoding format")
parse.add_argument("-c", "--config", default="None", help="configure file name")
parse.add_argument("-f", "--font", default="None", help="select font")
parse.add_argument("-o", "--out", default="vlonGui_font.c", help="output file name")

codeRange = ['a3a1-a3fe',
             'b0a1-b0fe', 'b1a1-b1fe', 'b2a1-b2fe', 'b3a1-b3fe', 'b4a1-b4fe', 'b5a1-b5fe', 'b6a1-b6fe', 'b7a1-b7fe',
             'b8a1-b8fe', 'b9a1-b9fe',
             'baa1-bafe', 'bba1-bbfe', 'bca1-bcfe', 'bda1-bdfe', 'bea1-befe', 'bfa1-bffe', 'c0a1-c0fe', 'c1a1-c1fe',
             'c2a1-c2fe', 'c3a1-c3fe',
             'c4a1-c4fe', 'c5a1-c5fe', 'c6a1-c6fe', 'c7a1-c7fe', 'c8a1-c8fe', 'c9a1-c9fe', 'caa1-cafe', 'cba1-cbfe',
             'cca1-ccfe', 'cda1-cdfe',
             'cea1-cefe', 'cfa1-cffe', 'd0a1-d0fe', 'd1a1-d1fe', 'd2a1-d2fe', 'd3a1-d3fe', 'd4a1-d4fe', 'd5a1-d5fe',
             'd6a1-d6fe', 'd7a1-d7f9',
             ]


class GlyphContainer:
    def __init__(self):
        self.width = 0
        self.height = 0
        self.font_name = ''
        self.done = 0
        self.num = 0
        self.type = 1
        self.font = {}
        self.wait_end = 0
        self.bytes_per_char = 0
        self.container = {}

    def parse(self, elements):
        if self.done:
            return

        if elements[0] == 'ENDCHAR':
            self.container[self.font['unicode']] = self.font
            self.wait_end = 0
        elif self.wait_end:
            self.font['bmp'].append(elements[0])
            if self.font['width'] > self.width:
                print(self.font)
            if self.font['height'] > self.width:
                print(self.font)
        elif elements[0] == 'ENCODING':
            self.font = {'unicode': int(elements[1])}
        elif elements[0] == 'BBX':
            self.font['width'] = int(elements[1])
            self.font['height'] = int(elements[2])
            self.font['x_offset'] = int(elements[3])
            self.font['y_offset'] = int(elements[4])
        elif elements[0] == 'BITMAP':
            self.wait_end = 1
            self.font['bmp'] = []
        elif elements[0] == 'FONTBOUNDINGBOX':
            self.width = int(elements[1])
            self.height = int(elements[1])
            self.bytes_per_char = ((self.width * self.height) + 7) >> 3
            # Add one byte to save width
            self.bytes_per_char = self.bytes_per_char + 1
        elif elements[0] == 'FONT_NAME':
            for word in elements[1:]:
                self.font_name = self.font_name + word
        elif elements[0] == 'CHARS':
            self.num = int(elements[1])
        elif elements[0] == 'ENDFONT':
            self.done = 1

    def getBmp(self, uni):
        return self.container[uni]

    def printBmp(self, bmp):
        print(bmp)
        width = int(bmp['width'])
        line_bytes = int((width + 7) / 8)
        for line in bmp['bmp']:
            v = int(line, 16)
            # print(v)
            msk = 1 << (line_bytes * 8 - 1)
            for i in range(width):
                if v & msk:
                    print('*', end='')
                else:
                    print(' ', end='')
                msk = msk >> 1
            print('')

    def print(self):
        print('font name: ' + self.font_name)
        print('font width: ' + str(self.width))
        print('font height: ' + str(self.height))
        print('total:' + str(self.num))

    def mapToArray(self, bmp, width):
        array = [width]
        v = 0
        c = 0

        bytes_per_line = (width + 7) >> 3
        for row in bmp:
            msk = 1 << (bytes_per_line * 8 - 1)
            row = int(row, 16)
            for p in range(width):
                v = v << 1
                if row & msk:
                    v = v + 1
                msk = msk >> 1
                c = c + 1
                if c >= 8:
                    array.append(v)
                    c = 0
                    v = 0
        if c != 0:
            v = v << (8 - c)
            array.append(v)

        if len(array) < self.bytes_per_char:
            for i in range(self.bytes_per_char - len(array)):
                array.append(0)
        elif len(array) > self.bytes_per_char:
            array = array[:(self.bytes_per_char - len(array))]

        return array

    def save(self, filename, characters_table, format):
        with open(filename, 'w', encoding="utf-8") as f:
            inc_define = "_" + filename.split('.')[0].upper() + "_C_"
            f.writelines('/**\n')
            f.writelines(' * @file ' + filename + '\n')
            f.writelines(' * @brief \n')
            f.writelines(' * \t\tGenerated by vlonGui tool v' + str(VERSION) + '\n')
            f.writelines(' * \t\tfont name: ' + self.font_name + '\n')
            now = datetime.datetime.now()
            f.writelines(' * @date ' + now.strftime("%Y-%m-%d") + '\n')
            f.writelines(" */\n")

            f.writelines("#ifndef " + inc_define + "\n")
            f.writelines("#define " + inc_define + "\n\n")
            f.writelines("const unsigned char " + filename.split('.')[0] + "[] = {\n")

            f.write(hex(VERSION) + ',\t/** Tool version */\n')
            f.write(hex(self.type) + ',\t/** File type */\n')
            f.write(hex(self.width & 0xff) + ',\t/** The width of font */\n')
            f.write(hex(self.height & 0xff) + ',\t/** The height of font */\n')

            f.write(hex(self.bytes_per_char & 0xff) + ', 0x0,\t/** Bytes per character */\n')
            f.write(hex(len(characters_table) & 0xff) + ", ")
            f.write(hex((len(characters_table) >> 8) & 0xff) + ",\t/** Table length */\n")

            offset = 8 + len(characters_table) * 8
            for characters in characters_table:
                f.write(hex(characters[0].encode(format)[1]) + ', ')
                f.write(hex(characters[0].encode(format)[0]) + ', ')
                f.write(hex(characters[-1].encode(format)[1]) + ', ')
                f.write(hex(characters[-1].encode(format)[0]) + ', ')
                f.write('\t/** The start and end of this segment. */\n')

                f.write(hex(offset & 0xff) + ', ')
                f.write(hex((offset >> 8) & 0xff) + ', ')
                f.write(hex((offset >> 16) & 0xff) + ', ')
                f.write(hex((offset >> 24) & 0xff) + ',\t/** The offset of this segment */\n')
                offset = offset + (len(characters) * self.bytes_per_char)

            for characters in characters_table:
                for glyph in characters:
                    for v in self.mapToArray(self.container[ord(glyph)]['bmp'], self.container[ord(glyph)]['width']):
                        f.write(hex(v) + ', ')
                    f.write('\n')

            f.writelines("};\n\n#endif // " + inc_define)


if __name__ == '__main__':
    table = []

    args = parse.parse_args()
    if args.font[-4:] != '.bdf':
        print("Invalid format!!")
        exit(-1)

    # print all of supported fronts. Then exit.
    if not os.path.exists(args.font):
        print("font doesn't exist")
        exit(-1)

    # Read .bdf file and parse all glyphs
    with open(args.font, mode='r') as f:
        container = GlyphContainer()
        lines = f.readlines()
        for line in lines:
            line = line[:-1]
            element = line.split(' ')
            container.parse(element)
        container.print()

    # Get characters that we need to convert
    characters_table = []
    for entry in codeRange:
        r = entry.split('-')
        min_v = int(r[0], 16)
        max_v = int(r[1], 16)

        item = []
        for value in range(min_v, max_v + 1):
            # Convert to string with input encoding format
            code = value.to_bytes(2, 'big')
            character = code.decode(args.encode)
            item.append(character)
        characters_table.append(item)

    # Generate vlonGui fonts file
    container.save(args.out, characters_table, args.encode)
    print('done!!!')
