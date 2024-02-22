#pragma once
#include "gameObject.h"

class Box : public GameObject
{

private:
	class Model_Load* m_Model;
	BoundingBoxAABB m_Box ;

public:
	BoundingBoxAABB& GetAABB() { return m_Box; }
	Model_Load& GetModel() { return *m_Model; }
	void Init() override;
};