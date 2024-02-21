#pragma once

#include <Windows.h>
#include<xinput.h>
#pragma comment(lib, "xinput.lib")

#ifndef __INPUT_H__
#define __INPUT_H__

class Input
{
private:
	//�L�[�{�[�h�̏��
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	//�R���g���[���[�̏��
	static XINPUT_STATE m_PadState;
	static XINPUT_STATE m_OldPadState;
public:

	static void Init();
	static void Uninit();
	static void Update();

	//�L�[�{�[�h�̏���
	static bool GetKeyPress( BYTE KeyCode );  //�L�[�������Ă����
	static bool GetKeyTrigger( BYTE KeyCode );  //�L�[���������u��
	static bool GetKeyUp(BYTE KeyCode);  //�L�[���グ���u��


	//�Q�[���p�b�h�̃{�^���̏���
	static bool GetPadButtonPress(WORD BuutonCode);  //�{�^���������Ă����
	static bool GetPadButtonTrigger(WORD BuutonCode);  //�{�^�����������u��
	static bool GetPadButtonUP(WORD BuutonCode);  //�{�^�����グ���u��

	//�Q�[���p�b�h�̃g���K�[�̏���  XINPUT_GAMEPAD_TRIGGER_THRESHOLD�c�������o���Ă���臒l�i�������j(�����Ă���ƔF�肳���l)
	static bool GetPadTrigger_Left();  //���̃g���K�[�������Ă����
	static bool GetPadTriggerUp_Left();  //���̃g���K�[���グ�Ă����
	static bool GetPadTrigger_Right();  //�E�g���K�[�������Ă����
	static bool GetpadTriggerUp_Right();  //�E�g���K�[���グ�Ă����

	//�Q�[���p�b�h�̃X�e�B�b�N�̏���
	static short int GetPadstick_Left_X();  //����X���W�X�e�B�b�N���̎擾
	static short int GetPadstick_Left_Y();  //����Y���W�X�e�B�b�N���̎擾
	static short int GetPadstick_Right_X();  //�E��X���W�X�e�B�b�N���̎擾
	static short int GetPadstick_Right_Y(); //�E��Y���W�X�e�B�b�N���̎擾
};
#endif
// __INPUT_H__