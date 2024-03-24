//
// Created by takuto1127 on 3/8/24.
//
#include "console.hpp"
#include "font.hpp"
#include "string.h"

Console::Console(PixelWriter& writer, const PixelColor& fg_color, const PixelColor& bg_color) : writer_{writer}, fg_color_{fg_color}, bg_color_{bg_color},
  buffer_{}, cursor_row_{0}, cursor_column_{0} {

}

void Console::PutString(const char* s) {
  while (*s) {
    if (*s == '\n'){
      NewLine();
    } else if (cursor_column_ < kColumns - 1) {
      WriteAscii(writer_, 8 * cursor_column_, 16 * cursor_row_, *s, fg_color_);
      buffer_[cursor_row_][cursor_column_] = *s;
      ++cursor_column_;
    }
    ++s;
  }
}

/// @brief 改行する
void Console::NewLine() {
  cursor_column_ = 0;
  // 画面をスクロールさせる
  if (cursor_row_ < kRows - 1) {
    ++cursor_row_;
  } else {
    for (int y = 0; y < 16 * kRows; ++y) {
      for (int x = 0; x < 8 * kColumns; ++x) {
        writer_.Write(x, y, bg_color_);
      }
    }
    for (int row = 0; row < kRows - 1; ++row) {
      // buffer_[row]にbuffer_[row + 1]をコピー (サイズはkColumns + 1)
      memcpy(buffer_[row], buffer_[row + 1], kColumns + 1);
      // 一行ずつ文字列を描画
      WriteString(writer_, 0, 16 * row, buffer_[row], fg_color_);
    }
    // buffer_[kRows - 1]を0でクリア
    memset(buffer_[kRows - 1], 0, kColumns + 1);
  }
}