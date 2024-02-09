#pragma once
#include "gameObject.h"

class Book : public GameObject
{
private:
	class Model_Load* m_Model;
	BoundingBoxAABB m_AABB;
	bool hit_player = false;

public:

	void Init()override;
	void Update()override;
	BoundingBoxAABB GetAABB() { return m_AABB; }


};

