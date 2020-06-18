#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"
#include <fstream>
using namespace std;

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	typedef struct
	{
		int vIndex1, vIndex2, vIndex3, vIndex4;
		int tIndex1, tIndex2, tIndex3, tIndex4;
		int nIndex1, nIndex2, nIndex3, nIndex4;
	}FaceType;
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, int);
	ID3D11ShaderResourceView* GetTexture();
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	bool ReadFileCounts(char*);
	bool LoadDataStructures(char*, int);
	bool LoadModel(char*);
	void ReleaseModel();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount, m_faceShape;
	TextureClass* m_Texture;
	ModelType* m_model;
};
#endif