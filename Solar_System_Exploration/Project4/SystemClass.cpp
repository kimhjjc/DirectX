#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;

	m_Sound = 0;
	m_Booster = 0;
	m_Crash = 0;

	isBooster = false;
	isCrash = false;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;
	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);
	// Create the input object. This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}
	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	// Create the graphics object. This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}
	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}
	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}
	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}
	// Initialize the cpu object.
	m_Cpu->Initialize();
	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}
	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Sound = new SoundClass;

	result = m_Sound->Initialize(m_hwnd);

	m_Booster = new SoundClass;

	result = m_Booster->Initialize(m_hwnd);

	m_Crash = new SoundClass;

	result = m_Crash->Initialize(m_hwnd);

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_Booster)
	{
		m_Booster->Shutdown();
		delete m_Booster;
		m_Booster = 0;
	}

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}
	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}
	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}
	// Shutdown the window.
	ShutdownWindows();
	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}

			if (m_Input->IsLShiftPressed())
			{
				if (!isBooster)
				{
					m_Booster->PlayBoosterFile();
				}

				isBooster = true;

			}

			else
			{
				isBooster = false;
			}

			if (m_Graphics->m_Crash)
			{
				m_Crash->PlatCrashFile();

				m_Graphics->m_Crash = false;
			}
		}
		// Check if the user pressed escape and wants to quit.
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;
	float mouselX, mouselY;
	static float rotation = 0.0f;
	static float inputTimer[3] = { 0.0f, };		// m_Timer->GetTime() 함수가 무조건 1초당 1000으로 변환
	static float forwardShipSpeed = 0.0f;		// 1초에 앞으로 움직일 거리
	static float rightShipSpeed = 0.0f;		// 1초에 옆으로 움직일 거리
	float Acceleration = 100;
	float Acceleration2 = 10;
	float breakAcceleration = 0.5f;
	float realSpeed = 0.0f;		// 1초당 실제 움직이는 속도 계산법 :  realSpeed = (shipSpeed * m_Timer->GetTime()) / 1000 
	float SideMoveSpeed = 0.0f;	// 좌, 우로 움직이는 속도


	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}
	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	if (forwardShipSpeed > Acceleration * 3 || forwardShipSpeed < -Acceleration * 3 ||
		rightShipSpeed >  Acceleration * 3 || rightShipSpeed < -Acceleration * 3)
		breakAcceleration *= 10;

	if (forwardShipSpeed >= 7.0f)
		forwardShipSpeed -= Acceleration * Acceleration2 * breakAcceleration * m_Timer->GetTime() / 1000;
	else if (forwardShipSpeed <= -7.0f)
		forwardShipSpeed += Acceleration * Acceleration2 * breakAcceleration  * m_Timer->GetTime() / 1000;
	else
		forwardShipSpeed = 0;

	if (rightShipSpeed >= 7.0f)
		rightShipSpeed -= Acceleration * Acceleration2 * breakAcceleration  * m_Timer->GetTime()/ 1000;
	else if (rightShipSpeed <= -7.0f)
		rightShipSpeed += Acceleration * Acceleration2 * breakAcceleration  * m_Timer->GetTime()/ 1000;
	else
		rightShipSpeed = 0;

	if (m_Input->IsLShiftPressed())
	{
		Acceleration *= 10.0f;
	}
	if (m_Input->IsWPressed())
	{
		if (forwardShipSpeed < Acceleration * 3)
			forwardShipSpeed += Acceleration * Acceleration2 * m_Timer->GetTime() / 1000;

		if (forwardShipSpeed > Acceleration * 3)
			forwardShipSpeed = Acceleration * 3;
	}
	if (m_Input->IsSPressed())
	{
		if (forwardShipSpeed > -Acceleration * 3)
			forwardShipSpeed -= Acceleration * Acceleration2 * m_Timer->GetTime() / 1000;

		if (forwardShipSpeed < -Acceleration * 3)
			forwardShipSpeed = -Acceleration * 3;
	}

	realSpeed = (forwardShipSpeed * m_Timer->GetTime()) / 1000;

	if (m_Input->IsAPressed())
	{
		if (rightShipSpeed > -Acceleration * 3)
			rightShipSpeed -= Acceleration * Acceleration2 * m_Timer->GetTime() / 1000;

		if (rightShipSpeed < -Acceleration * 3)
			rightShipSpeed = -Acceleration * 3;
	}
	if (m_Input->IsDPressed())
	{
		if (rightShipSpeed < Acceleration * 3)
			rightShipSpeed += Acceleration * Acceleration2 * m_Timer->GetTime() / 1000;

		if (rightShipSpeed > Acceleration * 3)
			rightShipSpeed = Acceleration * 3;
	}

	SideMoveSpeed = (rightShipSpeed * m_Timer->GetTime()) / 1000;

	if (realSpeed != 0.0f)
	{
		m_Graphics->MoveForward(realSpeed);
	}
	if (SideMoveSpeed != 0.0f)
	{
		m_Graphics->StrafeRight(SideMoveSpeed);
	}

	// Update the rotation variable each frame.
	// 약 1초에 한바퀴 = D3DX_PI * 0.0001f * 400.0f --------- 계산법이 조금 틀림. m_Timer->GetTime() 함수 써서 해야함
	rotation = (float)D3DX_PI * 0.0001f * 400.0f * 0.1f;
	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(mouseX, mouseY);
	m_Input->GetMouseMovement(mouselX, mouselY);

	if (mouselX != 0.0f || mouselY != 0.0f)
	{
		m_Graphics->MouseMove(mouselX, mouselY, 0.3f);
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), forwardShipSpeed, rightShipSpeed, m_Timer->GetTime());
	if (!result)
	{
		return false;
	}
	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed())
	{
		//return false;
	}


	if (m_Input->IsNum1Pressed() && inputTimer[0] <= 0.0f)
	{
		m_Graphics->Get_m_Light()->Turn_On_OFFAmbientLight();
		inputTimer[0] = 500.0f;
	}
	if (m_Input->IsNum2Pressed() && inputTimer[1] <= 0.0f)
	{
		m_Graphics->Get_m_Light()->Turn_On_OFFDiffuseLight();
		inputTimer[1] = 500.0f;
	}
	if (m_Input->IsNum3Pressed() && inputTimer[2] <= 0.0f)
	{
		if (m_Graphics->Get_m_Light()->GetSpecularPower() > 50)
			m_Graphics->Get_m_Light()->SetSpecularPower(32);
		else
			m_Graphics->Get_m_Light()->SetSpecularPower(1000);

		inputTimer[2] = 500.0f;
	}

	// Finally render the graphics to the screen.
	result = m_Graphics->Render(rotation);
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		if (inputTimer[i] > 0.0f)
			inputTimer[i] -= m_Timer->GetTime();
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	// Get an external pointer to this object.
	ApplicationHandle = this;
	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);
	// Give the application a name.
	m_applicationName = L"Engine";
	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	// Register the window class.
	RegisterClassEx(&wc);
	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth,
		screenHeight, NULL, NULL, m_hinstance, NULL);
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	// Hide the mouse cursor.
	ShowCursor(false);
	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);
	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
	// Release the pointer to this class.
	ApplicationHandle = NULL;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}