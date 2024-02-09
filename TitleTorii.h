#pragma once
#include "gameObject.h"

class TitleTorii :  public GameObject
{
private:
	class Model_Load* m_Model;

public:
	void Init() override;


};

