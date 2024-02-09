#pragma once
#include<map>
#include<memory>

template<class T>
class State
{
private:
	//ステートを保持しているオブジェクト(State<T>のTのクラスを保持)
	T* m_Context;

	//現在セットしている子供ステート(監視もしている)
	std::weak_ptr<State<T>> m_CurrentState;

	//状態遷移のトリガーと遷移先のキー値を格納するマップ
	std::map<int, int>  m_TransitionMap;

	//各状態のIDと子供ステートを格納するマップ配列
	std::map<int, std::shared_ptr<State<T>>>  m_ChildStateMap;

public:
	State(T* context) { m_Context = context; };
	virtual ~State() {};
	virtual void Enter() = 0;  //ステート移行時に呼び出される関数
	virtual void Update() = 0;  //ステートの更新中に呼び出される関数
	virtual void Exit() = 0;  //ステート移行時に呼び出される関数

	void SendEnter()
	{
		Enter();

		//現在ステートが設定されていれば
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendEnter();
		}
	}
	void SendUpdate()
	{
		Update();

		//現在ステートが設定されていれば
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendUpdate();
		}
	}
	void SendExit()
	{
		//現在ステートが設定されていれば
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}
		Exit();
	}

	//子供ステートを登録する
	template<class T>
	void AddChildState() 
	{
		//クラスに付与されている専用のIDを取得
		auto stateId = typeid(T).hash_code();

		//指定したキーのマップに格納されているものがあるか確認
		auto hasState = m_ChildStateMap.contains(stateId);

		//stateIdのキーのマップ場所に格納をする
		m_ChildStateMap[stateId] = std::make_shared<T>(m_Context);
	}

	// FromからToへ遷移する設定を登録
	template<class From, class To>
	void AddChildTransition(int eventId)
	{
		auto FromStateId = typeid(From).hash_code(); //From側のclassのマップのハッシュ値の取得
		auto ToStateId = typeid(To).hash_code();  //To側のclassのマップのハッシュ値の取得

		//両方のキー値にステートが格納をされているかを確認
		auto HasHash = m_ChildStateMap.contains(FromStateId) && m_ChildStateMap.contains(ToStateId);

		//どっちかもしくは両方のキー値にステートが格納されていない場合
		if (!HasHash) { return; }

		//StateのAddtransition関数で格納をする
		m_ChildStateMap[FromStateId]->AddTransition(eventId, ToStateId);

	}

	//状態変位の条件を追加する
	void AddTransition(int eventId, int stateId)
	{	
		m_TransitionMap[eventId] = stateId;
	}

	//遷移が出来るかを取得
	void SendTrigger(int eventId)
	{
		//オブジェクトがない場合
		if (m_CurrentState.expired()) { return; }

		//遷移条件が登録されているなら
		int stateId = 0;
		if (m_CurrentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}

		m_CurrentState.lock()->SendTrigger(eventId);
	}

	//状態遷移が格納されているかの確認
	bool TryGetTransition(int eventId, int& stateId)
	{
		//コンテナ内に要素があるかどうかを確認する
		auto HasEvent = m_TransitionMap.contains(eventId);
		if (HasEvent == true)
		{
			stateId = m_TransitionMap	[eventId];
		}
		return HasEvent;
	}

	//現在の子ステートをセットする
	template<class T>
	void SetCurrentState()
	{
		auto stateId = typeid(T).hash_code();
		auto hasState = m_ChildStateMap.contains(stateId);

		if (hasState == false) { return; }

		//最初の子ステートをセットして初期化をする
		m_CurrentState = m_ChildStateMap[stateId];
	}

	//現在の子ステートから変える
	void ChangeState(int stateId)
	{
		// 既にステートが入ってたら終了処理を呼ぶ。
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}

		// 新しくステートを生成して初期化している。
		m_CurrentState = m_ChildStateMap[stateId];
		m_CurrentState.lock()->SendEnter();
	}
};
