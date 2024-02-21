#pragma once

#include <Windows.h>
#include<xinput.h>
#pragma comment(lib, "xinput.lib")

#ifndef __INPUT_H__
#define __INPUT_H__

class Input
{
private:
	//キーボードの状態
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	//コントローラーの状態
	static XINPUT_STATE m_PadState;
	static XINPUT_STATE m_OldPadState;
public:

	static void Init();
	static void Uninit();
	static void Update();

	//キーボードの処理
	static bool GetKeyPress( BYTE KeyCode );  //キーを押している間
	static bool GetKeyTrigger( BYTE KeyCode );  //キーを押した瞬間
	static bool GetKeyUp(BYTE KeyCode);  //キーを上げた瞬間


	//ゲームパッドのボタンの処理
	static bool GetPadButtonPress(WORD BuutonCode);  //ボタンを押している間
	static bool GetPadButtonTrigger(WORD BuutonCode);  //ボタンを押した瞬間
	static bool GetPadButtonUP(WORD BuutonCode);  //ボタンを上げた瞬間

	//ゲームパッドのトリガーの処理  XINPUT_GAMEPAD_TRIGGER_THRESHOLD…公式が出している閾値（いきち）(押していると認定される値)
	static bool GetPadTrigger_Left();  //左のトリガーを押している間
	static bool GetPadTriggerUp_Left();  //左のトリガーを上げている間
	static bool GetPadTrigger_Right();  //右トリガーを押している間
	static bool GetpadTriggerUp_Right();  //右トリガーを上げている間

	//ゲームパッドのスティックの処理
	static short int GetPadstick_Left_X();  //左のX座標スティック情報の取得
	static short int GetPadstick_Left_Y();  //左のY座標スティック情報の取得
	static short int GetPadstick_Right_X();  //右のX座標スティック情報の取得
	static short int GetPadstick_Right_Y(); //右のY座標スティック情報の取得
};
#endif
// __INPUT_H__