#pragma once

//state enums
enum GameState
{
  GAME_STATE_TITLE = 0, //タイトル画面
  GAME_STATE_RULE,  //ルール説明画面
  GAME_STATE_PLAYING, //ゲームプレイ中
  GAME_STATE_RESULT, //結果表示画面
  GAME_STATE_GAME_OVER, //ゲームオーバー画面
  GAME_STATE_TEST, //テスト画面
};

enum SystemState
{               // システム状態
  SYSTEM_STATE_IDLE = 0,       // 待機状態
  SYSTEM_STATE_ORIGIN_SEARCH,  // 原点検出状態
  SYSTEM_STATE_POKET_SHOW,     // ポケット表示状態
  SYSTEM_STATE_LOTTERY,        // 抽選
  SYSTEM_STATE_LOTTERY_END,    // 抽選終了状態
  SYSTEM_STATE_ERROR,          // エラー状態
};

enum LotteryState
{                  // 抽選状態
  LOTTERY_STATE_SPINUP = 0,        // スピンアップ状態
  LOTTERY_STATE_DECELERATE,        // 減速状態
  LOTTERY_STATE_LOW_SPEED_ROTATE,  // 低速回転状態
  LOTTERY_STATE_STOP_WAIT,         // 停止待機状態
};

enum IdleAnimationState
{                // 待機アニメーション状態
  IDLE_ANIMATION_STATE_OFF = 0,        // アニメーションOFF
  IDLE_ANIMATION_STATE_COLOR_WIPE,     // カラー拭き取り
  IDLE_ANIMATION_STATE_THEATER_CHASE,  // シアター・チェイス
  IDLE_ANIMATION_STATE_RAINBOW_CYCLE,  // レインボー・サイクル
};

enum ErrorCode
{               // エラーコード
  ERROR_CODE_NONE = 0,       // エラーなし
  ERROR_CODE_ORIGIN_SENSOR,  // 原点センサーエラー
  ERROR_CODE_TIMEOUT,        // タイムアウトエラー
  ERROR_CODE_UNKNOWN,        // 不明なエラー
};

enum AserialColorPaletteCommand
{    // ASerial ColorPaletteコマンド
  COMMAND_SET_POKET_COLOR = 0x20,  // ポケットカラー設定コマンド
  COMMAND_SET_IDLE_ANIMATION,      // 待機アニメーション設定コマンド
  COMMAND_GET_SYSTEM_STATUS,       // システムステータス取得コマンド
  COMMAND_GET_LOTTERY_STATUS,      // 抽選ステータス取得コマンド
  COMMAND_START_LOTTERY,           // 抽選開始コマンド
  COMMAND_IDLE_MODE,               // 待機モード設定コマンド
  COMMAND_SHOW_POKET,              // ポケット表示コマンド
  COMMAND_GET_LOTTERY_RESULT,      // 抽選結果取得コマンド
  COMMAND_ORIGIN_SEARCH,           // 原点検出コマンド
};

enum EffectType
{
  EFFECT_NONE = 0,    //エフェクトなし
  EFFECT_BLINK_LOW,   //点滅(低速)
  EFFECT_BLINK_MEDIUM,    //点滅(中速)
  EFFECT_BLINK_HIGH,  //点滅(高速)
  EFFECT_FADE_LOW,   //フェードイン・アウト(低速)
  EFFECT_FADE_MEDIUM, //フェードイン・アウト(中速)
  EFFECT_FADE_HIGH,  //フェードイン・アウト(高速)
  EFFECT_SPECTRUM_LOW,    //スペクトラム(低速)*色は無視
  EFFECT_SPECTRUM_MEDIUM, //スペクトラム(中速)*色は無視
  EFFECT_SPECTRUM_HIGH,   //スペクトラム(高速)*色は無視
  EFFECT_RAINBOW_LOW,  //レインボー(低速)*色は無視
  EFFECT_RAINBOW_MEDIUM,   //レインボー(中速)*色は無視
  EFFECT_RAINBOW_HIGH, //レインボー(高速)*色は無視
  EFFECT_RAINBOW_LOW_REVERSE,  //レインボー逆回転(低速)*色は無視
  EFFECT_RAINBOW_MEDIUM_REVERSE,   //レインボー逆回転(中速)*色は無視
  EFFECT_RAINBOW_HIGH_REVERSE //レインボー逆回転(高速)*色は無視
};