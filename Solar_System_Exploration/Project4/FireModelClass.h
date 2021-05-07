#pragma once

#ifndef _FIREMODELCLASS_H_
#define _FIREMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;

#include "textureclass.h"

class FireModelClass
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
	FireModelClass();
	FireModelClass(const FireModelClass&);
	~FireModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	bool ReadFileCounts(char*);
	bool LoadDataStructures(char*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount, m_faceShape;
	TextureClass *m_Texture1, *m_Texture2, *m_Texture3;
	ModelType* m_model;
};

#endif