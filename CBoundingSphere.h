#pragma once
#include	<SimpleMath.h>
#include<iostream>

#include	"renderer.h"
#include	"collision.h"

class CBoundintSphere
{
private:
	DirectX::SimpleMath::Vector3 m_Center;  //中心座標
	float m_Radius;  //半径

	DirectX::SimpleMath::Vector3 m_Max;  //AABB 最大
	DirectX::SimpleMath::Vector3 m_Min;  //AABB 最小

	//ワールド座標の計算(大賞オブジェクトの行列が入っている)
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix mtx);

public:
	//BS情報を生成する
	void Caliculate(const std::vector<VERTEX_3D>& vertices);

	//半径を戻す
	float GetRadius();

	//中心座標を戻す
	DirectX::SimpleMath::Vector3 GetCenter();

	//BS情報生成
	BoundingSphere MakeBS(
		DirectX::SimpleMath::Matrix mtx,  //オブジェクトの行列(RTS)
		DirectX::SimpleMath::Vector3 scale);

	//BS描画用のワールド変換行列を生成する
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	//最大値の取得
	DirectX::SimpleMath::Vector3 GetMax();

	//最小値の取得
	DirectX::SimpleMath::Vector3 GetMin();

};