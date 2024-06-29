#include "VulkanApp.h"

TVulkanApp::TVulkanApp()
	: TApplication("Vulkan Application")
{

}

TVulkanApp::~TVulkanApp()
{

}

void TVulkanApp::OnInitialize()
{
	// init something
	TLog::AddMessageHandlerCallback(TLog::DefaultStdoutMessageHandler);
	TInputManager::CreateInstance();

	// init main window
	TWindowProperties WindowProps{};
	WindowProps.Title = TApplication::ApplicationName;
	Window = new TWindow(WindowProps, true);
}

void TVulkanApp::OnUpdate(const TDouble DeltaTime)
{
	if (Window->IsClosed())
	{
		QuitRequested = true;
	}

	Window->ProcessInput();
}

void TVulkanApp::OnQuit()
{
	// destroy main window
	if (Window)
	{
		delete Window;
		Window = nullptr;
	}

	// release something
	TInputManager::DeleteInstance();
	TLog::ClearAllMessageHandlerCallbacks();
}