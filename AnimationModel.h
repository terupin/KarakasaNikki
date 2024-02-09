#pragma once

#include<SimpleMath.h>
#include"Model_Load.h"

class AnimationModel :public Component
{
private:
	class Model_Load* m_Anim;

public:
	using Component::Component;

	void GetAnim(Model_Load* m_LoadAnim);
	void Update(
		const char* FromAnimName, int FromFrame, 
		const char* ToAnimName, int ToFrame, 
		float BlendRate);
	//void Draw();
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
	static aiMatrix4x4 ChangeAiMatrix(aiMatrix4x4 AiMatrix,DirectX::SimpleMath::Matrix Matrix);
	static DirectX::SimpleMath::Matrix ChangeMatrix(DirectX::SimpleMath::Matrix Matrix, aiMatrix4x4 AiMatrix);

};

