#pragma once
#include	<cinttypes>
#include	"scene.h"

// �O���Q��
class Application;

class Manager
{
private:
	static class Scene* m_Scene;

public:
	static void Init(Application* ap);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	static class Scene* GetScene()
	{
		return m_Scene;
	}

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->UninitBase();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->InitBase();
	}
};