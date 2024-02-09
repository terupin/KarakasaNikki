#pragma once
#include "gameObject.h"

class Box : public GameObject
{

private:
	class Model_Load* m_Model;
	BoundingBoxAABB m_Box ;


public:
	void Init() override;
};