/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2017-2018 Roman Lebedev

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

#include "decompressors/DummyHuffmanTable.h"
#include "io/Buffer.h"     // for Buffer
#include "io/ByteStream.h" // for ByteStream
#include <type_traits>     // for is_same

namespace rawspeed {
template <typename HuffmanTableTag> class DummyHuffmanTable;
} // namespace rawspeed

template <typename T> static constexpr int getHuffmanTableMaxLength() {
  if constexpr (std::is_same<T, rawspeed::DummyHuffmanTable<>>())
    return 0;
  return T::Traits::MaxCodeLenghtBits;
}

template <typename T> static T createHuffmanTable(rawspeed::ByteStream& bs) {
  T ht;

  // first 16 bytes are consumed as n-codes-per-length
  const auto count =
      ht.setNCodesPerLength(bs.getBuffer(getHuffmanTableMaxLength<T>()));

  if (count) {
    // and then count more bytes consumed as code values
    const auto codesBuf = bs.getBuffer(count);
    ht.setCodeValues(rawspeed::Array1DRef<const uint8_t>(codesBuf.begin(),
                                                         codesBuf.getSize()));
  }

  // and one more byte as 'fixDNGBug16' boolean
  const bool fixDNGBug16 = bs.getByte() != 0;

  bool fullDecode = false;
  if (T::Traits::SupportsFullDecode)
    fullDecode = bs.getByte() != 0;

  ht.setup(fullDecode, fixDNGBug16);

  return ht;
}
