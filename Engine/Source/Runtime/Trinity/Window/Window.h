#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Config.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/String/String.h"

#if defined(TRNT_USE_GLFW)
#include "Glfw/GlfwWindowHandle.h"
using TWindowHandle = TGlfwWindowHandle;
#elif defined(TRNT_PLATFORM_WIN64)
#include "Windows/WindowsWindowHandle.h"
using TWindowHandle = TWindowsWindowHandle;
#else
#	error "TWindowHandle isn't declared on current platform!"
#endif

#include "Trinity/Input/KeyCodes.h"
#include "Trinity/Input/MouseButtons.h"

#include "Trinity/Core/Logging/Log.h"

TRNT_DECLARE_LOG_INFO(Window, TLogLevel::EDebug);

class TWindowProperties
{
public:
	TString Title = "Trinity Engine";

	TUInt32 Width = 1280;
	TUInt32 Height = 720;

	TUInt32 XPosition = 50;
	TUInt32 YPosition = 50;

	TBool Resizable = true;
	TBool Borderless = false;
	TBool TFloating = false;
	TBool Fullscreen = false;
};

class TRNT_API TWindow
{
public:
	TWindow(const TWindowProperties& WindowProperties);

	~TWindow();

public:
	TBool Initialize();

	void Destroy();

	void Maximize();

	void Iconify();

	void Focus();

	void Close();

	void SetIcon(const TChar* IconFilePath);

	void SetTitle(const TString& Title);

	void SetWidth(TInt32 Width);

	void SetHeight(TInt32 Height);

	void SetSize(TInt32 Width, TInt32 Height);

	void SetXPosition(TInt32 XPosition);

	void SetYPosition(TInt32 YPosition);

	void SetPosition(TInt32 XPosition, TInt32 YPosition);

	void SetResizable(TBool Resizable);

	void SetBorderless(TBool Borderless);

	void SetTFloating(TBool TFloating);

	void SetFullscreen(TBool Fullscreen);

	const TChar* GetClipboardString() const;

	void SetClipboard(const TChar* String);

	void SetCursorHide(TBool Hide);

public:
	void ProcessInput();

public:
	TRNT_FORCE_INLINE TFloat AspectRatio() const
	{
		if (WindowProperties.Fullscreen)
		{
			return static_cast<TFloat>(FullscreenWidth) / static_cast<TFloat>(FullscreenHeight);
		}
		return static_cast<TFloat>(WindowProperties.Width) / static_cast<TFloat>(WindowProperties.Height);
	}

	TRNT_FORCE_INLINE TBool IsInitialized() const { return Initialized; }

	TRNT_FORCE_INLINE TBool IsFocused() const { return Focused; }

	TRNT_FORCE_INLINE TBool IsResized() const { return Resized; }

	TRNT_FORCE_INLINE TBool IsIconified() const { return Iconified; }

	TRNT_FORCE_INLINE TBool IsClosed() const { return Closed; }

	TRNT_FORCE_INLINE const TString& GetTitle() const { return WindowProperties.Title; }

	TRNT_FORCE_INLINE TInt32 GetWidth() const { return WindowProperties.Fullscreen ? FullscreenWidth : WindowProperties.Width; }

	TRNT_FORCE_INLINE TInt32 GetHeight() const { return WindowProperties.Fullscreen ? FullscreenHeight : WindowProperties.Height; }

	TRNT_FORCE_INLINE TInt32 GetXPosition() const { return WindowProperties.Fullscreen ? 0 : WindowProperties.XPosition; }

	TRNT_FORCE_INLINE TInt32 GetYPosition() const { return WindowProperties.Fullscreen ? 0 : WindowProperties.YPosition; }

	TRNT_FORCE_INLINE TBool IsResizable() const { return WindowProperties.Resizable; }

	TRNT_FORCE_INLINE TBool IsBorderless() const { return WindowProperties.Borderless; }

	TRNT_FORCE_INLINE TBool IsTFloating() const { return WindowProperties.TFloating; }

	TRNT_FORCE_INLINE TBool IsFullscreen() const { return WindowProperties.Fullscreen; }

	TRNT_FORCE_INLINE TDouble GetMouseXPosition() const { return MouseXPosition; }

	TRNT_FORCE_INLINE TDouble GetMouseYPosition() const { return MouseYPosition; }

	TRNT_FORCE_INLINE TBool CursorIsHidden() const { return CursorHidden; }

	TRNT_FORCE_INLINE void* GetNativeHandle() const { return WindowHandle; }

private:
	TWindowHandle WindowHandle;

	TWindowProperties WindowProperties;

	TInt32 FullscreenWidth;
	TInt32 FullscreenHeight;

	TDouble MouseXPosition;
	TDouble MouseYPosition;

	TBool Initialized;
	TBool Focused;
	TBool Resized;
	TBool Iconified;
	TBool Closed;

	TBool CursorHidden;

private:
#if defined(TRNT_USE_GLFW)
	friend void WindowPositionCallback(GLFWwindow* GLFWWindow, TInt32 XPosition, TInt32 YPosition);
	friend void WindowSizeCallback(GLFWwindow* GLFWWindow, TInt32 Width, TInt32 Height);
	friend void WindowCloseCallback(GLFWwindow* GLFWWindow);
	friend void WindowFocusCallback(GLFWwindow* GLFWWindow, TInt32 Focused);
	friend void WindowIconifyCallback(GLFWwindow* GLFWWindow, TInt32 Iconified);
	friend void FramebufferSizeCallback(GLFWwindow* GLFWWindow, TInt32 Width, TInt32 Height);
	friend void CursorEnterCallback(GLFWwindow* GLFWWindow, TInt32 Entered);
	friend void DropCallback(GLFWwindow* GLFWWindow, TInt32 Count, const TChar** Paths);
	friend void KeyCallback(GLFWwindow* GLFWWindow, TInt32 Key, TInt32 Scancode, TInt32 Action, TInt32 Mods);
	friend void CharCallback(GLFWwindow* GLFWWindow, TUInt32 Codepoint);
	friend void MouseButtonCallback(GLFWwindow* GLFWWindow, TInt32 Button, TInt32 Action, TInt32 Mods);
	friend void CursorPositionCallback(GLFWwindow* GLFWWindow, TDouble XPosition, TDouble YPosition);
	friend void ScrollCallback(GLFWwindow* GLFWWindow, TDouble XOffset, TDouble YOffset);
#endif
};

#if defined(TRNT_USE_GLFW)
static TRNT_API TKeyCode ConvertGlfwKeyToTKeyCode(TInt32 GlfwKeyCode);
static TRNT_API TMouseButton ConvertGlfwMouseToTMouseButton(TInt32 Button);
#elif defined(TRNT_PLATFORM_WIN64)

#else
#	error "Current platform keycodes and mouse buttons cannot be converted to TKeyCode and TMouseButton!"
#endif