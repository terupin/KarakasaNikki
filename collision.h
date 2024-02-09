#pragma once
#include	<SimpleMath.h>
#include"renderer.h"

//���E��
struct  BoundingSphere
{
	DirectX::SimpleMath::Vector3 Center;
	float Radius;
};

// ���E�{�b�N�XAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 Center;
};

//����
struct Segment
{
	//�n�_�̒l���������Ȃ�悤�ɂ���
	DirectX::SimpleMath::Vector3 StartPoint;//�n�_
	DirectX::SimpleMath::Vector3 EndPoint;  //�I�_
};

//�J�v�Z��
struct Capsule
{
	DirectX::SimpleMath::Vector3 Center;  //���S
	Segment Segment;
	float Radius; //���a
};


void SetCapsule(
	Capsule& cap,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 scale,
 	std::vector<VERTEX_3D>& vertices);

void SetAABB(
	BoundingBoxAABB& box,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 scale,
	std::vector<VERTEX_3D>& vertices);

//�_�ƒ����̍ŒZ����
float CalcPointLineDist(
	DirectX::SimpleMath::Vector3& point,//�_
	Segment& segment,  //����
	DirectX::SimpleMath::Vector3& IntersectionPoint,//�_���璼���ɉ����������̒[�_
	float& t); //�����̒�����1�ɂ����ꍇ�̒����̎n�_����[�_�̒����̊���

//�_�Ɛ����̍ŒZ����
float CalcPointSegmentDist(
	DirectX::SimpleMath::Vector3& point,//�_
	Segment& segment,  //����
	DirectX::SimpleMath::Vector3& IntersectionPoint,//�_���璼���ɉ����������̒[�_
	float& t); //�����̒�����1�ɂ����ꍇ�̒����̎n�_����[�_�̒����̊���

//�����ƒ����̍ŒZ����
float calcLineLineDist(
	Segment& segment1,  //����
	Segment& segment2,
	DirectX::SimpleMath::Vector3& IntersectionPoint1,  //�ŒZ���̒[�_
	DirectX::SimpleMath::Vector3& IntersectionPoint2,
	float& t1,  //�����̒�����1�ɂ����ꍇ�̒����̎n�_����[�_�̒����̊���
	float& t2);

//�����Ɛ����̍ŒZ����
float calcSegmentSegmentDist(
	Segment& segment1,  //����
	Segment& segment2,
	DirectX::SimpleMath::Vector3& IntersectionPoint1,  //�ŒZ���̒[�_
	DirectX::SimpleMath::Vector3& IntersectionPoint2,
	float& t1,  //�����̒�����1�ɂ����ꍇ�̒����̎n�_����[�_�̒����̊���
	float& t2);

//�J�v�Z�����m�̓����蔻��
bool CapsuleCollision(Capsule& Cap1, Capsule& Cap2);

// ���E���̓����蔻��
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2);

//�J�v�Z����AABB�̓����蔻��
bool HitCapsuleAABB_xz(Capsule& Cap, BoundingBoxAABB Box,
	DirectX::SimpleMath::Vector3 pos);