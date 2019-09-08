#include "stdafx.h"
#include "Engine.h"

#include "Graphics.h"
#include "RenderWindow.h"
#include "Singletons.h"

#include "TestComponent.h"

Engine* Engine::CreateOrNull(HINSTANCE hInstance, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight)
{
	Assert(hInstance != nullptr);

	Engine* instance = new Engine();
	if (!instance->initialize(hInstance, windowTitle, windowClassName, windowWidth, windowHeight))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

void Engine::RunEngine()
{
	while (processMessages())
	{
		update();
		renderFrame();
	}

	Release();
}

bool Engine::processMessages()
{
	return mRenderWindow->ProcessMessages();
}

void Engine::update()
{
	Input::GetInstance()->Update();
	ActorManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update();

	if (Input::GetInstance()->IsKeyPressed(DIK_W))
	{
		CameraManager::GetInstance()->MainCamera->AdjustEye(XMFLOAT3(0.0f, 0.05f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_S))
	{
		CameraManager::GetInstance()->MainCamera->AdjustEye(XMFLOAT3(0.0f, -0.05f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_A))
	{
		CameraManager::GetInstance()->MainCamera->AdjustEye(XMFLOAT3(-0.05f, 0.0f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_D))
	{
		CameraManager::GetInstance()->MainCamera->AdjustEye(XMFLOAT3(0.05f, 0.0f, 0.0f));
	}

	if (Input::GetInstance()->IsKeyPressed(DIK_UPARROW))
	{
		CameraManager::GetInstance()->MainCamera->AdjustRotation(XMFLOAT3(-0.01f, 0.0f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_DOWNARROW))
	{
		CameraManager::GetInstance()->MainCamera->AdjustRotation(XMFLOAT3(0.01f, 0.0f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_LEFTARROW))
	{
		CameraManager::GetInstance()->MainCamera->AdjustRotation(XMFLOAT3(0.0f, -0.01f, 0.0f));
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_RIGHTARROW))
	{
		CameraManager::GetInstance()->MainCamera->AdjustRotation(XMFLOAT3(0.0f, 0.01f, 0.0f));
	}
}

void Engine::renderFrame()
{
	mGraphics->RenderFrame();
}

Engine::Engine()
	: Application()
{
}

bool Engine::initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight)
{
	mRenderWindow = RenderWindow::CreateOrNull(hInstance, windowTitle, windowClassName, windowWidth, windowHeight, this);
	if (mRenderWindow == nullptr)
	{
		ErrorLog("RenderWindow create failed.");

		return false;
	}

	mGraphics = Graphics::CreateOrNull(mRenderWindow->WindowHandle, windowWidth, windowHeight);
	if (mGraphics == nullptr)
	{
		ErrorLog("Graphics create failed.");

		return false;
	}

	if (!loadComponents())
	{
		return false;
	}

	if (!loadResources())
	{
		return false;
	}
	
	if (!loadActors())
	{
		return false;
	}

	if (!loadCameras())
	{
		return false;
	}

	if (!Input::GetInstance()->Initialize(hInstance, mRenderWindow->WindowHandle, mRenderWindow->Width, mRenderWindow->Height))
	{
		return false;
	}

	return true;
}

bool Engine::loadComponents() const
{
	if (!ComponentFactory::GetInstance()->Initialize())
	{
		ErrorLog("ComponentFactory initialize failed.");

		return false;
	}

	ComponentFactory::GetInstance()->RegisterPrototype(L"Test", TestComponent::Create());

	return true;
}

bool Engine::loadResources() const
{
	ResourceFactory* factory = ResourceFactory::GetInstance();

	bool bResult = true;
	bResult &= factory->RegisterPrototype(L"RectBuffer", RectBuffer::CreateOrNull(mGraphics->Device, mGraphics->DeviceContext));
	bResult &= factory->RegisterPrototype(L"Texture_Test", Texture::CreateOrNull(mGraphics->Device, mGraphics->DeviceContext, L"Resources\\Textures\\niniz.png"));
	bResult &= factory->RegisterPrototype(L"VS_Test", TVertexShader<CB_VS_WVP>::CreateOrNull(mGraphics->Device, mGraphics->DeviceContext, L"Resources\\Shaders\\VertexShader.cso"));
	bResult &= factory->RegisterPrototype(L"PS_Test", TPixelShader<CB_PS_EMPTY>::CreateOrNull(mGraphics->Device, mGraphics->DeviceContext, L"Resources\\Shaders\\PixelShader.cso"));
	bResult &= factory->RegisterPrototype(L"PS_Test_Color", TPixelShader<CB_PS_COLOR>::CreateOrNull(mGraphics->Device, mGraphics->DeviceContext, L"Resources\\Shaders\\PixelShader.cso"));

	return bResult;
}

bool Engine::loadActors() const
{
	Actor* actor = Actor::Create();
	actor->EquipComponent<Transform>(L"Transform", L"Transform", XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
	actor->EquipComponent<Renderer2D>(L"Renderer", L"Renderer2D", mGraphics->Device, mGraphics->DeviceContext, L"Texture_Test", L"VS_Test", L"PS_Test_Color");
	actor->EquipComponent<TestComponent>(L"Test", L"Test");
	ActorManager::GetInstance()->RegisterActor(actor);

	return true;
}

bool Engine::loadCameras() const
{
	Camera* camera = Camera::Create();
	camera->EquipComponent<CameraComponent>(L"CameraComponent", L"CameraComponent", CameraComponent::eProjectionType::Orthographic, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XM_PI / 4.0f, 4.0f, 3.0f, 0.1f, 1000.0f);
	//camera->EquipComponent<CameraComponent>(L"CameraComponent", L"CameraComponent", CameraComponent::eProjectionType::Perspective, XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XM_PI / 4.0f, static_cast<float>(mRenderWindow->Width), static_cast<float>(mRenderWindow->Height), 0.1f, 1000.0f);
	CameraManager::GetInstance()->RegisterCamera(L"MainCamera", camera);
	CameraManager::GetInstance()->SetMainCamera(L"MainCamera");

	return true;
}

void Engine::destroy()
{
	Input::GetInstance()->DestroyInstance();
	RenderManager::GetInstance()->DestroyInstance();
	ResourceFactory::GetInstance()->DestroyInstance();
	ComponentFactory::GetInstance()->DestroyInstance();
	CameraManager::GetInstance()->DestroyInstance();
	ActorManager::GetInstance()->DestroyInstance();

	Application::destroy();
}

