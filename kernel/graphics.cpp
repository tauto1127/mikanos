/**
 * @file graphics.cpp
 *
 * 画像描画関連のプログラムを集めたファイル．
 */

// #@@range_begin(pixel_writer_impl)
#include "graphics.hpp"

void RGBResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  // 赤，緑，青，予約領域の4つ分 （ちょうど4byteになる等になってる
  p[0] = c.r;
  p[1] = c.g;
  p[2] = c.b;
}
// #@@range_end(pixel_writer_impl)

void BGRResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  p[0] = c.b;
  p[1] = c.g;
  p[2] = c.r;
}
