#pragma once
#include	<SimpleMath.h>
#include"renderer.h"

//境界球
struct  BoundingSphere
{
	DirectX::SimpleMath::Vector3 Center;
	float Radius;
};

// 境界ボックスAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 Center;
};

//線分
struct Segment
{
	//始点の値が小さくなるようにする
	DirectX::SimpleMath::Vector3 StartPoint;//始点
	DirectX::SimpleMath::Vector3 EndPoint;  //終点
};

//カプセル
struct Capsule
{
	DirectX::SimpleMath::Vector3 Center;  //中心
	Segment Segment;
	float Radius; //半径
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

//点と直線の最短距離
float CalcPointLineDist(
	DirectX::SimpleMath::Vector3& point,//点
	Segment& segment,  //直線
	DirectX::SimpleMath::Vector3& IntersectionPoint,//点から直線に下した垂線の端点
	float& t); //直線の長さを1にした場合の直線の始点から端点の長さの割合

//点と線分の最短距離
float CalcPointSegmentDist(
	DirectX::SimpleMath::Vector3& point,//点
	Segment& segment,  //直線
	DirectX::SimpleMath::Vector3& IntersectionPoint,//点から直線に下した垂線の端点
	float& t); //直線の長さを1にした場合の直線の始点から端点の長さの割合

//直線と直線の最短距離
float calcLineLineDist(
	Segment& segment1,  //線分
	Segment& segment2,
	DirectX::SimpleMath::Vector3& IntersectionPoint1,  //最短線の端点
	DirectX::SimpleMath::Vector3& IntersectionPoint2,
	float& t1,  //直線の長さを1にした場合の直線の始点から端点の長さの割合
	float& t2);

//線分と線分の最短距離
float calcSegmentSegmentDist(
	Segment& segment1,  //線分
	Segment& segment2,
	DirectX::SimpleMath::Vector3& IntersectionPoint1,  //最短線の端点
	DirectX::SimpleMath::Vector3& IntersectionPoint2,
	float& t1,  //直線の長さを1にした場合の直線の始点から端点の長さの割合
	float& t2);

//カプセル同士の当たり判定
bool CapsuleCollision(Capsule& Cap1, Capsule& Cap2);

// 境界球の当たり判定
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2);

//カプセルとAABBの当たり判定
bool HitCapsuleAABB_xz(Capsule& Cap, BoundingBoxAABB Box,
	DirectX::SimpleMath::Vector3 pos);