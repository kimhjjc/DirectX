#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_
#include <d3dx10math.h>
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
	void SetAmbientSwitch(bool);
	void SetDiffuseSwitch(bool);
	void SetSpecularSwitch(bool);

	void SetPosition(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor();
	D3DXVECTOR4 GetPosition();

	float GetSpecularPower();
	bool GetAmbientSwitch();
	bool GetDiffuseSwitch();
	bool GetSpecularSwitch();

	void Turn_On_OFFAmbientLight();
	void Turn_On_OFFDiffuseLight();
	void Turn_On_OFFSpecularLight();

private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR4 m_specularColor;
	D3DXVECTOR4 m_position;

	float m_specularPower;
	bool m_ambientSwitch;
	bool m_diffuseSwitch;
	bool m_specularSwitch;
};
#endif