#include	<SimpleMath.h>
#include	"collision.h"
#include <algorithm>
#include<iostream>

using namespace DirectX::SimpleMath;

//�J�v�Z�����𐶐�
//�|�W�V�����ƃX�P�[�����l����
void SetCapsule(Capsule& Cap, Vector3 pos, Vector3 scale,
	std::vector<VERTEX_3D>& vertices)
{
	//�S�Ă̒��_�̍ő�l,�ŏ��l
	Vector3 max;
	Vector3 min;

	//�ő�l�ƍŏ��l�̐���
	Vector3 length;

	//���W�̍ő�l�ƍŏ��l�����߂�
	min = max = vertices[0].Position;
	for (auto& v : vertices)
	{
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	//���ꂼ��̐��l���v�Z����
	length.x = max.x - min.x * scale.x;
	length.y = max.y - min.y * scale.y;
	length.z = max.z - min.z * scale.z;

	//���S�����߂�
	Cap.Center = (max + min) * 0.5f * scale + pos;

	//���a�����߂�
	if (length.x >= length.z)
		Cap.Radius = length.x * 0.5f * scale.x;
	else
		Cap.Radius = length.z * 0.5f * scale.z;

	//�n�_�����߂�
	Cap.Segment.StartPoint = Cap.Center;  //x��z�̍��W�����߂�(�^�񒆂ɒu��)
	Cap.Segment.StartPoint.y = (min.y + Cap.Radius);  //Y���W�����߂�

	//�I�_�����߂�
	Cap.Segment.EndPoint = Cap.Center;  //x��z�̍��W�����߂�(�^�񒆂ɒu��)
	Cap.Segment.EndPoint.y = (max.y - Cap.Radius);  //Y���W�����߂�
}

//AABB�̒��_�����߂�
void SetAABB(
	BoundingBoxAABB& box, 
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 scale, 
	std::vector<VERTEX_3D>& vertices)
{
	//�S�Ă̒��_�̍ő�l,�ŏ��l
	Vector3 max;
	Vector3 min;


	//AABB�̍ő�l�ƍŏ��l�����߂�
	for (auto& v : vertices)
	{
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	//�ő�l�ƍŏ��l�̃|�W�V�������i�[�@�@(�X�P�[���̊i�[)
	box.max = max * scale + pos;
	box.min = min * scale + pos;

	//���S�����߂�
	box.Center=pos;

}

//�_�ƒ����̍ŒZ���� 
float CalcPointLineDist(Vector3& point, Segment& segment, Vector3& IntersectionPoint, float& t)
{

	float distance = 0.0f;

	// �����̃x�N�g�������߂�
	Vector3 Vec = segment.EndPoint - segment.StartPoint;

	//�����̎n�_�Ɠ_�����ԃx�N�g�������߂�
	Vector3 PoVec = point - segment.StartPoint;

	//����Vec�̒��������߂�
	float VecLine = Vec.Length();

	//Vec�x�N�g����PoVec�x�N�g���̓��ς��v�Z����
	float VecdotPoVec = Vec.Dot(PoVec);

	// �䗦���v�Z
	t = VecdotPoVec / VecLine;

	// �����̑�
	IntersectionPoint = segment.StartPoint + Vec * t;

	// �����̑��̒���
	distance = (IntersectionPoint - point).Length();
	return distance;
}

//�_�Ɛ����̍ŒZ����
float CalcPointSegmentDist(
	Vector3& point,
	Segment& segment,
	Vector3& IntersectionPoint,
	float& t)
{
	//�Ƃ肠�����_�ƒ����̍ŒZ���������߂�
	float dist = CalcPointLineDist(point, segment, IntersectionPoint, t);

	//IntersectionPoint�������̊O�ɂ���i�n�_���j
	if (t < 0.0f)
	{
		//�_��������̎n�_�܂ł̋������ŒZ
		IntersectionPoint = segment.StartPoint;  //�J�n�_
		dist = (point - IntersectionPoint).Length();
		return dist;
	}

	//IntersectionPoint�������̊O�ɂ���i�n�_���j
	if (t > 1.0f)
	{
		//�_��������̏I�_�܂ł̋������ŒZ
		IntersectionPoint = segment.EndPoint;  //�J�n�_
		dist = (point - IntersectionPoint).Length();
		return dist;
	}

	//t�̒l��0<t<1�̏ꍇ�͍ŒZ�����������̒��ɂ���
	return dist;
}

//�����ƒ����̍ŒZ����
float calcLineLineDist(
	Segment& segment1, Segment& segment2,
	Vector3& IntersectionPoint1, Vector3& IntersectionPoint2,
	float& t1, float& t2)
{
	float StartLength = (segment1.StartPoint - segment2.StartPoint).Length();  //�����̎n�_���m�̋���
	float EndLength = (segment1.EndPoint - segment2.EndPoint).Length();  //�����̏I�_���m�̋���

	//2�̒��������s
	if (StartLength == EndLength)
	{
		//����1�̎n�_���璼��2�܂ł̍ŒZ�����ɖ��ɋA������
		float dist = CalcPointLineDist(segment1.StartPoint, segment2, IntersectionPoint2, t2);
		IntersectionPoint1 = segment1.StartPoint;

		//�K�������̒��ɂ��邽�ߎn�_�ɔ䗦�������Ă���
		t1 = 0.0f;
		return dist;
	}

	//2�̒��������s�ł͂Ȃ�



	// �����̃x�N�g�������߂�
	Vector3 StartVec = segment1.StartPoint - segment1.StartPoint;  //�����̎n�_�̃x�N�g��
	Vector3 EndVec = segment1.StartPoint - segment1.StartPoint;  //�����̏I�_�̃x�N�g��
	Vector3 Vec1 = segment1.EndPoint - segment1.StartPoint;  //����1�̃x�N�g��
	Vector3 Vec2 = segment2.EndPoint - segment2.StartPoint;  //����2�̃x�N�g��

	//���όv�Z
	float DotVec1Vec2 = Vec1.Dot(Vec2);
	float DotVec1Vec1 = Vec1.Dot(Vec1);
	float DotVec2Vec2 = Vec2.Dot(Vec2);

	t1 = (DotVec1Vec2 * Vec2.Dot(StartVec) - DotVec2Vec2 * Vec1.Dot(StartVec))
		- (DotVec1Vec1 * DotVec2Vec2 - DotVec1Vec2 * DotVec1Vec2);
	IntersectionPoint1 = segment1.StartPoint + Vec1 * t1;
	t2 = Vec2.Dot(IntersectionPoint1 - segment2.StartPoint) / DotVec2Vec2;
	IntersectionPoint2 = segment2.StartPoint + Vec2 * t2;

	return (IntersectionPoint1 - IntersectionPoint2).Length();
}

//�����Ɛ����̍ŒZ����
float calcSegmentSegmentDist(
	Segment& segment1, Segment& segment2,
	Vector3& IntersectionPoint1, Vector3& IntersectionPoint2,
	float& t1, float& t2)
{
	Vector3 Vec1 = segment1.EndPoint - segment1.StartPoint;  //����1�̃x�N�g��
	Vector3 Vec2 = segment2.EndPoint - segment2.StartPoint;  //����2�̃x�N�g��
	float dist = 0;

	//2�����̍ŒZ���������߂�
	dist = calcLineLineDist(segment1, segment2, IntersectionPoint1, IntersectionPoint2, t1, t2);

	//IntersectionPoint(�ŒZ���̒[�_)�̗������������ɂ���ꍇ
	if (0.0f < t1 && t1 < 1.0f && 0.0f < t2 && t2 < 1.0f)
	{
		return dist;
	}

	//IntersectionPoint1���������ɂ��邩
	//IntersectionPoint1��t1�����߂Ȃ���
	t2 = std::clamp(t2, 0.0f, 1.0f);
	IntersectionPoint2 = segment2.StartPoint + Vec2 * t2;
	dist = CalcPointSegmentDist(IntersectionPoint2, segment1, IntersectionPoint1, t1);
	if (0.0f <= t1 && t1 < 1.0f)
	{
		return dist;
	}

	//IntersectionPoint2���������ɂ��邩
	//IntersectionPoint2��t2�����߂Ȃ���
	t2 = std::clamp(t1, 0.0f, 1.0f);
	IntersectionPoint1 = segment1.StartPoint + Vec1 * t1;
	dist = CalcPointSegmentDist(IntersectionPoint1, segment2, IntersectionPoint2, t2);
	if (0.0f <= t2 && t2 < 1.0f)
	{
		return dist;

	}
	//�ǂ�����Ă͂܂�Ȃ��ꍇ
	//�[�_����[�_�܂ł��ŒZ����
	return (IntersectionPoint2 - IntersectionPoint1).Length();
}

//���E���̓����蔻��
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2)
{
	double centerlength = (p1.Center - p2.Center).Length();  //���S�̊Ԃ̋��������߂�
	double radiuslength = (p1.Radius + p2.Radius);

	if (centerlength <= radiuslength)
		return true;

	return false;
}

//�J�v�Z�����m�̃R���W����
bool CapsuleCollision(Capsule& Cap1, Capsule& Cap2)
{
	Vector3 IntersectionPoint1;
	Vector3 IntersectionPoint2;
	float t1;
	float t2;
	float dist = 0;

	dist = calcSegmentSegmentDist(
		Cap1.Segment, Cap2.Segment,
		IntersectionPoint1, IntersectionPoint2,
		t1, t2);

	if (dist < Cap1.Radius + Cap2.Radius)
		return true;

	return false;
}

//�J�v�Z����AABB��XZ�̓����蔻��
bool HitCapsuleAABB_xz(Capsule& Cap, BoundingBoxAABB Box,Vector3 pos)
{
	float distance_x;
	float distance_z;
	float nowdistance_x;
	float nowdistance_z;

	distance_x = fabs(Cap.Radius + (Box.Center.x - Box.min.x));
	distance_z = fabs(Cap.Radius + (Box.Center.z - Box.min.z));

	//���݂̃J�v�Z���ƃ{�b�N�X�̒��S�̒���
	nowdistance_x = fabs(Box.Center.x - pos.x);
	nowdistance_z = fabs(Box.Center.z - pos.z);

	if (distance_x > nowdistance_x && distance_z > nowdistance_z)
		return true;

	return false;


}
