#pragma once

#include "renderer.h"

#include <list>
#include <SimpleMath.h>

#include "component.h"
#include"collision.h"

//プレイヤーの行動
enum Trigger
{
	ToIdle,
	ToWalk,
	ToRun,
	ToJump,
	ToFall,
	ToAttack,
	ToGuard,
	ToDeath,
};


class GameObject
{

protected:
	bool		m_Destroy = false;

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	DirectX::SimpleMath::Vector3 m_Forward;  //プレイヤーの前

	DirectX::SimpleMath::Vector3	m_OldPosition ;
	DirectX::SimpleMath::Vector3	m_OldRotation;
	DirectX::SimpleMath::Vector3	m_OldScale;

	std::list<Component*> m_Component;

	std::list<GameObject*> m_ChildGameObject; 
	DirectX::SimpleMath::Matrix m_OldMatrix;
	DirectX::SimpleMath::Matrix m_WorldMatrix;

public:
	GameObject() {}//コンストラクタ
	virtual ~GameObject() {}//デストラクタ（仮想関数）

	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }
	DirectX::SimpleMath::Vector3 GetForward() { return m_Forward; }
	void SetPosition(DirectX::SimpleMath::Vector3 Position) { m_Position = Position; }
	void SetRotation(DirectX::SimpleMath::Vector3 Rotation) { m_Rotation = Rotation; }
	void SetScale(DirectX::SimpleMath::Vector3 Scale) { m_Scale = Scale; }
	void SetForward(DirectX::SimpleMath::Vector3 Forward) { m_Forward = Forward; }


	DirectX::SimpleMath::Matrix GetWorldMatrix() { return m_WorldMatrix; }



	DirectX::SimpleMath::Vector3 GetNowForward()//前方向ベクトル取得
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		m_Forward.x = rot._31;
		m_Forward.y = rot._32;
		m_Forward.z = rot._33;

		return m_Forward;
	}

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			UninitBase();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();

		return component;
	}

	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();
			
		return child;
	}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		Uninit();
		//子供ゲームオブジェクトの終了処理）
		for (auto* childgo : m_ChildGameObject)
		{
			childgo->Uninit();

			for (Component* component : childgo->m_Component)
			{
				component->Uninit();
				delete component;
			}
			childgo->m_Component.clear();//リストのクリア

			delete childgo;
		}
		m_ChildGameObject.clear();//リストのクリア

		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();//リストのクリア

	}

	void UpdateBase()
	{

		for (Component* component : m_Component)
		{
			component->Update();
		}

		Update();
	}

	void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		m_WorldMatrix = scale * rot * trans * ParentMatrix;


		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawBase(m_WorldMatrix);
		}


		Renderer::SetWorldMatrix(&m_WorldMatrix);

		for (Component* component : m_Component)
		{
			component->Draw();
		}

		Draw();
	}
};