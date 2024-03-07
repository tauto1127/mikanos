#include "graphics.hpp"

//hankaku.oで勝手に定義されている変数を使うための宣言
extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;
//unitとuintを勘違いしてしまった

const uint8_t* GetFont(char c) {
    auto index = 16 * static_cast<unsigned int>(c);
    if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {//別のポインタ型に変換
        return nullptr;
    }
    return &_binary_hankaku_bin_start + index;
}

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  const uint8_t *font = GetFont(c);
  if(font == nullptr) {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      /*
       * dxが0の時：左端
       * dxが7の時：右端
      */
      if ((font[dy] << dx) & 0x80u) { //0x80は10000000
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

void WriteString(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color) {
  for (int i = 0; s[i] != '\0'; ++i) {
    WriteAscii(writer, x + 8 * i, y, s[i], color);
  }
}