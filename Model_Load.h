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


//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int BoneNum;
	std::string BoneName[4];
	float BoneWeight[4];
	int BoneIdx[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;						// 座標変換に使用する行列
	aiMatrix4x4 AnimationMatrix;			// 自分の事だけ考えた行列
	aiMatrix4x4 OffsetMatrix;				// ボーンオフセット行列
	aiQuaternion BlendRFrom;				// モーションブレンドする際のFROM側
	aiQuaternion BlendRTo;					// モーションブレンドする際のTO側
	aiVector3D	BlendPosFrom;				// モーションブレンドする際のFROM側
	aiVector3D	BlendPosTo;					// モーションブレンドする際のTO側
	int idx;
};

struct CBBoneCombMatrx 
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// ボーンコンビネーション行列
};

class Model_Load:public Component
{	
public:
	using Component::Component;
	
	const aiScene* m_AiScene = nullptr;
	std::unordered_map < std::string, const aiScene*> m_Animation;  //アニメーションの格納場所

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_INTexture;  //モデルにテクスチャがある場合の格納場所
	ID3D11ShaderResourceView* m_OutTexture;  //モデルにテクスチャがない場合

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//変形後頂点データ

	std::unordered_map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）
	
	std::vector<VERTEX_3D> m_Vertices;  //頂点座標軍

	ID3D11Buffer* m_BoneCombMtxCBuffer;  //定数バッファ

	const char* TextureName=nullptr;

	void LoadModel(const char *FileName);
	void LoadStaticModel(const char* FileName);
	void LoadTex(const char* FileName);  //変形しないモデルの読み込み
	void LoadAnimation(const char *FileName,const char *AnimName);
	void Uninit()override;
	void Draw()override;  //モデルの表示

	void CreateBone(aiNode* node);
};