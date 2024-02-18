#pragma once

#include "gameObject.h"

class Sky : public GameObject
{

private:
	class Model_Load* m_Model;
public:
	void Init() override;
	void Update() override;

};