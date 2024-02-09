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
	const char* FromAnimName, int FromFrame, //現在のアニメーション(From側)
	const char* ToAnimName, int ToFrame,  //描画したいアニメーション(To側) 
	float BlendRate)  //混ぜ合わせ率　
{
	// アニメーションありか？
	if (m_Anim->m_Animation.count(FromAnimName) == 0)
		return;
	if (m_Anim->m_Animation.count(ToAnimName) == 0)
		return;

	if (!m_Anim->m_Animation[FromAnimName]->HasAnimations())
		return;
	if (!m_Anim->m_Animation[ToAnimName]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Anim->m_Animation[FromAnimName]->mAnimations[0];
	aiAnimation* animation2 = m_Anim->m_Animation[ToAnimName]->mAnimations[0];

	// 現在のアニメーションについて関連するボーンを全て更新
	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = ToFrame % nodeAnim->mNumRotationKeys;//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = ToFrame % nodeAnim->mNumPositionKeys;//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// 現在のFromアニメーションについて関連するボーンを全て更新
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = FromFrame % nodeAnim->mNumRotationKeys;//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = FromFrame % nodeAnim->mNumPositionKeys;//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// ブレンド
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Anim->m_Bone[nodeAnim->mNodeName.C_Str()];

		// 位置のブレンド
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//線形補間

		// 姿勢のブレンド
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

	UpdateBoneMatrix(m_Anim->m_AiScene->mRootNode, rootMatrix);

	//ボーンコンビネーション行列の配列を生成する
	std::vector<aiMatrix4x4> bonecombmtxcontainer;
	bonecombmtxcontainer.resize(m_Anim->m_Bone.size());
	for (auto data : m_Anim->m_Bone)
	{
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	//転置
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		bcmtx.Transpose();
	}

	//定数バッファに反映させる
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
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Anim->m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）


	bone->Matrix = worldMatrix;			// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

//SimpleMathのMatrixからAiMatrixに変換する関数
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

//AiMatrixからSimpleMathのMatrixに変換する関数
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

