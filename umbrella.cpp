#include "umbrella.h"
#include"shader.h"
#include "scene.h"
#include"manager.h"
#include"camera.h"
#include"player.h"
#include"enemy.h"
#include"Model_Load.h"

#include"input.h"

#include<iostream>

using namespace DirectX::SimpleMath;

void Umbrella::Init()
{

    AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
   
    //モデルとテクスチャの読み込み
    m_Model = AddComponent<Model_Load>();
    m_UDModel = AddComponent<AnimationModel>();
    m_Model->LoadModel ("asset/model/close.fbx");
    m_Model->LoadTex("asset/model/WAGASA_RED.png");

    //アニメーションの読み込み
    m_Model->LoadAnimation("asset/model/Open.fbx", "Open");
    m_Model->LoadAnimation("asset/model/close.fbx", "Close");

   this->m_Rotation = Vector3(2.4f, 0.0f, 0.0f);

    //読み込んだアニメーションを取得する
    m_UDModel->GetAnim(m_Model);

    SetScale(Vector3(1.7, 1.7, 1.7));

}

void Umbrella::Update()
{
    Scene* scene = Manager::GetScene();  //シーンの取得
    m_Player = scene->GetGameObject<Player>();  //プレイヤーの取得

    //当たり判定の設定
    SetCapsule(UnbCol,GetPosition(), GetScale(), m_Model->m_Vertices);

    //手のポジション等を検索
    m_AnimPlayer = m_Player->GetComponent<Model_Load>()->m_AiScene;  //プレイヤーのアニメーションモデルの情報
    m_RightHandBone = m_Player->m_Model->m_Bone[m_HandBoneName];  //プレイヤーの右手のボーン情報

    //手の変換行列×手の逆ボーンオフセット行列
    aiMatrix4x4 rootMatrix = m_RightHandBone.Matrix*m_RightHandBone.OffsetMatrix.Inverse();
   

    //プレイヤーのワールド行列を取得
    m_WorldMatrix = m_Player->GetWorldMatrix();  
    
    RightHandMatrix = AnimationModel::ChangeMatrix(RightHandMatrix, rootMatrix);  //aiMatrixをMatrixに変換
    RightHandMatrix.Decompose(m_Rightscale, m_Rightrot, m_Rightpos);
    RightHandMatrix *= m_WorldMatrix;
    RightHandMatrix.Decompose(m_Worldscale, m_Worldrot, m_Worldpos);  //行列を分解
    
    
    //共役クォータニオンにする
    //s_rot.x = -s_rot.x;
    //s_rot.y = -s_rot.y;
    //s_rot.z = -s_rot.z;

    s_rot = m_Rightrot.ToEuler();//クォータニオンをオイラー角に変換

    //代入
    SetPosition(m_Worldpos);
    SetRotation({ s_rot.y,s_rot.x,s_rot.z });  //個々の計算がおかしい

}
