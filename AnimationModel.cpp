#include<SimpleMath.h>
#include"main.h"
#include "AnimationModel.h"
#include"renderer.h"
#include<iostream>

using namespace DirectX::SimpleMath;

void AnimationModel::GetAnim(Model_Load* m_LoadAnim)
{
	m_Anim = m_LoadAnim;
}

void AnimationModel::Update(
	const char* FromAnimName, int FromFrame, //���݂̃A�j���[�V����(From��)
	const char* ToAnimName, int ToFrame,  //�`�悵�����A�j���[�V����(To��) 
	float BlendRate)  //�������킹���@
{
	// �A�j���[�V�������肩�H
	if (m_Anim->m_Animation.count(FromAnimName) == 0)
		return;
	if (m_Anim->m_Animation.count(ToAnimName) == 0)
		return;

	if (!m_Anim->m_Animation[FromAnimName]->HasAnimations())
		return;
	if (!m_Anim->m_Animation[ToAnimName]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Anim->m_Animation[FromAnimName]->mAnimations[0];
	aiAnimation* animation2 = m_Anim->m_Animation[ToAnimName]->mAnimations[0];

	// ���݂̃A�j���[�V�����ɂ��Ċ֘A����{�[����S�čX�V
	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = ToFrame % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = ToFrame % nodeAnim->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// ���݂�From�A�j���[�V�����ɂ��Ċ֘A����{�[����S�čX�V
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = FromFrame % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = FromFrame % nodeAnim->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// �u�����h
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		// �ʒu�̃u�����h
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//���`���

		// �p���̃u�����h
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//���ʐ��`���

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

	UpdateBoneMatrix(m_Anim->m_AiScene->mRootNode, rootMatrix);

	//�{�[���R���r�l�[�V�����s��̔z��𐶐�����
	std::vector<aiMatrix4x4> bonecombmtxcontainer;
	bonecombmtxcontainer.resize(m_Anim->m_Bone.size());
	for (auto data : m_Anim->m_Bone)
	{
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	//�]�u
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		bcmtx.Transpose();
	}

	//�萔�o�b�t�@�ɔ��f������
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_Anim->m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr))
	{
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_Anim->m_BoneCombMtxCBuffer, 0);
	}

}


void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	BONE* bone = &m_Anim->m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��

	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j


	bone->Matrix = worldMatrix;			// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

//SimpleMath��Matrix����AiMatrix�ɕϊ�����֐�
aiMatrix4x4 AnimationModel::ChangeAiMatrix(aiMatrix4x4 AiMatrix, DirectX::SimpleMath::Matrix Matrix)
{
	AiMatrix.a1 = Matrix._11;
	AiMatrix.a2 = Matrix._12;
	AiMatrix.a3 = Matrix._13;
	AiMatrix.a4 = Matrix._14;
	AiMatrix.b1 = Matrix._21;
	AiMatrix.b2 = Matrix._22;
	AiMatrix.b3 = Matrix._23;
	AiMatrix.b4 = Matrix._24;
	AiMatrix.c1 = Matrix._31;
	AiMatrix.c2 = Matrix._32;
	AiMatrix.c3 = Matrix._33;
	AiMatrix.c4 = Matrix._34;
	AiMatrix.d1 = Matrix._41;
	AiMatrix.d2 = Matrix._42;
	AiMatrix.d3 = Matrix._43;
	AiMatrix.d4 = Matrix._44;
	return AiMatrix;
}

//AiMatrix����SimpleMath��Matrix�ɕϊ�����֐�
Matrix AnimationModel::ChangeMatrix(DirectX::SimpleMath::Matrix Matrix, aiMatrix4x4 AiMatrix)
{
	Matrix._11 = AiMatrix.a1;
	Matrix._12 = AiMatrix.a2;
	Matrix._13 = AiMatrix.a3;
	Matrix._14 = AiMatrix.a4;
	Matrix._21 = AiMatrix.b1;
	Matrix._22 = AiMatrix.b2;
	Matrix._23 = AiMatrix.b3;
	Matrix._24 = AiMatrix.b4;
	Matrix._31 = AiMatrix.c1;
	Matrix._32 = AiMatrix.c2;
	Matrix._33 = AiMatrix.c3;
	Matrix._34 = AiMatrix.c4;
	Matrix._41 = AiMatrix.d1;
	Matrix._42 = AiMatrix.d2;
	Matrix._43 = AiMatrix.d3;
	Matrix._44 = AiMatrix.d4;
	Matrix = Matrix.Transpose();
	
	return Matrix;
}

