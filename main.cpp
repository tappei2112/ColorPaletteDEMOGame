#include "ASerial_lib_Controller_Win/ASerial_lib_Controller_Win.h"
#include "ASerial_lib_Controller_Win/WindowsSerial/WindowsSerial.h"
#include "DigitDisplay_class.h"
#include "Dxlib.h"
#include "main.h"
#include "StateEnums.h"
#include <cstdint>
#include <Windows.h>

//ASerial設定
constexpr auto TARGET_DEVICE_ID = 0x06; // ターゲットデバイスID
constexpr auto TARGET_DEVICE_VER = 0x01; // ターゲットデバイスバージョン

//定数
constexpr auto DISPLAY_CENTER_X = 1920 / 2; // 数字表示X座標(画面中央)
constexpr auto DISPLAY_CENTER_Y = 1080 / 2; // 数字表示Y座標(画面中央)

namespace Instance {
  WindowsSerial Serial(9600); // シリアル通信インターフェース
  ASerial_lib_Controller_Win serial_controller(TARGET_DEVICE_ID, TARGET_DEVICE_VER); // ASerialコントローラオブジェクト
  DigitDisplay digit_display; // 数字表示クラスオブジェクト
  DigitDisplay dot_digit_display; // ドット数字表示クラスオブジェクト
}

//graph handles
namespace GraphHandles {
  int title_logo = 0; // タイトルロゴ
  int next_amusement_logo_mov = 0;  // Next Amusementロゴムービー
  int arduino_dxlib_logo_mov = 0; // Arduino＆Dxlibロゴムービー
  int background_01 = 0;  // 背景画像1
  int background_02 = 0;  // 背景画像2
  int background_03 = 0;  // 背景画像3
  int background_04 = 0;  // 背景画像4
  int RuleBoard_1 = 0; // ルール説明ボード1
  int RuleBoard_2 = 0; // ルール説明ボード2
  int RuleBoard_3 = 0; // ルール説明ボード3
  int RuleBoard_4 = 0; // ルール説明ボード4
  int RuleBoard_5 = 0; // ルール説明ボード5
  int X = 0; //バツ印グラフィックハンドル
  int ren = 0; //連グラフィックハンドル
  int standby = 0; //待機画面グラフィックハンドル
  int start = 0; //スタート画面グラフィックハンドル
  int win = 0; //勝利画面グラフィックハンドル
  int end = 0; //終了画面グラフィックハンドル
  int x1 = 0;
  int x2 = 0;
  int x3 = 0;
  int x4 = 0;
  int x5 = 0;
  int x7 = 0;
  int colors = 0;
  int digit_font_handles[10] = { 0 }; // 数字フォントグラフィックハンドル配列
  int dot_digit_font_handles[10] = { 0 }; // ドット数字フォントグラフィックハンドル配列
}

namespace AudioHandles {
  int se_beach_wave = 0;  // 波のSE
  int se_card_flip = 0;   // カードめくりSE
  int BGM_1 = 0;          // BGM1
  int BGM_2 = 0;          // BGM2
  int BGM_3 = 0;          // BGM3
}

// global variables
int g_game_state = GAME_STATE_TITLE; // ゲーム状態管理変数
int g_old_game_state = -1; // 前回のゲーム状態管理変数
float g_delay_time = 0.0f; // デルタタイム管理変数
bool g_space_key_prev_state = false; // スペースキーの前回状態管理変数
int g_total_win = 0; // 総獲得金額管理変数


//プロトタイプ宣言
void ChangeGameState(int new_state);
int CheckHitSpaceKey(void);


int setup(void)
{
  printfDx("Setup Start...\n");
  WaitTimer(1000);

  printfDx("Connecting to Device...\n");
  WaitTimer(1000);

  // インターフェースを設定
  Instance::serial_controller.SetInterfacePt(&Instance::Serial);
  int result = Instance::serial_controller.AutoConnectDevice();
  if (result == -1) {
    printfDx("Device Connect Failed.\n");
  }
  else {
    printfDx("Device Connected Successfully!\n");
    printfDx("Connected COM%d\n", Instance::Serial.GetConnectCOM());
  }
  WaitTimer(2000);

  if (Instance::serial_controller.GetConnectionState() == true) {
    printfDx("Initializing Lottery System...\n");

    Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_ORIGIN_SEARCH);
    WaitTimer(25000); // 原点検出待機時間
    int wait_count = 0;
    while (1) {
      Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_GET_SYSTEM_STATUS);
      ASerialDataStruct::ASerialData read_data{};
      int result = Instance::serial_controller.ReadData(&read_data);
      if (result == 0) {
        if (read_data.data[0] == SYSTEM_STATE_IDLE) {
          printfDx("Lottery System Initialized Successfully!\n");
          WaitTimer(2000);
          break;
        }
      }
      else {
        wait_count++;
        if (wait_count > 20) {
          printfDx("Lottery System Initialization Failed.\n");
          WaitTimer(2000);
          break;
        }
      }
    }
  }

  ///===リソース読み込み===

  printfDx("Loading Graph Resources...\n");

  GraphHandles::title_logo = LoadGraph("Assets/image/TitleLogo.png");
  GraphHandles::next_amusement_logo_mov = LoadGraph("Assets/movie/プロジェクトロゴアニメーション.mp4");
  GraphHandles::arduino_dxlib_logo_mov = LoadGraph("Assets/movie/Dxlib＆Arduinoロゴアニメーション.mov");
  GraphHandles::background_01 = LoadGraph("Assets/image/Pier-in-a-port-town3.jpg");
  GraphHandles::background_02 = LoadGraph("Assets/image/Pixel-art-style-coast3.jpg");
  GraphHandles::background_03 = LoadGraph("Assets/image/Pixel-art-style-lighthouse1.jpg");
  GraphHandles::background_04 = LoadGraph("Assets/image/Pixel-art-sailing-ship2.jpg");
  GraphHandles::RuleBoard_1 = LoadGraph("Assets/image/RuleBoard01.png");
  GraphHandles::RuleBoard_2 = LoadGraph("Assets/image/RuleBoard02.png");
  GraphHandles::RuleBoard_3 = LoadGraph("Assets/image/RuleBoard03.png");
  GraphHandles::RuleBoard_4 = LoadGraph("Assets/image/RuleBoard04.png");
  GraphHandles::RuleBoard_5 = LoadGraph("Assets/image/RuleBoard05.png");
  GraphHandles::X = LoadGraph("Assets/image/X.png");
  GraphHandles::ren = LoadGraph("Assets/image/ren.png");
  GraphHandles::standby = LoadGraph("Assets/image/standby.png");
  GraphHandles::start = LoadGraph("Assets/image/start.png");
  GraphHandles::win = LoadGraph("Assets/image/win_font.png");
  GraphHandles::end = LoadGraph("Assets/image/end.png");
  GraphHandles::x1 = LoadGraph("Assets/image/x1.png");
  GraphHandles::x2 = LoadGraph("Assets/image/x2.png");
  GraphHandles::x3 = LoadGraph("Assets/image/x3.png");
  GraphHandles::x4 = LoadGraph("Assets/image/x4.png");
  GraphHandles::x5 = LoadGraph("Assets/image/x5.png");
  GraphHandles::x7 = LoadGraph("Assets/image/x7.png");
  GraphHandles::colors = LoadGraph("Assets/image/colors.png");
  LoadDivGraph("Assets/image/TH1numberFont.png", 10, 10, 1, 600, 800, GraphHandles::digit_font_handles);
  LoadDivGraph("Assets/image/dotNumber_BK.png", 10, 10, 1, 300, 480, GraphHandles::dot_digit_font_handles);


  printfDx("End Loading Graph Resources.\n");

  printfDx("Loading Audio Resources...\n");

  AudioHandles::se_beach_wave = LoadSoundMem("Assets/Audio/SE/カモメが鳴くビーチ.mp3");
  AudioHandles::se_card_flip = LoadSoundMem("Assets/Audio/SE/カードをめくる.mp3");

  AudioHandles::BGM_1 = LoadSoundMem("Assets/Audio/BGM/魔王魂ループファンタジー02.mp3");
  AudioHandles::BGM_2 = LoadSoundMem("Assets/Audio/BGM/魔王魂ループオーケストラ16.mp3");
  AudioHandles::BGM_3 = LoadSoundMem("Assets/Audio/BGM/maou_loop_bgm_fantasy05.mp3");

  printfDx("End Loading Audio Resources.\n");

  //ポケットの設定
  uint8_t poket_data[5] = { 0, 0xFF, 0xFF, 0xFF, EffectType::EFFECT_NONE };
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 1;
  poket_data[1] = 0x00;
  poket_data[2] = 0xFF;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 2;
  poket_data[1] = 0x00;
  poket_data[2] = 0x00;
  poket_data[3] = 0x00;
  poket_data[4] = EFFECT_RAINBOW_HIGH_REVERSE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 3;
  poket_data[1] = 0xFF;
  poket_data[2] = 0x00;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 4;
  poket_data[1] = 0x00;
  poket_data[2] = 0xFF;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 5;
  poket_data[1] = 0x00;
  poket_data[2] = 0x00;
  poket_data[3] = 0xFF;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 6;
  poket_data[1] = 0xFF;
  poket_data[2] = 0xFF;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 7;
  poket_data[1] = 0x00;
  poket_data[2] = 0xFF;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 8;
  poket_data[1] = 0xFF;
  poket_data[2] = 0x00;
  poket_data[3] = 0x00;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);
  poket_data[0] = 9;
  poket_data[1] = 0x00;
  poket_data[2] = 0x00;
  poket_data[3] = 0xFF;
  poket_data[4] = EffectType::EFFECT_NONE;
  Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_POKET_COLOR, poket_data, 5);


  Instance::digit_display.InportHandle(GraphHandles::digit_font_handles);
  Instance::dot_digit_display.InportHandle(GraphHandles::dot_digit_font_handles);

  clsDx();
  return 0;
}

int loop(void)
{
  switch (g_game_state) {
    case GAME_STATE_TITLE:
    {
      static bool title_logo_play_flag = true;
      static int movie_play_time = GetNowCount();
      static float logo_y = 0.0f;
      static bool logo_down_flag = true;
      const int speed = 20; // ロゴの移動速度

      if (g_old_game_state != g_game_state) {
        uint8_t idle_animation = IdleAnimationState::IDLE_ANIMATION_STATE_COLOR_WIPE;
        Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SET_IDLE_ANIMATION, &idle_animation, 1);
        PlaySoundMem(AudioHandles::se_beach_wave, DX_PLAYTYPE_LOOP); // 波のSE再生
        movie_play_time = GetNowCount();
        title_logo_play_flag = true;
        g_old_game_state = g_game_state;
      }

      DrawGraph(0, 0, GraphHandles::background_02, TRUE);
      DrawGraph(0, logo_y, GraphHandles::title_logo, TRUE);

      if (logo_down_flag == true) {
        logo_y += speed * g_delay_time;
        if (logo_y >= 50.0f) {
          logo_down_flag = false;
        }
      }
      else {
        logo_y -= speed * g_delay_time;
        if (logo_y <= 0.0f) {
          logo_down_flag = true;
        }
      }

      if (title_logo_play_flag == true) {
        StopSoundMem(AudioHandles::se_beach_wave); // 波のSE停止
        const int play_movie_num = 2 - 1; // 再生するムービーの数 -1
        static int play_count = 0;
        static bool play_started = false;
        static int play_handle = 0;
        if (play_started == false) {
          switch (play_count) {
            case 0:
            {
              play_handle = GraphHandles::next_amusement_logo_mov;
              PlayMovieToGraph(play_handle);
              break;
            }
            case 1:
            {
              play_handle = GraphHandles::arduino_dxlib_logo_mov;
              PlayMovieToGraph(play_handle);
              break;
            }
            default:
            {
              break;
            }
          }
          play_started = true;
        }

        if (GetMovieStateToGraph(play_handle) == 0) {
          if (play_count == play_movie_num) {
            title_logo_play_flag = false;
            play_count = 0;
            play_started = false;
            movie_play_time = GetNowCount();  // ロゴ表示時間計測開始
            SeekMovieToGraph(play_handle, 0); // ムービーを先頭に戻す
            PlaySoundMem(AudioHandles::se_beach_wave, DX_PLAYTYPE_LOOP); // 波のSE再生
          }
          else {
            SeekMovieToGraph(play_handle, 0); // ムービーを先頭に戻す
            play_started = false;
            play_count++;
          }
        }

        DrawGraph(0, 0, play_handle, TRUE);
      }

      if (GetNowCount() - movie_play_time >= 60000 && title_logo_play_flag == false) { //一定時間何も操作がなければムービー再生
        title_logo_play_flag = true;
      }

      if (CheckHitSpaceKey() == 1 && title_logo_play_flag == false) {
        ChangeGameState(GAME_STATE_RULE);
        StopSoundMem(AudioHandles::se_beach_wave); // 波のSE停止
      }

      break;
    }
    case GAME_STATE_RULE:
    {
      static int rule_page = 1;
      static int next_page_timer = GetNowCount();
      static int rule_read_timer = GetNowCount();
      if (g_old_game_state != g_game_state) { // ゲーム状態が変化したとき初期化
        rule_page = 1;
        next_page_timer = GetNowCount();
        rule_read_timer = GetNowCount();
        PlaySoundMem(AudioHandles::BGM_1, DX_PLAYTYPE_LOOP); // BGM2再生
        g_old_game_state = g_game_state;
        Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_ORIGIN_SEARCH);
      }

      DrawGraph(0, 0, GraphHandles::background_01, TRUE);
      const int x = DISPLAY_CENTER_X - 400; // ルールボードX座標(画面中央合わせ)
      const int y = DISPLAY_CENTER_Y - 300; // ルールボードY座標(画面中央合わせ)

      switch (rule_page) {
        case 1:
        {
          DrawGraph(x, y, GraphHandles::RuleBoard_1, TRUE);
          break;
        }
        case 2:
        {
          DrawGraph(x, y, GraphHandles::RuleBoard_2, TRUE);
          break;
        }
        case 3:
        {
          DrawGraph(x, y, GraphHandles::RuleBoard_3, TRUE);
          break;
        }
        case 4:
        {
          DrawGraph(x, y, GraphHandles::RuleBoard_4, TRUE);
          break;
        }
        case 5:
        {
          DrawGraph(x, y, GraphHandles::RuleBoard_5, TRUE);
          break;
        }
        default:
          break;
      }

      if (GetNowCount() - next_page_timer >= 5000) { // 5秒ごとに次のページへ
        PlaySoundMem(AudioHandles::se_card_flip, DX_PLAYTYPE_BACK); // カードめくりSE再生
        rule_page++;
        if (rule_page > 5) {
          rule_page = 1;
        }
        next_page_timer = GetNowCount();
      }

      static int serial_read_timer = GetNowCount();

      if (GetNowCount() - serial_read_timer >= 2000 && GetNowCount() - rule_read_timer >= 30000) { // 2秒ごとにシステムステータス取得
        serial_read_timer = GetNowCount();
        ASerialDataStruct::ASerialData read_data{};
        Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_GET_SYSTEM_STATUS);
        int result = Instance::serial_controller.ReadData(&read_data);
        if (result == 0) {
          if (read_data.data[0] == SYSTEM_STATE_IDLE) {
            ChangeGameState(GAME_STATE_PLAYING);
            StopSoundMem(AudioHandles::BGM_1); // BGM2停止
          }
        }
      }

      DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 430, 0.2, 0.0, GraphHandles::standby, TRUE);

      break;
    }
    case GAME_STATE_PLAYING:  // ゲームプレイ中
    {
      static int phase = 0;
      static int count = 0;
      static int magnification = 1;
      static int wait_timer = GetNowCount();
      static int magnification_graph_handle = GraphHandles::x1;
      static int in_pocket = 0;
      static int win = 0;
      if (g_old_game_state != g_game_state) {
        phase = 0;
        count = 0;
        magnification = 1;
        magnification_graph_handle = GraphHandles::x1;
        in_pocket = 0;
        wait_timer = GetNowCount();
        g_total_win = 0;  // 総獲得金額リセット
        PlaySoundMem(AudioHandles::BGM_2, DX_PLAYTYPE_LOOP); // BGM2再生
        g_old_game_state = g_game_state;
      }

      DrawGraph(0, 0, GraphHandles::background_04, TRUE); // 背景描画
      DrawGraph(0, 0, GraphHandles::colors, TRUE); // カラ-描画

      //連チャン倍率表示
      if (count < 10) { //10連チャン
        magnification = 1;
        magnification_graph_handle = GraphHandles::x1;
      }
      else if (count < 20) {//20連チャン
        magnification = 2;
        magnification_graph_handle = GraphHandles::x2;
      }
      else if (count < 30) {//30連チャン
        magnification = 3;
        magnification_graph_handle = GraphHandles::x3;
      }
      else if (count < 40) {//40連チャン
        magnification = 4;
        magnification_graph_handle = GraphHandles::x4;
      }
      else if (count < 50) {//50連チャン
        magnification = 5;
        magnification_graph_handle = GraphHandles::x5;
      }
      else {//60連チャン以上
        magnification = 7;
        magnification_graph_handle = GraphHandles::x7;
      }

      DrawGraph(1920 - 500, 0, magnification_graph_handle, TRUE); //倍率表示

      //トータル獲得金額表示
      Instance::digit_display.Display(g_total_win, DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 250, 0.5f);
      DrawRotaGraph(1920 - 400, DISPLAY_CENTER_Y + 250, 0.5f, 0.0f, GraphHandles::win, TRUE);

      //連チャン数表示
      Instance::dot_digit_display.Display(count, DISPLAY_CENTER_X, 200, 0.5f);
      DrawRotaGraph(DISPLAY_CENTER_X + 300, 200, 0.5f, 0.0f, GraphHandles::ren, TRUE);

      switch (phase) {
        case 0: //抽選の一番初めの時だけ
        {
          DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, 0.4, 0.0, GraphHandles::start, TRUE);
          Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_SHOW_POKET);
          if (GetNowCount() - wait_timer >= 5000) { //5秒待機
            phase = 1;
            wait_timer = GetNowCount();
          }
          break;
        }
        case 1: //抽選開始
        {
          //DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, 0.4, 0.0, GraphHandles::start, TRUE);
          if (GetNowCount() - wait_timer >= 1000) { //1秒待機
            Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_START_LOTTERY);
            phase = 2;
            wait_timer = GetNowCount();
          }
          break;
        }
        case 2: //抽選中
        {
          ASerialDataStruct::ASerialData read_data{};
          Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_GET_SYSTEM_STATUS);
          int result = Instance::serial_controller.ReadData(&read_data);
          if (result == 0) {
            if (read_data.data[0] == SYSTEM_STATE_LOTTERY_END) {
              phase = 3;
              wait_timer = GetNowCount();
            }
          }
          break;
        }
        case 3: //抽選結果取得
        {
          ASerialDataStruct::ASerialData read_data{};
          Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_GET_LOTTERY_RESULT);
          int result = Instance::serial_controller.ReadData(&read_data);
          if (result == 0) {
            in_pocket = read_data.data[0];
            wait_timer = GetNowCount();
            phase = 4;
          }
          break;
        }
        case 4:
        {

          win = 0;
          switch (in_pocket) {
            case 0:win = 0; break;
            case 1:win = 3; break;
            case 2:win = 48; break;
            case 3:win = 12; break;
            case 4:win = 3; break;
            case 5:win = 6; break;
            case 6:win = 24; break;
            case 7:win = 3; break;
            case 8:win = 12; break;
            case 9:win = 6; break;
          }

          win *= magnification; // 獲得金額に倍率をかける
          g_total_win += win; // 総獲得金額に加算

          wait_timer = GetNowCount();

          phase = 5;
        }
        case 5:
        {
          if (in_pocket != 0) {
            Instance::digit_display.Display(win, DISPLAY_CENTER_X, DISPLAY_CENTER_Y - 100, 1.0f);
          }
          else {
            DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, 2.5f, 0.0, GraphHandles::X, TRUE);
            DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 200, 0.5, 0.0, GraphHandles::end, TRUE);
          }

          if (GetNowCount() - wait_timer >= 3000) {
            if (in_pocket == 0) { //ハズレの場合
              count = 0; // 連チャンカウントリセット
              phase = 0; // 抽選フェーズリセット
              StopSoundMem(AudioHandles::BGM_2); // BGM2停止
              Instance::serial_controller.WriteData(AserialColorPaletteCommand::COMMAND_IDLE_MODE); // 待機モードへ
              ChangeGameState(GAME_STATE_RESULT); // 結果表示画面へ移行
            }
            else {
              count++; // 連チャンカウント増加
              phase = 1; // 抽選フェーズを抽選開始へ
            }
          }
          break;
        }
        default:
          break;
      }


      break;
    }
    case GAME_STATE_RESULT:
    {
      static int display_timer = GetNowCount();
      if (g_old_game_state != g_game_state) {
        PlaySoundMem(AudioHandles::BGM_3, DX_PLAYTYPE_LOOP); // BGM3再生
        g_old_game_state = g_game_state;
        display_timer = GetNowCount();
      }
      DrawGraph(0, 0, GraphHandles::background_03, TRUE); // 背景描画
      Instance::digit_display.Display(g_total_win, DISPLAY_CENTER_X, DISPLAY_CENTER_Y, 0.8f);
      DrawRotaGraph(DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 320, 0.5f, 0.0f, GraphHandles::win, TRUE);

      if (GetNowCount() - display_timer >= 30000) { //10秒表示したらタイトルへ戻る
        StopSoundMem(AudioHandles::BGM_3); // BGM3停止
        ChangeGameState(GAME_STATE_TITLE);
      }
      break;
    }
    case GAME_STATE_GAME_OVER:
    {
      break;
    }
    case GAME_STATE_TEST:
    {
      break;
    }
    default:
      break;
  }

  return 0;
}

int end(void)
{
  clsDx();
  ClearDrawScreen();
  ScreenFlip(); //裏画面と表画面の入れ替え

  printfDx("End Game...");
  WaitTimer(2000);

  if (Instance::serial_controller.GetConnectionState() == true) {
    Instance::serial_controller.DisConnectDevice();
  }
  return 0;
}

void ChangeGameState(int new_state)
{
  g_old_game_state = g_game_state;
  g_game_state = new_state;
}

int CheckHitSpaceKey(void)
{
  bool current_state = (CheckHitKey(KEY_INPUT_SPACE) != 0);
  int result = 0;
  if (current_state == true && g_space_key_prev_state == false) {
    result = 1; // スペースキーが押された瞬間
  }
  g_space_key_prev_state = current_state;
  return result;
}

void SetDelayTime(float delay_time)
{
  g_delay_time = delay_time;
}
