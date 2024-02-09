#include	<SimpleMath.h>
#include	"collision.h"
#include <algorithm>
#include<iostream>

using namespace DirectX::SimpleMath;

//カプセル情報を生成
//ポジションとスケールを考える
void SetCapsule(Capsule& Cap, Vector3 pos, Vector3 scale,
	std::vector<VERTEX_3D>& vertices)
{
	//全ての頂点の最大値,最小値
	Vector3 max;
	Vector3 min;

	//最大値と最小値の線分
	Vector3 length;

	//座標の最大値と最小値を求める
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

	//それぞれの数値を計算する
	length.x = max.x - min.x * scale.x;
	length.y = max.y - min.y * scale.y;
	length.z = max.z - min.z * scale.z;

	//中心を求める
	Cap.Center = (max + min) * 0.5f * scale + pos;

	//半径を求める
	if (length.x >= length.z)
		Cap.Radius = length.x * 0.5f * scale.x;
	else
		Cap.Radius = length.z * 0.5f * scale.z;

	//始点を求める
	Cap.Segment.StartPoint = Cap.Center;  //xとzの座標を決める(真ん中に置く)
	Cap.Segment.StartPoint.y = (min.y + Cap.Radius);  //Y座標を決める

	//終点を決める
	Cap.Segment.EndPoint = Cap.Center;  //xとzの座標を決める(真ん中に置く)
	Cap.Segment.EndPoint.y = (max.y - Cap.Radius);  //Y座標を決める
}

//AABBの頂点を決める
void SetAABB(
	BoundingBoxAABB& box, 
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 scale, 
	std::vector<VERTEX_3D>& vertices)
{
	//全ての頂点の最大値,最小値
	Vector3 max;
	Vector3 min;


	//AABBの最大値と最小値を求める
	for (auto& v : vertices)
	{
		if (min.x > v.Position.x) min.x = v.Position.x;
		if (min.y > v.Position.y) min.y = v.Position.y;
		if (min.z > v.Position.z) min.z = v.Position.z;

		if (max.x < v.Position.x) max.x = v.Position.x;
		if (max.y < v.Position.y) max.y = v.Position.y;
		if (max.z < v.Position.z) max.z = v.Position.z;
	}

	//最大値と最小値のポジションを格納　　(スケールの格納)
	box.max = max * scale + pos;
	box.min = min * scale + pos;

	//中心を求める
	box.Center=pos;

}

//点と直線の最短距離 
float CalcPointLineDist(Vector3& point, Segment& segment, Vector3& IntersectionPoint, float& t)
{

	float distance = 0.0f;

	// 線分のベクトルを求める
	Vector3 Vec = segment.EndPoint - segment.StartPoint;

	//線分の始点と点を結ぶベクトルを求める
	Vector3 PoVec = point - segment.StartPoint;

	//線分Vecの長さを求める
	float VecLine = Vec.Length();

	//VecベクトルとPoVecベクトルの内積を計算する
	float VecdotPoVec = Vec.Dot(PoVec);

	// 比率を計算
	t = VecdotPoVec / VecLine;

	// 垂線の足
	IntersectionPoint = segment.StartPoint + Vec * t;

	// 垂線の足の長さ
	distance = (IntersectionPoint - point).Length();
	return distance;
}

//点と線分の最短距離
float CalcPointSegmentDist(
	Vector3& point,
	Segment& segment,
	Vector3& IntersectionPoint,
	float& t)
{
	//とりあえず点と直線の最短距離を求める
	float dist = CalcPointLineDist(point, segment, IntersectionPoint, t);

	//IntersectionPointが線分の外にある（始点寄り）
	if (t < 0.0f)
	{
		//点から線分の始点までの距離が最短
		IntersectionPoint = segment.StartPoint;  //開始点
		dist = (point - IntersectionPoint).Length();
		return dist;
	}

	//IntersectionPointが線分の外にある（始点寄り）
	if (t > 1.0f)
	{
		//点から線分の終点までの距離が最短
		IntersectionPoint = segment.EndPoint;  //開始点
		dist = (point - IntersectionPoint).Length();
		return dist;
	}

	//tの値が0<t<1の場合は最短距離が線分の中にある
	return dist;
}

//直線と直線の最短距離
float calcLineLineDist(
	Segment& segment1, Segment& segment2,
	Vector3& IntersectionPoint1, Vector3& IntersectionPoint2,
	float& t1, float& t2)
{
	float StartLength = (segment1.StartPoint - segment2.StartPoint).Length();  //直線の始点同士の距離
	float EndLength = (segment1.EndPoint - segment2.EndPoint).Length();  //直線の終点同士の距離

	//2つの直線が並行
	if (StartLength == EndLength)
	{
		//線分1の始点から直線2までの最短距離に問題に帰着する
		float dist = CalcPointLineDist(segment1.StartPoint, segment2, IntersectionPoint2, t2);
		IntersectionPoint1 = segment1.StartPoint;

		//必ず線分の中にあるため始点に比率をおいておく
		t1 = 0.0f;
		return dist;
	}

	//2つの直線が平行ではない



	// 線分のベクトルを求める
	Vector3 StartVec = segment1.StartPoint - segment1.StartPoint;  //両方の始点のベクトル
	Vector3 EndVec = segment1.StartPoint - segment1.StartPoint;  //両方の終点のベクトル
	Vector3 Vec1 = segment1.EndPoint - segment1.StartPoint;  //線分1のベクトル
	Vector3 Vec2 = segment2.EndPoint - segment2.StartPoint;  //線分2のベクトル

	//内積計算
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

//線分と線分の最短距離
float calcSegmentSegmentDist(
	Segment& segment1, Segment& segment2,
	Vector3& IntersectionPoint1, Vector3& IntersectionPoint2,
	float& t1, float& t2)
{
	Vector3 Vec1 = segment1.EndPoint - segment1.StartPoint;  //線分1のベクトル
	Vector3 Vec2 = segment2.EndPoint - segment2.StartPoint;  //線分2のベクトル
	float dist = 0;

	//2直線の最短距離を求める
	dist = calcLineLineDist(segment1, segment2, IntersectionPoint1, IntersectionPoint2, t1, t2);

	//IntersectionPoint(最短線の端点)の両方が線分内にある場合
	if (0.0f < t1 && t1 < 1.0f && 0.0f < t2 && t2 < 1.0f)
	{
		return dist;
	}

	//IntersectionPoint1が線分内にあるか
	//IntersectionPoint1とt1を求めなおす
	t2 = std::clamp(t2, 0.0f, 1.0f);
	IntersectionPoint2 = segment2.StartPoint + Vec2 * t2;
	dist = CalcPointSegmentDist(IntersectionPoint2, segment1, IntersectionPoint1, t1);
	if (0.0f <= t1 && t1 < 1.0f)
	{
		return dist;
	}

	//IntersectionPoint2が線分内にあるか
	//IntersectionPoint2とt2を求めなおす
	t2 = std::clamp(t1, 0.0f, 1.0f);
	IntersectionPoint1 = segment1.StartPoint + Vec1 * t1;
	dist = CalcPointSegmentDist(IntersectionPoint1, segment2, IntersectionPoint2, t2);
	if (0.0f <= t2 && t2 < 1.0f)
	{
		return dist;

	}
	//どれも当てはまらない場合
	//端点から端点までが最短距離
	return (IntersectionPoint2 - IntersectionPoint1).Length();
}

//境界球の当たり判定
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2)
{
	double centerlength = (p1.Center - p2.Center).Length();  //中心の間の距離を求める
	double radiuslength = (p1.Radius + p2.Radius);

	if (centerlength <= radiuslength)
		return true;

	return false;
}

//カプセル同士のコリジョン
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

//カプセルとAABBのXZの当たり判定
bool HitCapsuleAABB_xz(Capsule& Cap, BoundingBoxAABB Box,Vector3 pos)
{
	float distance_x;
	float distance_z;
	float nowdistance_x;
	float nowdistance_z;

	distance_x = fabs(Cap.Radius + (Box.Center.x - Box.min.x));
	distance_z = fabs(Cap.Radius + (Box.Center.z - Box.min.z));

	//現在のカプセルとボックスの中心の長さ
	nowdistance_x = fabs(Box.Center.x - pos.x);
	nowdistance_z = fabs(Box.Center.z - pos.z);

	if (distance_x > nowdistance_x && distance_z > nowdistance_z)
		return true;

	return false;


}
