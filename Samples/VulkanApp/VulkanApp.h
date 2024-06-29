#pragma once

#include <Trinity/Application/Application.h>
#include <Trinity/Window/Window.h>
#include <Trinity/Input/InputManager.h>

class TVulkanApp : public TApplication
{
public:
	TVulkanApp();
	
	~TVulkanApp();

public:
	void OnInitialize() override;

	void OnUpdate(const TDouble DeltaTime) override;

	void OnQuit() override;

private:
	TWindow* Window = nullptr;
};