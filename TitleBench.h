#pragma once
#include"gameObject.h"
#include"AnimationModel.h"

class TitleBench : public GameObject
{
private:
	class Model_Load* m_Model;
	BoundingBoxAABB m_Box;
public:
	void Init();
};

