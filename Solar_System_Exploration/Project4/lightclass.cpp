#include "lightclass.h"
LightClass::LightClass()
{
}
LightClass::LightClass(const LightClass& other)
{
}
LightClass::~LightClass()
{
}
void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}
void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}
void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}
void LightClass::SetAmbientSwitch(bool onoff)
{
	m_ambientSwitch = onoff;
}
void LightClass::SetDiffuseSwitch(bool onoff)
{
	m_diffuseSwitch = onoff;
}
void LightClass::SetSpecularSwitch(bool onoff)
{
	m_specularSwitch = onoff;
}
void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
	return;
}
D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}
D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}
D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}
D3DXVECTOR4 LightClass::GetPosition()
{
	return m_position;
}
float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

bool LightClass::GetAmbientSwitch()
{
	return m_ambientSwitch;
}

bool LightClass::GetDiffuseSwitch()
{
	return m_diffuseSwitch;
}

bool LightClass::GetSpecularSwitch()
{
	return m_specularSwitch;
}

void LightClass::Turn_On_OFFAmbientLight()
{
	//m_ambientSwitch = !m_ambientSwitch;
	if (m_ambientColor == D3DXVECTOR4(0.60f, 0.60f, 0.60f, 1.0f))
		SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if(m_ambientColor == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f))
	SetAmbientColor(0.60f, 0.60f, 0.60f, 1.0f);
}

void LightClass::Turn_On_OFFDiffuseLight()
{
	//m_diffuseSwitch = !m_diffuseSwitch;
	if (m_diffuseColor == D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f))
		SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if (m_diffuseColor == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f))
		SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void LightClass::Turn_On_OFFSpecularLight()
{

	m_specularSwitch = !m_specularSwitch;
	/*if (m_specularColor == D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f))
		SetSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if (m_specularColor == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f))
		SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);*/
}