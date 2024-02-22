#pragma once
#include<SimpleMath.h>
#include "gameObject.h"
#include"AnimationModel.h"
#include"player.h"
#include"collision.h"
#include"renderer.h"

class Player;

class Umbrella :public GameObject
{
private:
    //モデルの読み込み
    class Model_Load* m_Model;
    class AnimationModel* m_UDModel;
    
    Player* m_Player;//プレイヤー情報
    BONE m_RightHandBone;  //右手のボーン情報

    std::string m_HandBoneName= "mixamorig:RightHand";  //右手のボーン名

    const aiScene* m_AnimPlayer;  //プレイヤーのアニメーションモデル情報
    aiMatrix4x4 m_RightHandMatrix;//右手のボーン情報

    DirectX::SimpleMath::Matrix m_WorldMatrix;
    DirectX::SimpleMath::Matrix RightHandMatrix;

    //ワールド行列を分解したときに代入する物
    DirectX::SimpleMath::Vector3 m_Worldscale;
    DirectX::SimpleMath::Vector3 m_Worldpos;
    DirectX::SimpleMath::Quaternion m_Worldrot;

    //右手のボーンの行列を分解したときに代入する
    DirectX::SimpleMath::Vector3 m_Rightscale;
    DirectX::SimpleMath::Vector3 m_Rightpos;
    DirectX::SimpleMath::Quaternion m_Rightrot;

    //クォータニオンの角度をオイラー角に変えた時に格納する場所
    DirectX::SimpleMath::Vector3 s_rot;
     
public:
    //当たり判定
    Capsule UnbCol;

    void Init() override;
    void Update() override;

};