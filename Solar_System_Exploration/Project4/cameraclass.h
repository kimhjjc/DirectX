#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_
#include <d3dx10math.h>
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();
	void SetPosition(float, float, float);
	void SetPosition(D3DXVECTOR3);
	void SetRotation(float, float, float);
	void SetRotation(D3DXVECTOR3);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};
#endif