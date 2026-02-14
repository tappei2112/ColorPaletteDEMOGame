#pragma once
class DigitDisplay
{
public:
  //数字フォントのグラフィックのハンドルを渡します
  //グラフィックのサイズは'0'のサイズでとります
  void InportHandle(int* handle);

  //グラフィックハンドルを取得します
  int GetHandle(int DigitNUM);


  void Display(int NUM, int X, int Y, float size = 1.0f);

  void Display_Animaion(int NUM, int X, int Y, int speed, int High, float delay_time, float size = 1.0f);


protected:
  int m_numGraph[10];//数字フォントのグラフィックのハンドル

  int m_Xsize;//数字のXサイズ
  int m_Ysize;//数字のYサイズ
};

