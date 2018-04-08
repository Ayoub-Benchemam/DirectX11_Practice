#include <iostream>
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>


LPCTSTR g_WndClassName = TEXT("firstwindow");
HWND g_hwnd = NULL;

const int g_Height = 600;
const int g_Width = 800;
float g_red = 0.0f;
float g_blue = 0.0f;
float g_green = 0.0f;
int g_colormodr = 1;
int g_colormodb = 1;
int g_colormodg = 1;

bool InitializeDirect3d11App(HINSTANCE hInstance);
void ReleaseObjects();
bool InitScene();
void UpdateScene();
void DrawScene();

bool IntializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (IntializeWindow(hInstance, nShowCmd, g_Width, g_Height, true))
	{
		MessageBox(0, TEXT("Window Initialization - Failed"), TEXT("Error"), MB_OK);
		return 0;
	}
		if (InitializeDirect3d11App(hInstance) == true)
		{
			MessageBox(0, TEXT("Direct3D Initialization - Failed"), TEXT("ERROR"), MB_OK);
			return 0;
		}

	if (InitScene() == true)
	{
		MessageBox(0, TEXT("Scene Initialization - Failed"), TEXT("ERROR"), MB_OK);
		return 0;
	}
	messageloop();

	return 0;
}

WNDCLASSEX wc;

int InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Error Register Class"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return 1;
	}
	g_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, g_WndClassName, TEXT("Window Title"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	if (!g_hwnd)
	{
		MessageBox(NULL, TEXT("Error creating window"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return 1;
	}
	BOOL ShowWndw(HWND hWnd, int nCmdShow);
	UpdateWindow(g_hwnd);

	return true;
}

int messageloop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		BOOL PeekMessageL(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
		if (PeekMessageL(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message = WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// run the game code
		}
	}
	return	msg.wParam;
}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, LPARAM lParam, WPARAM wParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				if (MessageBox(0, TEXT("Are you sure you want to exit?"), TEXT("Really?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
					DestroyWindow(hwnd);
			}
			return 0;

		case  WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, msg, lParam, wParam);
	}
	

IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;


void ReleaseObjects()
{
	//Release the COM Objects we created
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	HRESULT hr;

	//Describing our buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = g_Width;
	bufferDesc.Height = g_Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describing our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_MODE_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);

	return true;

}


bool InitScene()
{
	return true;
}

void UpdateScene()
{
	//Update the colors of ours scene
	g_red += g_colormodr * 0.00005f;
	g_green += g_colormodg * 0.00002f;
	g_blue += g_colormodb * 0.00001f;

	if (g_red >= 1.0f || g_red <= 0.0f)
		g_colormodr *= -1;
	if (g_green >= 1.0f || g_green <= 0.0f)
		g_colormodg *= -1;
	if (g_blue >= 1.0f || g_blue <= 0.0f)
		g_colormodb *= -1;
}

void DrawScene()
{
	//Clear our backbuffer to the updated color
	float bgColor[4] = { g_red, g_green, g_blue, 1.0f };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}

int messageloop2()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		BOOL PeekMessageL(
			LPMSG lmMsg,
			HWND hWnd,
			UINT wMsgFilterMin,
			UINT wMsgFilterMax,
			UINT wRemoveMsg
		);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			// run game code

			UpdateScene();
			DrawScene();

		}
	}
	return msg.wParam;
}

//Vertex structure

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
		: pos(x, y, z){}
	DirectX::XMFLOAT3 pos;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

UINT numElements = ARRAYSIZE(layout);

ID3D11Buffer* triangleVertexBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;

void CleanUp()
{
	//Release the COM Objects we created

	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
	renderTargetView->Release();
	triangleVertexBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
}

bool InitialScene()
{
	HRESULT hr;
	//Compile Shaders from shader file

	hr = D3DCompileFromFile(TEXT("Effects.fx"), 0, 0, "VS", "VS_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DCompileFromFile(TEXT("Effects.fx"), 0, 0, "PS", "PS_5_0", 0, 0, 0, &PS_Buffer, 0, 0);

	//Create the Shader Objects

	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreateVertexShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shader

	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	//Create the vertex buffer

	Vertex v[]
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = 0;
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertexBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &triangleVertexBuffer, &stride, &offset);

	//Create the Input Layout
	hr = d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = g_Height;
	viewport.Width = g_Width;

	//Set the Viewport
	d3d11DevCon->RSSetViewports(1, &viewport);

	return true; 

}