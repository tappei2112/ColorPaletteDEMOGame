#include "DigitDisplay_class.h"
#include "DxLib.h"
#include <cmath>


void DigitDisplay::InportHandle(int* handle)
{
  for (int i = 0; i < 10; i++) {
    m_numGraph[i] = handle[i];
  }

  GetGraphSize(m_numGraph[0], &m_Xsize, &m_Ysize);

}



int DigitDisplay::GetHandle(int DigitNUM)
{
  return m_numGraph[DigitNUM];
}


void DigitDisplay::Display(int NUM, int X, int Y, float size)
{
  int NUMdig = 0;//Œ…”

  if (NUM == 0) {
    NUMdig = 1;
  }
  else {
    for (int i = NUM; i > 0; i = i / 10) {
      ++NUMdig;
    }
  }

  int* dig;//ŠeŒ…‚Ì”ŽšŠi”[”z—ñ(0Œ…–Ú‚©‚ç)
  dig = new int[NUMdig];

  int num = NUM;

  for (int i = NUMdig - 1; i >= 0; i--) {
    if (i == 0) {
      dig[i] = num;
    }
    else {
      int x = std::pow(10, i);
      dig[i] = num / x;
      num = num % x;
    }
  }

  int dX = 0;

  if (NUMdig % 2 == 0) {
    dX = X + ((m_Xsize * size) / 2);
    if (NUMdig / 2 >= 2) {
      dX += (m_Xsize * size) * ((NUMdig / 2) - 1);
    }
  }
  else {
    if (NUMdig == 1) {
      dX = X;
    }
    else {
      dX = X + ((m_Xsize * size) * (NUMdig / 2));
    }

  }


  for (int i = 0; i < NUMdig; i++) {

    DrawRotaGraph(dX, Y, size, NULL, m_numGraph[dig[i]], TRUE, FALSE);
    dX -= m_Xsize * size;

  }




  delete[] dig;

  return;
}









void DigitDisplay::Display_Animaion(int NUM, int X, int Y, int speed, int High, float delay_time, float size)
{
  int NUMdig = 0;//Œ…”

  for (int i = NUM; i > 0; i = i / 10) {
    ++NUMdig;
  }

  int* dig;//ŠeŒ…‚Ì”ŽšŠi”[”z—ñ(0Œ…–Ú‚©‚ç)
  dig = new int[NUMdig];

  int num = NUM;

  for (int i = NUMdig - 1; i >= 0; i--) {
    if (i == 0) {
      dig[i] = num;
    }
    else {
      int x = std::pow(10, i);
      dig[i] = num / x;
      num = num % x;
    }

  }

  int dX = 0;

  if (NUMdig % 2 == 0) {
    dX = X + ((m_Xsize * size) / 2);
    if (NUMdig / 2 >= 2) {
      dX += (m_Xsize * size) * ((NUMdig / static_cast<float>(2)) - 1);
    }



  }
  else {
    if (NUMdig == 1) {
      dX = X;
    }
    else {
      dX = X + ((m_Xsize * size) * (NUMdig / static_cast<float>(2)));
    }

  }


  static int Adig = 0;
  static float AY = 0;
  static int Flag = 0;

  if (Adig > NUMdig) {
    Adig = NUMdig;
  }

  //AY = Y;




  if (Flag == 0) {
    AY += speed * delay_time;
    if (AY >= High) {
      AY = High;
      Flag = 1;
    }
  }
  else {
    AY -= speed * delay_time;
    if (AY <= 0) {
      AY = 0;
      Flag = 0;
      Adig++;
      if (Adig > NUMdig) {
        Adig = 0;
      }
    }
  }





  for (int i = 0; i < NUMdig; i++) {

    if (i == Adig) {
      DrawRotaGraph(dX, Y - AY, size, NULL, m_numGraph[dig[i]], TRUE, FALSE);
    }
    else {
      DrawRotaGraph(dX, Y, size, NULL, m_numGraph[dig[i]], TRUE, FALSE);
    }


    dX -= m_Xsize * size;

  }




  delete[] dig;

  return;
}
