#pragma once
#include<memory>
#include<map>
#include"main.h"
#include"State.h"
#include"component.h"

template<class T>
class StateMachine:public Component
{
private:
	T* m_Context;  //このクラスを保持するオブジェクト

	//現在セットしているステート(監視もしている)
	std::weak_ptr<State<T>> m_CurrentState;

	//各状態のIDとステートを格納するマップ配列
	std::map<int, std::shared_ptr<State<T>>> m_StateMap;

public:
	using Component::Component;

	StateMachine(T* context) { m_Context = context; }

	//ステートを登録する
	template<class T>
	void AddState()
	{
		//クラスに付与されている専用のIDを取得
		auto stateId = typeid(T).hash_code();

		//指定したキーのマップに格納されているものがあるか確認
		auto hasState = m_StateMap.contains(stateId);

		//何か格納されていたら戻る
		if (hasState == true)
		{
			return;
		}

		//stateIdのキーのマップ場所に格納をする
		m_StateMap[stateId] = std::make_shared<T>(m_Context);

	}

	//状態遷移をする設定を登録する
	template<class From, class To>
	void AddTransition(int eventId)
	{
		auto FromStateId = typeid(From).hash_code(); //From側のclassのマップのハッシュ値の取得
		auto ToStateId = typeid(To).hash_code();  //To側のclassのマップのハッシュ値の取得

		//両方のキー値にステートが格納をされているかを確認
		auto HasHash = m_StateMap.contains(FromStateId) && m_StateMap.contains(ToStateId);

		//どっちかもしくは両方のキー値にステートが格納されていない場合
		if (!HasHash) { return; }

		//StateのAddtransition関数で格納をする
		m_StateMap[FromStateId]->AddTransition(eventId, ToStateId);
	}
	
	//一番初めに持つStateを格納する
	template<class T>
	void SetStartState()
	{
		auto stateId = typeid(T).hash_code();  //クラスのハッシュ値の取得(キー値にする)
		auto hasState = m_StateMap.contains(stateId);  //stateIdのキーの要素を取得

		if (m_StateMap.contains(stateId) == false)
		{
			return;
		}
		ChangeState(stateId);

	}

	//遷移が出来るかを取得
	void SendTrigger(int eventId)
	{
		//オブジェクトがあるかどうか
		if (m_CurrentState.expired()) { return; }

		//遷移条件が登録されているなら
		int stateId = 0;//TryGetTranstionで取得するIDの格納場所
		if (m_CurrentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}
		m_CurrentState.lock()->SendTrigger(eventId);
	}
	
	//現在のstateの更新処理
	void StateUpdate()
	{
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendUpdate();
		}
	}
												  
	//stateを遷移する
	void ChangeState(int stateId)
	{
		//ステートが入っていたら終了処理をする
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}
		m_CurrentState = m_StateMap[stateId];
		m_CurrentState.lock()->SendEnter();
	}

};

