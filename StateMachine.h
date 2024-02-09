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
	T* m_Context;  //���̃N���X��ێ�����I�u�W�F�N�g

	//���݃Z�b�g���Ă���X�e�[�g(�Ď������Ă���)
	std::weak_ptr<State<T>> m_CurrentState;

	//�e��Ԃ�ID�ƃX�e�[�g���i�[����}�b�v�z��
	std::map<int, std::shared_ptr<State<T>>> m_StateMap;

public:
	using Component::Component;

	StateMachine(T* context) { m_Context = context; }

	//�X�e�[�g��o�^����
	template<class T>
	void AddState()
	{
		//�N���X�ɕt�^����Ă����p��ID���擾
		auto stateId = typeid(T).hash_code();

		//�w�肵���L�[�̃}�b�v�Ɋi�[����Ă�����̂����邩�m�F
		auto hasState = m_StateMap.contains(stateId);

		//�����i�[����Ă�����߂�
		if (hasState == true)
		{
			return;
		}

		//stateId�̃L�[�̃}�b�v�ꏊ�Ɋi�[������
		m_StateMap[stateId] = std::make_shared<T>(m_Context);

	}

	//��ԑJ�ڂ�����ݒ��o�^����
	template<class From, class To>
	void AddTransition(int eventId)
	{
		auto FromStateId = typeid(From).hash_code(); //From����class�̃}�b�v�̃n�b�V���l�̎擾
		auto ToStateId = typeid(To).hash_code();  //To����class�̃}�b�v�̃n�b�V���l�̎擾

		//�����̃L�[�l�ɃX�e�[�g���i�[������Ă��邩���m�F
		auto HasHash = m_StateMap.contains(FromStateId) && m_StateMap.contains(ToStateId);

		//�ǂ������������͗����̃L�[�l�ɃX�e�[�g���i�[����Ă��Ȃ��ꍇ
		if (!HasHash) { return; }

		//State��Addtransition�֐��Ŋi�[������
		m_StateMap[FromStateId]->AddTransition(eventId, ToStateId);
	}
	
	//��ԏ��߂Ɏ���State���i�[����
	template<class T>
	void SetStartState()
	{
		auto stateId = typeid(T).hash_code();  //�N���X�̃n�b�V���l�̎擾(�L�[�l�ɂ���)
		auto hasState = m_StateMap.contains(stateId);  //stateId�̃L�[�̗v�f���擾

		if (m_StateMap.contains(stateId) == false)
		{
			return;
		}
		ChangeState(stateId);

	}

	//�J�ڂ��o���邩���擾
	void SendTrigger(int eventId)
	{
		//�I�u�W�F�N�g�����邩�ǂ���
		if (m_CurrentState.expired()) { return; }

		//�J�ڏ������o�^����Ă���Ȃ�
		int stateId = 0;//TryGetTranstion�Ŏ擾����ID�̊i�[�ꏊ
		if (m_CurrentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}
		m_CurrentState.lock()->SendTrigger(eventId);
	}
	
	//���݂�state�̍X�V����
	void StateUpdate()
	{
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendUpdate();
		}
	}
												  
	//state��J�ڂ���
	void ChangeState(int stateId)
	{
		//�X�e�[�g�������Ă�����I������������
		if (m_CurrentState.expired() == false)
		{
			m_CurrentState.lock()->SendExit();
		}
		m_CurrentState = m_StateMap[stateId];
		m_CurrentState.lock()->SendEnter();
	}

};

