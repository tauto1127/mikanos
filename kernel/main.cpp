/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたファイル．
 */

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "console.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "frame_buffer_config.hpp"

char console_buf[sizeof(Console)];
Console* console;

void* operator new(size_t size, void* buf) {
  return buf;
}

void operator delete(void* obj) noexcept {
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;



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
  //WriteAscii(*pixel_writer, 50, 50, 'A', {0, 0, 0});//Aを描画
  //WriteAscii(*pixel_writer, 58, 50, 'k', {0, 0, 0});//Bを描画

  //int i = 0;
  //for (char c = '!'; c <= '~'; ++c, ++i) {
  //  WriteAscii(*pixel_writer, 8 * i, 50, c, {0, 0, 0});
  //}

  //WriteString(*pixel_writer, 0, 66, "Hello, World!", {255, 0, 0});

  //char buf[128];
  //sprintf(buf, "1 + 2 = %d", 1 + 2);d

  //WriteString(*pixel_writer, 0, 82, buf, {0, 255, 0});

  console = new(console_buf) Console{*pixel_writer, {0, 0, 0}, {255, 255, 0}};
  console->PutString("Hello, World!\n, This is MikanOS!\n\nuooooooooooooooooooooooooooo\n uooooooooooooooooooooooooo\n    uoooooooooooooooooooooo\n      uoooooooooooooooooooo\n        uoooooooooooooooooo\n          uoooooooooooooooo\n            uoooooooooooooo\n              uoooooooooooo\n                uoooooooooo\n                  uoooooooo\n                    uoooooo\n                      uoooo\n                        uoo\n                          u\n");
  console->PutString("Hello, World!\n, This is MikanOS!\n\nuooooooooooooooooooooooooooo\n uooooooooooooooooooooooooo\n    uoooooooooooooooooooooo\n      uoooooooooooooooooooo\n        uoooooooooooooooooo\n          uoooooooooooooooo\n            uoooooooooooooo\n              uoooooooooooo\n                uoooooooooo\n                  uoooooooo\n                    uoooooo\n                      uoooo\n                        uoo\n                          u\n");
  console->PutString("Hello, World!\n, This is MikanOS!\n\nuooooooooooooooooooooooooooo\n uooooooooooooooooooooooooo\n    uoooooooooooooooooooooo\n      uoooooooooooooooooooo\n        uoooooooooooooooooo\n          uoooooooooooooooo\n            uoooooooooooooo\n              uoooooooooooo\n                uoooooooooo\n                  uoooooooo\n                    uoooooo\n                      uoooo\n                        uoo\n                          u\n");
  while (1) __asm__("hlt");
}
