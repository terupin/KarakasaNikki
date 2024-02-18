#include	<thread>
#include	<iostream>
#include	<windows.h>
#include	<timeapi.h>

class FPS {
public:
	FPS() = delete;
	FPS(uint64_t f) 
	{
		m_MicrosecondsPerFrame = static_cast<uint64_t>((1000.0f * 1000.0f) / f);
	}
	~FPS() {}

	uint64_t  CalcDelta() {
		m_current_time = timeGetTime();					// ���ݎ����擾
		m_delta_time = m_current_time - m_last_time;	// �O����s������Ɍo�ߎ��Ԃ��擾
		m_last_time = m_current_time;
		return m_delta_time;
	}

	void Wait() {
		int64_t sleep_time = m_MicrosecondsPerFrame - m_delta_time;
		if (sleep_time > 0) {
			float tt = sleep_time / 1000.0f;

			#if defined(DEBUG) || defined(_DEBUG)
				//std::cout << "sleep time(ms):" << tt << std::endl;
			#endif

			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//�w�肵�����Ύ��Ԃ������X���b�h���u���b�N���� (function template)
		}
	}

private:
	uint64_t m_MicrosecondsPerFrame = 0;				// micro second
	uint64_t m_current_time = 0;
	uint64_t m_last_time = 0;
	uint64_t m_delta_time = 0;
};