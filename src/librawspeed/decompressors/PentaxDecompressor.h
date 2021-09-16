/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2009-2014 Klaus Post
    Copyright (C) 2017 Axel Waggershauser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "common/RawImage.h"                    // for RawImage
#include "decompressors/AbstractDecompressor.h" // for AbstractDecompressor
#include "decompressors/HuffmanTable.h"         // for HuffmanTable
#include <array>                                // for array
#include <cstdint>                              // for uint8_t

namespace rawspeed {

class ByteStream;

class PentaxDecompressor final : public AbstractDecompressor {
  RawImage mRaw;
  const HuffmanTable ht;

public:
  PentaxDecompressor(const RawImage& img, ByteStream* metaData);

  void decompress(const ByteStream& data) const;

private:
  static HuffmanTable SetupHuffmanTable_Legacy();
  static HuffmanTable SetupHuffmanTable_Modern(ByteStream stream);
  static HuffmanTable SetupHuffmanTable(const ByteStream* metaData);

  static const std::array<std::array<std::array<uint8_t, 16>, 2>, 1>
      pentax_tree;
};

} // namespace rawspeed
