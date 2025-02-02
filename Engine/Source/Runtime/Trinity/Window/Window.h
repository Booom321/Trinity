#pragma once

#include "Trinity/Core/Config.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Logging/Log.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/String/String.h"
#include "Trinity/Input/InputAction.h"
#include "Trinity/Input/KeyCodes.h"
#include "Trinity/Input/ModifierKeys.h"
#include "Trinity/Input/MouseButtons.h"

#if defined(TRNT_USE_GLFW)
	#include "Glfw/GlfwWindowHandle.h"
#elif defined(TRNT_USE_WINDOWS_WINDOW)
	#include "Windows/WindowsWindowHandle.h"
#endif

#if defined(TRNT_USE_GLFW)
using TWindowHandle = TGlfwWindowHandle;
#elif defined(TRNT_USE_WINDOWS_WINDOW)
using TWindowHandle = TWindowsWindowHandle;
#else
	#error "TWindowHandle isn't declared on current platform!"
#endif

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
	TBool Floating = false;
	TBool Fullscreen = false;
};

class TRNT_API TWindow
{
public:
	TWindow(const TWindowProperties& WindowProperties, TBool ForceInit = false);

	~TWindow();

public:
	TBool Initialize();

	void Destroy();

	void Maximize();

	void Minimize();

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

	void SetFloating(TBool Floating);

	void SetFullscreen(TBool Fullscreen);

	const TChar* GetClipboardString() const;

	void SetClipboard(const TChar* String);

	void SetCursorHide(TBool Hide);

	void SetOpacity(TFloat Opacity);

public:
	void ProcessInput();

	void WaitEvents();

public:
	TRNT_FORCE_INLINE TFloat AspectRatio() const
	{
		if (WindowProperties.Fullscreen)
		{
			return static_cast<TFloat>(FullscreenWidth) / static_cast<TFloat>(FullscreenHeight);
		}
		return static_cast<TFloat>(WindowProperties.Width) / static_cast<TFloat>(WindowProperties.Height);
	}

	TRNT_FORCE_INLINE TBool IsInitialized() const
	{
		return Initialized;
	}

	TRNT_FORCE_INLINE TBool IsFocused() const
	{
		return Focused;
	}

	TRNT_FORCE_INLINE TBool IsIconified() const
	{
		return Iconified;
	}

	TRNT_FORCE_INLINE TBool IsClosed() const
	{
		return Closed;
	}

	TRNT_FORCE_INLINE const TString& GetTitle() const
	{
		return WindowProperties.Title;
	}

	TRNT_FORCE_INLINE TInt32 GetWidth() const
	{
		return WindowProperties.Fullscreen ? FullscreenWidth : WindowProperties.Width;
	}

	TRNT_FORCE_INLINE TInt32 GetHeight() const
	{
		return WindowProperties.Fullscreen ? FullscreenHeight : WindowProperties.Height;
	}

	TRNT_FORCE_INLINE TInt32 GetXPosition() const
	{
		return WindowProperties.Fullscreen ? 0 : WindowProperties.XPosition;
	}

	TRNT_FORCE_INLINE TInt32 GetYPosition() const
	{
		return WindowProperties.Fullscreen ? 0 : WindowProperties.YPosition;
	}

	TRNT_FORCE_INLINE TBool IsResizable() const
	{
		return WindowProperties.Resizable;
	}

	TRNT_FORCE_INLINE TBool IsBorderless() const
	{
		return WindowProperties.Borderless;
	}

	TRNT_FORCE_INLINE TBool IsFloating() const
	{
		return WindowProperties.Floating;
	}

	TRNT_FORCE_INLINE TBool IsFullscreen() const
	{
		return WindowProperties.Fullscreen;
	}

	TRNT_FORCE_INLINE TDouble GetMouseXPosition() const
	{
		return MouseXPosition;
	}

	TRNT_FORCE_INLINE TDouble GetMouseYPosition() const
	{
		return MouseYPosition;
	}

	TRNT_FORCE_INLINE TBool CursorIsHidden() const
	{
		return CursorHidden;
	}

	TRNT_FORCE_INLINE void* GetNativeHandle() const
	{
		return WindowHandle;
	}

	TRNT_FORCE_INLINE TFloat GetOpacity() const
	{
		return Opacity;
	}

private:
	TWindowHandle WindowHandle;

	TWindowProperties WindowProperties;

	TInt32 FullscreenWidth;
	TInt32 FullscreenHeight;

	TDouble MouseXPosition;
	TDouble MouseYPosition;

	TBool Initialized;
	TBool Focused;
	TBool Iconified;
	TBool Closed;

	TBool CursorHidden;
	TFloat Opacity;

public:
	using OnWindowMoveCallback = void (*)(TWindow* Window, TInt32 XPos, TInt32 YPos);
	using OnWindowResizeCallback = void (*)(TWindow* Window, TInt32 Width, TInt32 Height);
	using OnWindowCloseCallback = void (*)(TWindow* Window);
	using OnWindowFocusCallback = void (*)(TWindow* Window, TBool Focused);
	using OnWindowIconifyCallback = void (*)(TWindow* Window, TBool Iconified);
	using OnFramebufferSizeCallback = void (*)(TWindow* Window, TInt32 Width, TInt32 Height);
	using OnCursorEnterCallback = void (*)(TWindow* Window, TBool CursorEntered);
	using OnDropCallback = void (*)(TWindow* Window, TInt32 Count, const TChar** Paths);
	using OnKeyCallback = void (*)(TWindow* Window, TKeyCode Key, TInt32 Scancode, TInputAction Action, TModifierKey Mods);
	using OnCharCallback = void (*)(TWindow* Window, TChar Codepoint);
	using OnMouseButtonCallback = void (*)(TWindow* Window, TMouseButton Button, TInputAction Action, TModifierKey Mods);
	using OnCursorPositionCallback = void (*)(TWindow* Window, TDouble XPosition, TDouble YPosition);
	using OnScrollCallback = void (*)(TWindow* Window, TDouble XOffset, TDouble YOffset);

	TRNT_FORCE_INLINE OnWindowMoveCallback GetWindowMoveCallback() const
	{
		return OnWindowMove;
	}

	TRNT_FORCE_INLINE OnWindowResizeCallback GetWindowResizeCallback() const
	{
		return OnWindowResize;
	}

	TRNT_FORCE_INLINE OnWindowCloseCallback GetWindowCloseCallback() const
	{
		return OnWindowClose;
	}

	TRNT_FORCE_INLINE OnWindowFocusCallback GetWindowFocusCallback() const
	{
		return OnWindowFocus;
	}

	TRNT_FORCE_INLINE OnWindowIconifyCallback GetWindowIconifyCallback() const
	{
		return OnWindowIconify;
	}

	TRNT_FORCE_INLINE OnFramebufferSizeCallback GetFramebufferSizeCallback() const
	{
		return OnFramebufferSize;
	}

	TRNT_FORCE_INLINE OnCursorEnterCallback GetCursorEnterCallback() const
	{
		return OnCursorEnter;
	}

	TRNT_FORCE_INLINE OnDropCallback GetDropCallback() const
	{
		return OnDrop;
	}

	TRNT_FORCE_INLINE OnKeyCallback GetKeyCallback() const
	{
		return OnKey;
	}

	TRNT_FORCE_INLINE OnCharCallback GetCharCallback() const
	{
		return OnChar;
	}

	TRNT_FORCE_INLINE OnMouseButtonCallback GetMouseButtonCallback() const
	{
		return OnMouseButton;
	}

	TRNT_FORCE_INLINE OnCursorPositionCallback GetCursorPositionCallback() const
	{
		return OnCursorPosition;
	}

	TRNT_FORCE_INLINE OnScrollCallback GetScrollCallback() const
	{
		return OnScroll;
	}

	TRNT_FORCE_INLINE void SetWindowMoveCallback(OnWindowMoveCallback OnWindowMove)
	{
		this->OnWindowMove = OnWindowMove;
	}

	TRNT_FORCE_INLINE void SetWindowResizeCallback(OnWindowResizeCallback OnWindowResize)
	{
		this->OnWindowResize = OnWindowResize;
	}

	TRNT_FORCE_INLINE void SetWindowCloseCallback(OnWindowCloseCallback OnWindowClose)
	{
		this->OnWindowClose = OnWindowClose;
	}

	TRNT_FORCE_INLINE void SetWindowFocusCallback(OnWindowFocusCallback OnWindowFocus)
	{
		this->OnWindowFocus = OnWindowFocus;
	}

	TRNT_FORCE_INLINE void SetWindowIconifyCallback(OnWindowIconifyCallback OnWindowIconify)
	{
		this->OnWindowIconify = OnWindowIconify;
	}

	TRNT_FORCE_INLINE void SetFramebufferSizeCallback(OnFramebufferSizeCallback OnFramebufferSize)
	{
		this->OnFramebufferSize = OnFramebufferSize;
	}

	TRNT_FORCE_INLINE void SetCursorEnterCallback(OnCursorEnterCallback OnCursorEnter)
	{
		this->OnCursorEnter = OnCursorEnter;
	}

	TRNT_FORCE_INLINE void SetDropCallback(OnDropCallback OnDrop)
	{
		this->OnDrop = OnDrop;
	}

	TRNT_FORCE_INLINE void SetKeyCallback(OnKeyCallback OnKey)
	{
		this->OnKey = OnKey;
	}

	TRNT_FORCE_INLINE void SetCharCallback(OnCharCallback OnChar)
	{
		this->OnChar = OnChar;
	}

	TRNT_FORCE_INLINE void SetMouseButtonCallback(OnMouseButtonCallback OnMouseButton)
	{
		this->OnMouseButton = OnMouseButton;
	}

	TRNT_FORCE_INLINE void SetCursorPositionCallback(OnCursorPositionCallback OnCursorPosition)
	{
		this->OnCursorPosition = OnCursorPosition;
	}

	TRNT_FORCE_INLINE void SetScrollCallback(OnScrollCallback OnScroll)
	{
		this->OnScroll = OnScroll;
	}

private:
	OnWindowMoveCallback OnWindowMove;
	OnWindowResizeCallback OnWindowResize;
	OnWindowCloseCallback OnWindowClose;
	OnWindowFocusCallback OnWindowFocus;
	OnWindowIconifyCallback OnWindowIconify;
	OnFramebufferSizeCallback OnFramebufferSize;
	OnCursorEnterCallback OnCursorEnter;
	OnDropCallback OnDrop;
	OnKeyCallback OnKey;
	OnCharCallback OnChar;
	OnMouseButtonCallback OnMouseButton;
	OnCursorPositionCallback OnCursorPosition;
	OnScrollCallback OnScroll;

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
static TRNT_API TInputAction ConvertGlfwInputActionToTInputAction(TInt32 Action);
static TRNT_API TModifierKey ConvertGlfwModifierKeyToTModifierKey(TInt32 Mod);
#elif defined(TRNT_USE_WINDOWS_WINDOW)
// ...
#else
	#error "Current platform keycodes and mouse buttons cannot be converted to TKeyCode and TMouseButton!"
#endif