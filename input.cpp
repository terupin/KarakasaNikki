#include "main.h"
#include "input.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
XINPUT_STATE Input::m_PadState;
XINPUT_STATE Input::m_OldPadState;

void Input::Init()
{
	//�L�[�{�[�h�̏󋵂̏�����
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void Input::Uninit()
{
	XInputEnable(false);  //�o�C�u���[�V�����@�\���~�߂��
}

void Input::Update()
{
	memcpy( m_OldKeyState, m_KeyState, 256 );	//�O�̃L�[�{�[�h�̏��̊i�[
	GetKeyboardState( m_KeyState );  //�L�[�{�[�h�̏��̎擾

	m_OldPadState = m_PadState; //�O�̃Q�[���p�b�h�̏��̊i�[
	XInputGetState(0, &m_PadState);  //�Q�[���p�b�h�̏��̎擾
}

//�L�[�������Ă����
bool Input::GetKeyPress(BYTE KeyCode)
{
		return (m_KeyState[KeyCode] & 0x80);
}

//�L�[���������u��
bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetKeyUp(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

//�{�^���������Ă����
bool Input::GetPadButtonPress(WORD BuutonCode)
{
	if (m_OldPadState.Gamepad.wButtons & BuutonCode)
		if (m_PadState.Gamepad.wButtons & BuutonCode)
			return true;

	return false;
}

//�{�^�����������u��
bool Input::GetPadButtonTrigger(WORD BuutonCode)
{
	if (!(m_OldPadState.Gamepad.wButtons & BuutonCode))
		if (m_PadState.Gamepad.wButtons & BuutonCode)
			return true;

	return false;
}

//�{�^�����グ���u��
bool Input::GetPadButtonUP(WORD BuutonCode)
{
	if (m_OldPadState.Gamepad.wButtons & BuutonCode)
		if (!(m_PadState.Gamepad.wButtons & BuutonCode))
			return true;

	return false;
}

//���g���K�[�������Ă����
bool Input::GetPadTrigger_Left()
{
	if (m_PadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)  
		return true;

	return false;
}

//���g���K�[���グ�Ă����
bool Input::GetPadTriggerUp_Left()
{
	if (m_PadState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//�E�g���K�[�������Ă����
bool Input::GetPadTrigger_Right()
{
	if (m_PadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//�E�g���K�[���グ�Ă����
bool Input::GetpadTriggerUp_Right()
{
	if (m_PadState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;

	return false;
}

//���X�e�B�b�N�̏��̎擾
short int Input::GetPadstick_Left_X()
{
	//�V�сi�f�b�h�]�[���j�̔���
	if (m_PadState.Gamepad.sThumbLX<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_PadState.Gamepad.sThumbLX>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_PadState.Gamepad.sThumbLX = 0;
	}

	return m_PadState.Gamepad.sThumbLX;
}

short int Input::GetPadstick_Left_Y()
{
	//�V�сi�f�b�h�]�[���j�̔���
		if (m_PadState.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_PadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_PadState.Gamepad.sThumbLY = 0;
		}

	return m_PadState.Gamepad.sThumbLY;
}






