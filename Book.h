#pragma once
#include "gameObject.h"

class Book : public GameObject
{
private:
	class Model_Load* m_Model;
	BoundingBoxAABB m_AABB;
	bool hit_player = false;

public:
	BoundingBoxAABB& GetAABB() { return m_AABB; }
	Model_Load& GetModel() { return *m_Model; }

	void Init()override;
	void Update()override;


};

