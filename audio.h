#pragma once

#include <xaudio2.h>
#include "gameObject.h"


class Audio : public Component
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);  //音源データの読み込み
	void Play(bool Loop = false);  //音源の再生


};

