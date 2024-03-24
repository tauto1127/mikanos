/**
 * @file graphics.cpp
 *
 * 画像描画関連のプログラムを集めたファイル．
 */

// #@@range_begin(pixel_writer_impl)
#include "graphics.hpp"

/// @brief ピクセルを描画する
/// @param x 
/// @param y 
/// @param c 
void RGBResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  // 赤，緑，青，予約領域の4つ分 （ちょうど4byteになる等になってる
  p[0] = c.r;
  p[1] = c.g;
  p[2] = c.b;
}
// #@@range_end(pixel_writer_impl)
/// @brief ピクセルを描画する
/// @param x 
/// @param y 
/// @param c 
void BGRResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  p[0] = c.b;
  p[1] = c.g;
  p[2] = c.r;
}

/// @brief 四角形を描画
/// @param writer 
/// @param pos 
/// @param size 
/// @param c 
void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos, const Vector2D<int>& size, const PixelColor& c) {
  for (int dy = 0; dy < size.y; ++dy) {
    for (int dx = 0; dx < size.x; ++dx) {
      writer.Write(pos.x + dx, pos.y + dy, c);
    }
  }
}
/// @brief 四角形の枠を描画
/// @param wirter 
/// @param pos 
/// @param size 
/// @param c 
void DrawRectangle(PixelWriter& wirter, const Vector2D<int>& pos, const Vector2D<int>& size, const PixelColor& c) {
  for (int dx = 0; dx < size.x; ++dx) {
    wirter.Write(pos.x + dx, pos.y, c);
    wirter.Write(pos.x + dx, pos.y + size.y - 1, c);
  }
  for (int dy = 0; dy < size.y; ++dy) {
    wirter.Write(pos.x, pos.y + dy, c);
    wirter.Write(pos.x + size.x - 1, pos.y + dy, c);
  }
}

template <typename T>
struct Vector2D {
  T x, y;

  template <typename U>
  Vector2D<T>& operator +=(const Vector2D<U>& rhs) {
    x += rhs.x;
    y += rhs.y;
    return +this;
  }
}