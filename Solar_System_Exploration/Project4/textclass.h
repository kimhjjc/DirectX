#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_
#include "fontclass.h"
#include "fontshaderclass.h"
class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetPolygon(int, ID3D11DeviceContext*);
	bool SetEarthDistance(D3DXVECTOR3, ID3D11DeviceContext*);
	bool SetShipSpeed(int, ID3D11DeviceContext*);
	bool SetLife(int, ID3D11DeviceContext*);
	bool SetObjectCount(int, ID3D11DeviceContext*);
	bool SetScreenSize(ID3D11DeviceContext*);
private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
	SentenceType* m_sentence3;
	SentenceType* m_sentence4;
	SentenceType* m_sentence4_1;
	SentenceType* m_sentence5;
	SentenceType* m_sentence5_1;
	SentenceType* m_sentence6;
	SentenceType* m_sentence7;
	SentenceType* m_sentence8;
};
#endif