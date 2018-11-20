#include "RenderingGame.h"
#include "GameException.h"
#include "FirstPersonCamera.h"
#include "TriangleDemo.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ModelFromFile.h"

#include "FpsComponent.h"
#include "RenderStateHelper.h"


namespace Rendering
{
	;

	const XMFLOAT4 RenderingGame::BackgroundColor = { 0.5f, 0.5f, 0.5f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mModel(nullptr),
		mFpsComponent(nullptr), mRenderStateHelper(nullptr), mDemo(nullptr)
	{
		mDepthStencilBufferEnabled = true;
		mMultiSamplingEnabled = true;
	}

	RenderingGame::~RenderingGame()
	{
	}

	void RenderingGame::Initialize()
	{

		mCamera = new FirstPersonCamera(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera);

		mDemo = new TriangleDemo(*this, *mCamera);
		mComponents.push_back(mDemo);

		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw GameException("DirectInput8Creat() failed");

		}
		mKeyboard = new Keyboard(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);

		mMouse = new Mouse(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(Mouse::TypeIdClass(), mMouse);

		//--------------------------------------DRAWING-------------------------------------------------------------//
		//(rotx,roty,rotz,scale,posx,posy,posz)
		//mModel->clearTexture();

		//<-------- Kitchen Level - Cubes Positioned based on Level Design -------->   
		
		//note that setScale MUST be before setPosition for everything to work correctly


		//Fridge
		mModel = new ModelFromFile(*this, *mCamera, "Content\\Models\\cube.3ds");
		//mModel->SetPosition(-1.57f, -0.0f, -0.0f, 0.05f, -2.0f, 0.0f, 0.0f);
		mModel->setScale(0.05f, 0.05f, 1.0f);
		mModel->SetPosition(-1.57f, -0.0f, -0.0f, -2.0f, 0.0f, 0.0f);
		//mModel->setTexture(L"Content\\Textures\\appleD.jpg");
		mComponents.push_back(mModel);

		//Shelf
		mModel = new ModelFromFile(*this, *mCamera, "Content\\Models\\cube.3ds");
		mModel->SetPosition(-1.57f, -0.0f, -0.0f, 0.05f, 0.0f, 3.0f, 0.0f);
		//mModel->setTexture(L"Content\\Textures\\appleD.jpg");
		mComponents.push_back(mModel);

		//Counter
		mModel = new ModelFromFile(*this, *mCamera, "Content\\Models\\cube.3ds");
		mModel->SetPosition(-1.57f, -0.0f, -0.0f, 0.05f, 3.0f, 0.0f, 0.0f);
		//mModel->setTexture(L"Content\\Textures\\appleD.jpg");
		mComponents.push_back(mModel);

		//Box on Counter
		mModel = new ModelFromFile(*this, *mCamera, "Content\\Models\\cube.3ds");
		mModel->SetPosition(-1.57f, -0.0f, -0.0f, 0.05f, 3.0f, 1.5f, 0.0f);
		//mModel->setTexture(L"Content\\Textures\\appleD.jpg");
		mComponents.push_back(mModel);

		//Box at end of Kitchen 
		mModel = new ModelFromFile(*this, *mCamera, "Content\\Models\\cube.3ds");
		mModel->SetPosition(-1.57f, -0.0f, -0.0f, 0.05f, 6.0f, 0.0f, 0.0f);
		//mModel->setTexture(L"Content\\Textures\\appleD.jpg");
		mComponents.push_back(mModel);
		
		//--------------------------------------FINISHED DRAWING----------------------------------------------------//

		mFpsComponent = new FpsComponent(*this);
		mFpsComponent->Initialize();

		mRenderStateHelper = new RenderStateHelper(*this);



		Game::Initialize();

		mCamera->SetPosition(0.0f, 2.0f, 10.0f);
	}

	void RenderingGame::Shutdown()
	{
		DeleteObject(mDemo);
		DeleteObject(mCamera);
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		ReleaseObject(mDirectInput);
		DeleteObject(mFpsComponent);
		DeleteObject(mRenderStateHelper);


		DeleteObject(mModel);




		Game::Shutdown();
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{

		Game::Update(gameTime);

		mFpsComponent->Update(gameTime);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}


	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper->SaveAll();
		mFpsComponent->Draw(gameTime);
		mRenderStateHelper->RestoreAll();

		HRESULT hr = mSwapChain->Present(0, 0);
		if (FAILED(hr))
		{
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}


	}
}