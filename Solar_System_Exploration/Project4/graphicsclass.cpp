#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_EarthDistance = { 0,0,0 };
	m_Speed = 0;
	m_Life = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	struct filename {
		char* objName;
		WCHAR* textureName;
	};
	filename* filenames;

	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;

	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -50.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
		(WCHAR*)L"../Project4/data/seafloor.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}



	// Create the model object.
	m_ModelCount = 13;
	m_Model = new ModelClass[m_ModelCount];
	if (!m_Model)
	{
		return false;
	}
	filenames = new filename[m_ModelCount];
	if (!filenames)
	{
		return false;
	}

	filenames[0].objName = (char*)"../Project4/data/Spaceship.obj";
	filenames[0].textureName = (WCHAR*)L"../Project4/data/Spaceship.dds";
	filenames[1].objName = (char*)"../Project4/data/Planet.obj";
	filenames[1].textureName = (WCHAR*)L"../Project4/data/Solar.dds";
	filenames[2].objName = (char*)"../Project4/data/Planet.obj";					// 수성
	filenames[2].textureName = (WCHAR*)L"../Project4/data/Murcury.dds";
	filenames[3].objName = (char*)"../Project4/data/Planet.obj";					// 금성
	filenames[3].textureName = (WCHAR*)L"../Project4/data/Venus.dds";
	filenames[4].objName = (char*)"../Project4/data/Planet.obj";					// 지구
	filenames[4].textureName = (WCHAR*)L"../Project4/data/Earth.dds";
	filenames[5].objName = (char*)"../Project4/data/Planet.obj";					// 화성
	filenames[5].textureName = (WCHAR*)L"../Project4/data/Mars.dds";
	filenames[6].objName = (char*)"../Project4/data/Planet.obj";					// 목성
	filenames[6].textureName = (WCHAR*)L"../Project4/data/Jupiter.dds";
	filenames[7].objName = (char*)"../Project4/data/Planet.obj";					// 토성
	filenames[7].textureName = (WCHAR*)L"../Project4/data/Saturn.dds";
	filenames[8].objName = (char*)"../Project4/data/Planet.obj";					// 천왕성
	filenames[8].textureName = (WCHAR*)L"../Project4/data/Uranus.dds";
	filenames[9].objName = (char*)"../Project4/data/Planet.obj";					// 해왕성
	filenames[9].textureName = (WCHAR*)L"../Project4/data/Neptune.dds";
	filenames[10].objName = (char*)"../Project4/data/Planet.obj";					// 백그라운드	
	filenames[10].textureName = (WCHAR*)L"../Project4/data/Space_Background.dds";
	filenames[11].objName = (char*)"../Project4/data/small_planet_middle.obj";		// 소행성
	filenames[11].textureName = (WCHAR*)L"../Project4/data/Neptune.dds";
	filenames[12].objName = (char*)"../Project4/data/small_planet_small.obj";
	filenames[12].textureName = (WCHAR*)L"../Project4/data/Neptune.dds";
	for (int i = 0; i < m_ModelCount; i++)
	{
		if (!&m_Model[i])
		{
			return false;
		}
		// Initialize the model object.
		result = m_Model[i].Initialize(m_D3D->GetDevice(), filenames[i].objName, filenames[i].textureName, i);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the color shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 3; i++)
		sceneColor[i] = 0.0f;
	sceneColor[3] = 1.0f;

	char cardName[128] = { 0 };
	int cardMemory;

	m_D3D->GetVideoCardInfo(cardName, cardMemory);

	ofstream fout;
	// Open a file to write the file name.
	fout.open("VideoInfo.txt");
	// Write out the CardInfo message.

	fout << "CardName is" << cardName << ", Memory amount is " << cardMemory;

	// Close the file.
	fout.close();

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.6f, 0.6f, 0.6f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.5f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	m_Light->SetAmbientSwitch(true);
	m_Light->SetDiffuseSwitch(true);
	m_Light->SetSpecularSwitch(true);

	m_Camera->SetPosition(-2000.0f, 1.0f, -1500.0f);
	m_Camera->SetRotation(0.0f, 80.0f, 0.0f);

	if (filenames)
	{
		delete[] filenames;
	}

	m_Life = 100;


	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the model object.
	if (m_Model)
	{
		for (int i = 0; i < m_ModelCount; i++)
			m_Model->Shutdown();

		delete[] m_Model;
		m_Model = 0;
	}
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float shipSpeed, float frameTime)
{
	bool result;
	int totalVertexCount = 0;
	long int earthDistance = (long int)D3DXVec3Length(&m_EarthDistance);

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// 폴리곤개수
	for (int i = 0; i < m_ModelCount; i++)
		totalVertexCount += m_Model[i].GetIndexCount();

	result = m_Text->SetPolygon(totalVertexCount / 3, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// 지구와의 거리
	result = m_Text->SetEarthDistance(m_EarthDistance, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// 기체 속도
	result = m_Text->SetShipSpeed(shipSpeed, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	m_Speed = shipSpeed;
	// 라이프 시스템
	result = m_Text->SetLife(m_Life, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	return true;
}

void GraphicsClass::SetSceneColor(float red, float green, float blue, float alpha)
{
	sceneColor[0] = red;
	sceneColor[1] = green;
	sceneColor[2] = blue;
	sceneColor[3] = alpha;
}

bool GraphicsClass::Render(float rotationPower)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	D3DXMATRIX rotation;
	D3DXMATRIX transform;
	D3DXMATRIX scale;
	D3DXMATRIX revolution;
	D3DXMATRIX* m_worldMatrix;
	static float* m_revolution;
	static float* m_rotation;
	bool result;
	D3DXVECTOR3 cameraPosition = m_Camera->GetPosition();
	D3DXVECTOR3 cameraRotation = m_Camera->GetRotation();
	D3DXVECTOR3 playerPosition;
	D3DXVECTOR3 playerRotation;
	D3DXVECTOR3 earthPosition;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(sceneColor[0], sceneColor[1], sceneColor[2], sceneColor[3]);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	if (!m_rotation)
	{
		m_rotation = new float[m_ModelCount];
		for (int i = 0; i < m_ModelCount; i++)
			m_rotation[i] = 0.0f;
	}
	if (!m_revolution)
	{
		m_revolution = new float[m_ModelCount];
		for (int i = 0; i < m_ModelCount; i++)
			m_revolution[i] = 0.0f;
	}
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_worldMatrix = new D3DXMATRIX[m_ModelCount];
	if (!m_worldMatrix)
	{
		return false;
	}
	for (int i = 0; i < m_ModelCount; i++)
	{
		m_worldMatrix[i] = worldMatrix;
	}

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	playerRotation.x = cameraRotation.x * 0.0174532925f;	//pitch
	playerRotation.y = (cameraRotation.y - 90.0f) * 0.0174532925f;	//yaw
	playerRotation.z = cameraRotation.z * 0.0174532925f;	//roll

	playerPosition = cameraPosition;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	// Player, 비행기
	D3DXMATRIX playerSet;		// 모델을 카메라 앞에 갖다 둔다.
	D3DXMatrixTranslation(&playerSet, 50, -15, 0);
	D3DXMatrixScaling(&scale, 0.1, 0.1, 0.1);
	D3DXMatrixTranslation(&transform, playerPosition.x, playerPosition.y, playerPosition.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, playerRotation.y, 0, -playerRotation.x);
	m_worldMatrix[0] *= scale * playerSet * rotation * transform;	// 벡터 순서 = 크기 * 카메라와의 거리 * 카메라가 보는 방향 * 카메라의 위치

	// 태양
	D3DXMatrixScaling(&scale, 139.2 / 2, 139.2 / 2, 139.2 / 2);		// 실제 크기는 지구의 139.2 배가 맞지만 너무 커서 반으로 줄임
	D3DXMatrixTranslation(&transform, 0, 0, 0);
	m_worldMatrix[1] *= scale * transform;

	// 행성의 자전과 공전
	//for (int i = 2; i < m_ModelCount; i++)
	for (int i = 2; i <= 9; i++)
	{
		float rotationCycle;
		float revolutionCycle;
		switch (i)
		{
		case 2:	// 수성
			rotationCycle = 59.000;							// 자전
			revolutionCycle = 0.2409;						// 공전
			D3DXMatrixScaling(&scale, 0.25 * 2, 0.25 * 2, 0.25 * 2);	// 크기		// 너무 작아서 잘 안보이기 때문에 두배로 늘림
			D3DXMatrixTranslation(&transform, 579.1 + 500, 0, 0);	// 거리
			break;
		case 3:	// 금성
			rotationCycle = 223.00;
			revolutionCycle = 0.6102;
			D3DXMatrixScaling(&scale, 0.60 * 2, 0.60 * 2, 0.60 * 2);
			D3DXMatrixTranslation(&transform, 1082.0 + 500, 0, 0);
			break;
		case 4:	// 지구
			rotationCycle = 1.0000;
			revolutionCycle = 1.0000;
			D3DXMatrixScaling(&scale, 0.64 * 2, 0.64 * 2, 0.64 * 2);
			D3DXMatrixTranslation(&transform, 1496.0 + 500, 0, 0);
			break;
		case 5:	// 화성
			rotationCycle = 1.0250;
			revolutionCycle = 1.8809;
			D3DXMatrixScaling(&scale, 0.34 * 2, 0.34 * 2, 0.34 * 2);
			D3DXMatrixTranslation(&transform, 2279.4 + 500, 0, 0);
			break;
		case 6:	// 목성
			rotationCycle = 0.4097;
			revolutionCycle = 11.862;
			D3DXMatrixScaling(&scale, 7.1 * 2, 7.1 * 2, 7.1 * 2);
			D3DXMatrixTranslation(&transform, 7783.3 + 500, 0, 0);
			break;
		case 7:	// 토성
			rotationCycle = 0.4263;
			revolutionCycle = 29.458;
			D3DXMatrixScaling(&scale, 6.0 * 2, 6.0 * 2, 6.0 * 2);
			D3DXMatrixTranslation(&transform, 14294.0 + 500, 0, 0);
			break;
		case 8:	// 천왕성
			rotationCycle = 0.6250;
			revolutionCycle = 84.022;
			D3DXMatrixScaling(&scale, 2.79 * 2, 2.79 * 2, 2.79 * 2);
			D3DXMatrixTranslation(&transform, 28709.9 + 500, 0, 0);
			break;
		case 9:	// 해왕성
			rotationCycle = 0.6375;
			revolutionCycle = 164.774;
			D3DXMatrixScaling(&scale, 2.43 * 2, 2.43 * 2, 2.43 * 2);
			D3DXMatrixTranslation(&transform, 45040.0 + 500, 0, 0);
			break;
		default:
			break;
		}
		// rotationPower == 10초에 하루가 지나가는 것을 기준.
		// (속도 / 주기) / 날짜
		m_rotation[i] += (rotationPower / rotationCycle);
		m_revolution[i] += (rotationPower / revolutionCycle) / 365.0f;
		while (m_rotation[i] > 360.0f)
			m_rotation[i] -= 360.0f;
		while (m_revolution[i] > 360.0f)
			m_revolution[i] -= 360.0f;

		D3DXMatrixRotationY(&rotation, m_rotation[i]);
		D3DXMatrixRotationY(&revolution, m_revolution[i]);

		m_worldMatrix[i] *= rotation * scale * transform * revolution;
	}

	// 지구와 기체의 거리
	earthPosition.x = m_worldMatrix[4]._41;	// x			// 지구의 위치 계산 -> 지구 - 기체 = 지구와의 거리
	earthPosition.y = m_worldMatrix[4]._42;	// y
	earthPosition.z = m_worldMatrix[4]._43;	// z
	m_EarthDistance = earthPosition - playerPosition;

	// 백그라운드
	D3DXMatrixScaling(&scale, 10000, 10000, 10000);		// 
	D3DXMatrixTranslation(&transform, 0, 0, 0);
	m_worldMatrix[10] *= scale * transform;

	// 중간 소행성
	D3DXMatrixScaling(&scale, 1, 1, 1);		// 
	D3DXMatrixTranslation(&transform, -2000, 0, -1600);
	m_worldMatrix[11] *= scale * transform;

	// 큰 소행성
	D3DXMatrixScaling(&scale, 1, 1, 1);		// 
	D3DXMatrixTranslation(&transform, -2000, 0, -1700);
	m_worldMatrix[12] *= scale * transform;

	for (int i = 0; i < m_ModelCount; i++)
	{
		m_Model[i].Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i].GetIndexCount(), m_worldMatrix[i],
			viewMatrix, projectionMatrix, m_Model[i].GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Light->GetAmbientSwitch(),
			m_Light->GetDiffuseSwitch(), m_Light->GetSpecularSwitch());
		if (!result)
		{
			return false;
		}
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	if (m_worldMatrix)
	{
		delete[] m_worldMatrix;
	}


	return true;
}

void GraphicsClass::MoveForward(float speed)
{
	D3DXVECTOR3 cameraPosition = m_Camera->GetPosition();
	D3DXVECTOR3 cameraRotation = m_Camera->GetRotation();

	float radianY = cameraRotation.y * 3.141592 / 180;
	float radianX = cameraRotation.x * 3.141592 / 180;

	cameraPosition.x += sinf(radianY) * speed;
	cameraPosition.z += cosf(radianY) * speed;
	cameraPosition.y += -tanf(radianX) * speed;

	m_Camera->SetPosition(cameraPosition);
}

void GraphicsClass::StrafeRight(float speed)
{
	D3DXVECTOR3 cameraPosition = m_Camera->GetPosition();
	D3DXVECTOR3 cameraRotation = m_Camera->GetRotation();

	float radian = (cameraRotation.y + 90.0f) * 3.141592 / 180;
	cameraPosition.x += sinf(radian) * speed;
	cameraPosition.z += cosf(radian) * speed;

	m_Camera->SetPosition(cameraPosition);
}
//마우스x랑 앞뒤가 cameraRotation.y값 뒤집어질때 반대로됨
void GraphicsClass::MouseMove(float mouselX, float mouselY, float speed)
{
	D3DXVECTOR3 cameraRotation = m_Camera->GetRotation();

	cameraRotation.x += mouselY / 5;
	cameraRotation.y += mouselX / 5;



	m_Camera->SetRotation(cameraRotation);
}