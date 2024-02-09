#pragma once
#include	<SimpleMath.h>
#include<iostream>

#include	"renderer.h"
#include	"collision.h"

class CBoundintSphere
{
private:
	DirectX::SimpleMath::Vector3 m_Center;  //���S���W
	float m_Radius;  //���a

	DirectX::SimpleMath::Vector3 m_Max;  //AABB �ő�
	DirectX::SimpleMath::Vector3 m_Min;  //AABB �ŏ�

	//���[���h���W�̌v�Z(��܃I�u�W�F�N�g�̍s�񂪓����Ă���)
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix mtx);

public:
	//BS���𐶐�����
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	//���a��߂�
	float GetRadius();

	//���S���W��߂�
	DirectX::SimpleMath::Vector3 GetCenter();

	//BS��񐶐�
	BoundingSphere MakeBS(
		DirectX::SimpleMath::Matrix mtx,  //�I�u�W�F�N�g�̍s��(RTS)
		DirectX::SimpleMath::Vector3 scale);

	//BS�`��p�̃��[���h�ϊ��s��𐶐�����
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	//�ő�l�̎擾
	DirectX::SimpleMath::Vector3 GetMax();

	//�ŏ��l�̎擾
	DirectX::SimpleMath::Vector3 GetMin();

};