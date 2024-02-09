#pragma once
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "assimp-vc142-mtd.lib")
#else 
#pragma comment (lib, "assimp-vc142-mt.lib")
#endif

#include <unordered_map>

#include "gameObject.h"
#include "component.h"


//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int BoneNum;
	std::string BoneName[4];
	float BoneWeight[4];
	int BoneIdx[4];
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;						// ���W�ϊ��Ɏg�p����s��
	aiMatrix4x4 AnimationMatrix;			// �����̎������l�����s��
	aiMatrix4x4 OffsetMatrix;				// �{�[���I�t�Z�b�g�s��
	aiQuaternion BlendRFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiQuaternion BlendRTo;					// ���[�V�����u�����h����ۂ�TO��
	aiVector3D	BlendPosFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiVector3D	BlendPosTo;					// ���[�V�����u�����h����ۂ�TO��
	int idx;
};

struct CBBoneCombMatrx 
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// �{�[���R���r�l�[�V�����s��
};

class Model_Load:public Component
{	
public:
	using Component::Component;
	
	const aiScene* m_AiScene = nullptr;
	std::unordered_map < std::string, const aiScene*> m_Animation;  //�A�j���[�V�����̊i�[�ꏊ

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_INTexture;  //���f���Ƀe�N�X�`��������ꍇ�̊i�[�ꏊ
	ID3D11ShaderResourceView* m_OutTexture;  //���f���Ƀe�N�X�`�����Ȃ��ꍇ

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//�ό`�㒸�_�f�[�^

	std::unordered_map<std::string, BONE> m_Bone;			//�{�[���f�[�^�i���O�ŎQ�Ɓj
	
	std::vector<VERTEX_3D> m_Vertices;  //���_���W�R

	ID3D11Buffer* m_BoneCombMtxCBuffer;  //�萔�o�b�t�@

	const char* TextureName=nullptr;

	void LoadModel(const char *FileName);
	void LoadStaticModel(const char* FileName);
	void LoadTex(const char* FileName);  //�ό`���Ȃ����f���̓ǂݍ���
	void LoadAnimation(const char *FileName,const char *AnimName);
	void Uninit()override;
	void Draw()override;  //���f���̕\��

	void CreateBone(aiNode* node);
};