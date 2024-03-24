//
// Created by takuto1127 on 24/03/08.
//
#include "graphics.hpp"

class Console {
public:
  /// @brief 画面のサイズ
  static const int kRows = 25, kColumns = 80;
  // 画面のサイズは25行80列 staticなのでずっと存在する

  Console(PixelWriter& writer, const PixelColor& fg_color, const PixelColor& bg_color);//コンストラクタ
  void PutString(const char* s);

private:
  void NewLine();

  PixelWriter& writer_;
  const PixelColor fg_color_, bg_color_; //foreground, background color
                                        
  /// 画面バッファ（文字）
  ///null文字を入れるために+1
  char buffer_[kRows][kColumns + 1];   /// カーソルの位置
                                       
  /// @brief カーソルの位置(縦)
  int cursor_row_;
  /// @brief カーソルの位置(横)
  int cursor_column_;

};