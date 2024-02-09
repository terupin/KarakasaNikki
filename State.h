#pragma once
#include<map>
#include<memory>

template<class T>
class State
{
private:
	//�X�e�[�g��ێ����Ă���I�u�W�F�N�g(State<T>��T�̃N���X��ێ�)
	T* m_Context;

	//���݃Z�b�g���Ă���q���X�e�[�g(�Ď������Ă���)
	std::weak_ptr<State<T>> m_CurrentState;

	//��ԑJ�ڂ̃g���K�[�ƑJ�ڐ�̃L�[�l���i�[����}�b�v
	std::map<int, int>  m_TransitionMap;

	//�e��Ԃ�ID�Ǝq���X�e�[�g���i�[����}�b�v�z��
	std::map<int, std::shared_ptr<State<T>>>  m_ChildStateMap;

public:
	State(T* context) { m_Context = context; };
	virtual ~State() {};
	virtual void Enter() = 0;  //�X�e�[�g�ڍs���ɌĂяo�����֐�
	virtual void Update() = 0;  //�X�e�[�g�̍X�V���ɌĂяo�����֐�
	virtual void Exit() = 0;  //�X�e�[�g�ڍs���ɌĂяo�����֐�

	void SendEnter()
	{
		Enter();

		//���݃X�e�[�g���ݒ肳��Ă����
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendEnter();
		}
	}
	void SendUpdate()
	{
		Update();

		//���݃X�e�[�g���ݒ肳��Ă����
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendUpdate();
		}
	}
	void SendExit()
	{
		//���݃X�e�[�g���ݒ肳��Ă����
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}
		Exit();
	}

	//�q���X�e�[�g��o�^����
	template<class T>
	void AddChildState() 
	{
		//�N���X�ɕt�^����Ă����p��ID���擾
		auto stateId = typeid(T).hash_code();

		//�w�肵���L�[�̃}�b�v�Ɋi�[����Ă�����̂����邩�m�F
		auto hasState = m_ChildStateMap.contains(stateId);

		//stateId�̃L�[�̃}�b�v�ꏊ�Ɋi�[������
		m_ChildStateMap[stateId] = std::make_shared<T>(m_Context);
	}

	// From����To�֑J�ڂ���ݒ��o�^
	template<class From, class To>
	void AddChildTransition(int eventId)
	{
		auto FromStateId = typeid(From).hash_code(); //From����class�̃}�b�v�̃n�b�V���l�̎擾
		auto ToStateId = typeid(To).hash_code();  //To����class�̃}�b�v�̃n�b�V���l�̎擾

		//�����̃L�[�l�ɃX�e�[�g���i�[������Ă��邩���m�F
		auto HasHash = m_ChildStateMap.contains(FromStateId) && m_ChildStateMap.contains(ToStateId);

		//�ǂ������������͗����̃L�[�l�ɃX�e�[�g���i�[����Ă��Ȃ��ꍇ
		if (!HasHash) { return; }

		//State��Addtransition�֐��Ŋi�[������
		m_ChildStateMap[FromStateId]->AddTransition(eventId, ToStateId);

	}

	//��ԕψʂ̏�����ǉ�����
	void AddTransition(int eventId, int stateId)
	{	
		m_TransitionMap[eventId] = stateId;
	}

	//�J�ڂ��o���邩���擾
	void SendTrigger(int eventId)
	{
		//�I�u�W�F�N�g���Ȃ��ꍇ
		if (m_CurrentState.expired()) { return; }

		//�J�ڏ������o�^����Ă���Ȃ�
		int stateId = 0;
		if (m_CurrentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}

		m_CurrentState.lock()->SendTrigger(eventId);
	}

	//��ԑJ�ڂ��i�[����Ă��邩�̊m�F
	bool TryGetTransition(int eventId, int& stateId)
	{
		//�R���e�i���ɗv�f�����邩�ǂ������m�F����
		auto HasEvent = m_TransitionMap.contains(eventId);
		if (HasEvent == true)
		{
			stateId = m_TransitionMap	[eventId];
		}
		return HasEvent;
	}

	//���݂̎q�X�e�[�g���Z�b�g����
	template<class T>
	void SetCurrentState()
	{
		auto stateId = typeid(T).hash_code();
		auto hasState = m_ChildStateMap.contains(stateId);

		if (hasState == false) { return; }

		//�ŏ��̎q�X�e�[�g���Z�b�g���ď�����������
		m_CurrentState = m_ChildStateMap[stateId];
	}

	//���݂̎q�X�e�[�g����ς���
	void ChangeState(int stateId)
	{
		// ���ɃX�e�[�g�������Ă���I���������ĂԁB
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}

		// �V�����X�e�[�g�𐶐����ď��������Ă���B
		m_CurrentState = m_ChildStateMap[stateId];
		m_CurrentState.lock()->SendEnter();
	}
};
