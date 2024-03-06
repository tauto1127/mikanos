/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたファイル．
 */

#include <cstdint>
#include <cstddef>

#include "frame_buffer_config.hpp"

const uint8_t kFontA[16] = {
  0b00000000, //
  0b00011000, //    **
  0b00011000, //    **
  0b00011000, //    **
  0b00011000, //    **
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b01111110, //  ******
  0b01000010, //  *    *
  0b01000010, //  *    *
  0b01000010, //  *    *
  0b11100111, // ***  ***
  0b00000000, //
  0b00000000, //
};

struct PixelColor {//色の定義
  uint8_t r, g, b;
};

class PixelWriter {
  public: 
    PixelWriter(const FrameBufferConfig& config) : config_{config} {
    }
    //デストラクタ
    virtual ~PixelWriter() = default;
    virtual void Write(int x, int y, const PixelColor& c) = 0;
  
  protected:
    uint8_t* PixelAt(int x, int y) {
      /*   x
   * * * * *
   * * * * * y
   * * * * *
  | pixels_per_scan_line| (横幅)
  */
      return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
    }
  
  private:
    const FrameBufferConfig& config_;
};
/*+ WritePixelは一つの点を描画
 * @retval 0 成功
 * @retval not0 失敗
*/
class RGBResv8BitPerColorPixelWriter : public PixelWriter {
  
  public: 
    using PixelWriter::PixelWriter;//親クラスのコンストラクタを使う
  
  virtual void Write(int x, int y, const PixelColor& c) override {
    auto p = PixelAt(x, y);
    // 赤，緑，青，予約領域の4つ分 （ちょうど4byteになる等になってる
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
  }
};
class BGRResv8BitPerColorPixelWriter : public PixelWriter {
  
  public: 
    using PixelWriter::PixelWriter;//親クラスのコンストラクタを使う
  
  virtual void Write(int x, int y, const PixelColor& c) override {
    auto p = PixelAt(x, y);
    // 青，緑，赤，予約領域の4つ分 （ちょうど4byteになる等になってる
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
  };
};

void* operator new(size_t size, void* buf) {
  return buf;
}

void operator delete(void* obj) noexcept {
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  if (c != 'A') {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      /*
       * dxが0の時：左端
       * dxが7の時：右端
      */
      if ((kFontA[dy] << dx) & 0x80u) { //0x80は10000000
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  switch (frame_buffer_config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      /*
       * なぜnewが引数をとるのか？
       * 配置newという．
       * 普通のnew演算子を使うにはヒープ領域を確保する必要があるが，これはOSがメモリ管理機能を搭載しなければ使えない．
       * しかし，配置new演算子では引数に渡したアドレスにオブジェクトを生成することができる．
      */
      pixel_writer = new(pixel_writer_buf) RGBResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
    case kPixelBGRResv8BitPerColor:
                    //? なぜ引数がひとつなのか
      pixel_writer = new(pixel_writer_buf) BGRResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
  }
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
      pixel_writer -> Write(x, y, {255, 255, 255});
    }
  }
  WriteAscii(*pixel_writer, 50, 50, 'A', {0, 0, 0});//Aを描画

  while (1) __asm__("hlt");
}
