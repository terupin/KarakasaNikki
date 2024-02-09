#include "main.h"
#include "input.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
XINPUT_STATE Input::m_PadState;
XINPUT_STATE Input::m_OldPadState;

void Input::Init()
{
	//キーボードの状況の初期化
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void Input::Uninit()
{
	XInputEnable(false);  //バイブレーション機能を止める為
}

void Input::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );	//前のキーボードの情報の格納
	GetKeyboardState( m_KeyState );  //キーボードの情報の取得

	m_OldPadState = m_PadState; //前のゲームパッドの情報の格納
	XInputGetState(0, &m_PadState);  //ゲームパッドの情報の取得
}

//キーを押している間
bool Input::GetKeyPress(BYTE KeyCode)
{
		return (m_KeyState[KeyCode] & 0x80);
}

//キーを押した瞬間
bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetKeyUp(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

//ボタンを押している間
bool Input::GetPadButtonPress(WORD BuutonCode)
{
	if (m_OldPadState.Gamepad.wButtons & BuutonCode)
		if (m_PadState.Gamepad.wButtons & BuutonCode)
			return true;

	return false;
}

//ボタンを押した瞬間
bool Input::GetPadButtonTrigger(WORD BuutonCode)
{
	if (!(m_OldPadState.Gamepad.wButtons & BuutonCode))
		if (m_PadState.Gamepad.wButtons & BuutonCode)
			return true;

	return false;
}

//ボタンを上げた瞬間
bool Input::GetPadButtonUP(WORD BuutonCode)
{
	if (m_OldPadState.Gamepad.wButtons & BuutonCode)
		if (!(m_PadState.Gamepad.wButtons & BuutonCode))
			return true;

	return false;
}

//左トリガーを押している間
bool Input::GetPadTrigger_Left()
{
	if (m_PadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)  
		return true;

	return false;
}

//左トリガーを上げている間
bool Input::GetPadTriggerUp_Left()
{
	if (m_PadState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//右トリガーを押している間
bool Input::GetPadTrigger_Right()
{
	if (m_PadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//右トリガーを上げている間
bool Input::GetpadTriggerUp_Right()
{
	if (m_PadState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//左スティックの情報の取得
short int Input::GetPadstick_Left_X()
{
	//遊び（デッドゾーン）の判定
	if (m_PadState.Gamepad.sThumbLX<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_PadState.Gamepad.sThumbLX>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_PadState.Gamepad.sThumbLX = 0;
	}

	return m_PadState.Gamepad.sThumbLX;
}

short int Input::GetPadstick_Left_Y()
{
	//遊び（デッドゾーン）の判定
		if (m_PadState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_PadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_PadState.Gamepad.sThumbLY = 0;
		}

	return m_PadState.Gamepad.sThumbLY;
}






