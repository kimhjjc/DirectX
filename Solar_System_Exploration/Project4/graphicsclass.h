#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h"

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, float);
	D3DClass* Get_m_D3D() { return m_D3D; }
	TextureShaderClass* Get_m_TextureShader() { return m_TextureShader; }
	LightClass* Get_m_Light() { return m_Light; }
	void SetSceneColor(float, float, float, float);
	bool Render(float);

	void MoveForward(float);
	void StrafeRight(float);
	void MouseMove(float, float, float);

private:
	float sceneColor[4];
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	int m_ModelCount;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	D3DXVECTOR3 m_EarthDistance;
	int m_Speed, m_Life;
};
#endif