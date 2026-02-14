#include "DxLib.h"
#include "main.h"
#include <Windows.h>

constexpr auto SCREEN_WIDTH = 1920;
constexpr auto SCREEN_HEIGHT = 1080;
constexpr auto SCREEN_FPS = 60;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
  {
    return -1;			// エラーが起きたら直ちに終了
  }

  //基本設定
  SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に設定
  SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);  //画面サイズと色数の設定


  int setup_result = setup();
  if (setup_result == -1) { // setupでエラーが発生した場合
    DxLib_End();
    return -1;
  }

  const unsigned int frame_time = 1000 / SCREEN_FPS;
  unsigned int last_time = GetNowCount();
  unsigned int delay_time = 0;

  while (ProcessMessage() != -1) {
    ClearDrawScreen();    // 画面を一度クリアする
    if (loop() == 1) {
      break;
    }

    if (CheckHitKey(KEY_INPUT_ESCAPE)) { // ESCキーが押されたらループを抜ける(debug用)
      break;
    }

    clsDx();
    printfDx("Delay Time: %u ms\n", delay_time);
    printfDx("FPS: %.2f\n", 1000.0f / delay_time);

    ScreenFlip(); //裏画面と表画面の入れ替え
    //FPS調整
    unsigned int current_time = GetNowCount();
    while (current_time - last_time < frame_time) {
      current_time = GetNowCount();
    }
    delay_time = current_time - last_time;
    SetDelayTime(delay_time / 1000.0f); // デルタタイムを秒単位でセット
    last_time = current_time;
  }

  end();

  DxLib_End();				// ＤＸライブラリ使用の終了処理

  return 0;				// ソフトの終了
}