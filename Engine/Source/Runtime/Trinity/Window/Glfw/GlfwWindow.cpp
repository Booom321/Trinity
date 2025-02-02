#include "TrinityPCH.h"

#include "Trinity/Window/Window.h"

#pragma warning(push)
#pragma warning(disable : 26495)
#pragma warning(disable : 6011)
#if defined(TRNT_USE_GLFW)

	#include "Trinity/Input/InputManager.h"

	#include <stb_image.h>
#endif

#if defined(TRNT_USE_GLFW)

TKeyCode ConvertGlfwKeyToTKeyCode(TInt32 GlfwKeyCode)
{
	switch (GlfwKeyCode)
	{
		case GLFW_KEY_SPACE:
			return TKeyCode::ESpace;
		case GLFW_KEY_APOSTROPHE:
			return TKeyCode::EApostrophe;
		case GLFW_KEY_COMMA:
			return TKeyCode::EComma;
		case GLFW_KEY_MINUS:
			return TKeyCode::EMinus;
		case GLFW_KEY_PERIOD:
			return TKeyCode::EPeriod;
		case GLFW_KEY_SLASH:
			return TKeyCode::ESlash;
		case GLFW_KEY_0:
			return TKeyCode::E_0;
		case GLFW_KEY_1:
			return TKeyCode::E_1;
		case GLFW_KEY_2:
			return TKeyCode::E_2;
		case GLFW_KEY_3:
			return TKeyCode::E_3;
		case GLFW_KEY_4:
			return TKeyCode::E_4;
		case GLFW_KEY_5:
			return TKeyCode::E_5;
		case GLFW_KEY_6:
			return TKeyCode::E_6;
		case GLFW_KEY_7:
			return TKeyCode::E_7;
		case GLFW_KEY_8:
			return TKeyCode::E_8;
		case GLFW_KEY_9:
			return TKeyCode::E_9;
		case GLFW_KEY_SEMICOLON:
			return TKeyCode::ESemicolon;
		case GLFW_KEY_EQUAL:
			return TKeyCode::EEqual;
		case GLFW_KEY_A:
			return TKeyCode::E_A;
		case GLFW_KEY_B:
			return TKeyCode::E_B;
		case GLFW_KEY_C:
			return TKeyCode::E_C;
		case GLFW_KEY_D:
			return TKeyCode::E_D;
		case GLFW_KEY_E:
			return TKeyCode::E_E;
		case GLFW_KEY_F:
			return TKeyCode::E_F;
		case GLFW_KEY_G:
			return TKeyCode::E_G;
		case GLFW_KEY_H:
			return TKeyCode::E_H;
		case GLFW_KEY_I:
			return TKeyCode::E_I;
		case GLFW_KEY_J:
			return TKeyCode::E_J;
		case GLFW_KEY_K:
			return TKeyCode::E_K;
		case GLFW_KEY_L:
			return TKeyCode::E_L;
		case GLFW_KEY_M:
			return TKeyCode::E_M;
		case GLFW_KEY_N:
			return TKeyCode::E_N;
		case GLFW_KEY_O:
			return TKeyCode::E_O;
		case GLFW_KEY_P:
			return TKeyCode::E_P;
		case GLFW_KEY_Q:
			return TKeyCode::E_Q;
		case GLFW_KEY_R:
			return TKeyCode::E_R;
		case GLFW_KEY_S:
			return TKeyCode::E_S;
		case GLFW_KEY_T:
			return TKeyCode::E_T;
		case GLFW_KEY_U:
			return TKeyCode::E_U;
		case GLFW_KEY_V:
			return TKeyCode::E_V;
		case GLFW_KEY_W:
			return TKeyCode::E_W;
		case GLFW_KEY_X:
			return TKeyCode::E_X;
		case GLFW_KEY_Y:
			return TKeyCode::E_Y;
		case GLFW_KEY_Z:
			return TKeyCode::E_Z;
		case GLFW_KEY_LEFT_BRACKET:
			return TKeyCode::ELeftBracket;
		case GLFW_KEY_BACKSLASH:
			return TKeyCode::EBackSlash;
		case GLFW_KEY_RIGHT_BRACKET:
			return TKeyCode::ERightBracket;
		case GLFW_KEY_GRAVE_ACCENT:
			return TKeyCode::EGraveAccent;
		case GLFW_KEY_WORLD_1:
			return TKeyCode::EWorld1;
		case GLFW_KEY_WORLD_2:
			return TKeyCode::EWorld2;
		case GLFW_KEY_ESCAPE:
			return TKeyCode::EEscape;
		case GLFW_KEY_ENTER:
			return TKeyCode::EEnter;
		case GLFW_KEY_TAB:
			return TKeyCode::ETab;
		case GLFW_KEY_BACKSPACE:
			return TKeyCode::EBackspace;
		case GLFW_KEY_INSERT:
			return TKeyCode::EInsert;
		case GLFW_KEY_DELETE:
			return TKeyCode::EDelete;
		case GLFW_KEY_RIGHT:
			return TKeyCode::ERight;
		case GLFW_KEY_LEFT:
			return TKeyCode::ELeft;
		case GLFW_KEY_DOWN:
			return TKeyCode::EDown;
		case GLFW_KEY_UP:
			return TKeyCode::EUp;
		case GLFW_KEY_PAGE_UP:
			return TKeyCode::EPageUp;
		case GLFW_KEY_PAGE_DOWN:
			return TKeyCode::EPageDown;
		case GLFW_KEY_HOME:
			return TKeyCode::EHome;
		case GLFW_KEY_END:
			return TKeyCode::EEnd;
		case GLFW_KEY_CAPS_LOCK:
			return TKeyCode::ECapsLock;
		case GLFW_KEY_SCROLL_LOCK:
			return TKeyCode::EScrollLock;
		case GLFW_KEY_NUM_LOCK:
			return TKeyCode::ENumLock;
		case GLFW_KEY_PRINT_SCREEN:
			return TKeyCode::EPrintScr;
		case GLFW_KEY_PAUSE:
			return TKeyCode::EPause;
		case GLFW_KEY_F1:
			return TKeyCode::EF1;
		case GLFW_KEY_F2:
			return TKeyCode::EF2;
		case GLFW_KEY_F3:
			return TKeyCode::EF3;
		case GLFW_KEY_F4:
			return TKeyCode::EF4;
		case GLFW_KEY_F5:
			return TKeyCode::EF5;
		case GLFW_KEY_F6:
			return TKeyCode::EF6;
		case GLFW_KEY_F7:
			return TKeyCode::EF7;
		case GLFW_KEY_F8:
			return TKeyCode::EF8;
		case GLFW_KEY_F9:
			return TKeyCode::EF9;
		case GLFW_KEY_F10:
			return TKeyCode::EF10;
		case GLFW_KEY_F11:
			return TKeyCode::EF11;
		case GLFW_KEY_F12:
			return TKeyCode::EF12;
		case GLFW_KEY_F13:
			return TKeyCode::EF13;
		case GLFW_KEY_F14:
			return TKeyCode::EF14;
		case GLFW_KEY_F15:
			return TKeyCode::EF15;
		case GLFW_KEY_F16:
			return TKeyCode::EF16;
		case GLFW_KEY_F17:
			return TKeyCode::EF17;
		case GLFW_KEY_F18:
			return TKeyCode::EF18;
		case GLFW_KEY_F19:
			return TKeyCode::EF19;
		case GLFW_KEY_F20:
			return TKeyCode::EF20;
		case GLFW_KEY_F21:
			return TKeyCode::EF21;
		case GLFW_KEY_F22:
			return TKeyCode::EF22;
		case GLFW_KEY_F23:
			return TKeyCode::EF23;
		case GLFW_KEY_F24:
			return TKeyCode::EF24;
		case GLFW_KEY_F25:
			return TKeyCode::EF25;
		case GLFW_KEY_KP_0:
			return TKeyCode::ENumpad_0;
		case GLFW_KEY_KP_1:
			return TKeyCode::ENumpad_1;
		case GLFW_KEY_KP_2:
			return TKeyCode::ENumpad_2;
		case GLFW_KEY_KP_3:
			return TKeyCode::ENumpad_3;
		case GLFW_KEY_KP_4:
			return TKeyCode::ENumpad_4;
		case GLFW_KEY_KP_5:
			return TKeyCode::ENumpad_5;
		case GLFW_KEY_KP_6:
			return TKeyCode::ENumpad_6;
		case GLFW_KEY_KP_7:
			return TKeyCode::ENumpad_7;
		case GLFW_KEY_KP_8:
			return TKeyCode::ENumpad_8;
		case GLFW_KEY_KP_9:
			return TKeyCode::ENumpad_9;
		case GLFW_KEY_KP_DECIMAL:
			return TKeyCode::ENumpad_Decimal;
		case GLFW_KEY_KP_DIVIDE:
			return TKeyCode::ENumpad_Divide;
		case GLFW_KEY_KP_MULTIPLY:
			return TKeyCode::ENumpad_Multiply;
		case GLFW_KEY_KP_SUBTRACT:
			return TKeyCode::ENumpad_Subtract;
		case GLFW_KEY_KP_ADD:
			return TKeyCode::ENumpad_Add;
		case GLFW_KEY_KP_ENTER:
			return TKeyCode::ENumpad_Enter;
		case GLFW_KEY_KP_EQUAL:
			return TKeyCode::ENumpad_Equal;
		case GLFW_KEY_LEFT_SHIFT:
			return TKeyCode::ELeftShift;
		case GLFW_KEY_LEFT_CONTROL:
			return TKeyCode::ELeftControl;
		case GLFW_KEY_LEFT_ALT:
			return TKeyCode::ELeftAlt;
		case GLFW_KEY_LEFT_SUPER:
			return TKeyCode::ELeftSuper;
		case GLFW_KEY_RIGHT_SHIFT:
			return TKeyCode::ERightShift;
		case GLFW_KEY_RIGHT_CONTROL:
			return TKeyCode::ERightControl;
		case GLFW_KEY_RIGHT_ALT:
			return TKeyCode::ERightAlt;
		case GLFW_KEY_RIGHT_SUPER:
			return TKeyCode::ERightSuper;
		case GLFW_KEY_MENU:
			return TKeyCode::EMenu;
	}
	return TKeyCode::EUnknown;
}

TMouseButton ConvertGlfwMouseToTMouseButton(TInt32 Button)
{
	switch (Button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			return TMouseButton::ELeft;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return TMouseButton::ERight;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return TMouseButton::EMiddle;
	}

	return TMouseButton::EUnknown;
}

TInputAction ConvertGlfwInputActionToTInputAction(TInt32 Action)
{
	switch (Action)
	{
		case GLFW_PRESS:
			return TInputAction::EPress;
		case GLFW_RELEASE:
			return TInputAction::ERelease;
		case GLFW_REPEAT:
			return TInputAction::ERepeat;
	}

	return TInputAction::EUnknown;
}

TModifierKey ConvertGlfwModifierKeyToTModifierKey(TInt32 Mod)
{
	switch (Mod)
	{
		case GLFW_MOD_SHIFT:
			return TModifierKey::EShift;
		case GLFW_MOD_CONTROL:
			return TModifierKey::EControl;
		case GLFW_MOD_ALT:
			return TModifierKey::EAlt;
		case GLFW_MOD_SUPER:
			return TModifierKey::ESuper;
		case GLFW_MOD_CAPS_LOCK:
			return TModifierKey::ECapsLock;
		case GLFW_MOD_NUM_LOCK:
			return TModifierKey::ENumLock;
	}

	return TModifierKey::EUnknown;
}

static TInputManager* InputManagerInstance = nullptr;

void WindowPositionCallback(GLFWwindow* GLFWWindow, TInt32 XPosition, TInt32 YPosition)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (!Window || Window->WindowProperties.Fullscreen)
	{
		return;
	}

	Window->WindowProperties.XPosition = XPosition;
	Window->WindowProperties.YPosition = YPosition;

	if (Window->OnWindowMove)
	{
		Window->OnWindowMove(Window, XPosition, YPosition);
	}
}

void WindowSizeCallback(GLFWwindow* GLFWWindow, TInt32 Width, TInt32 Height)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		if (Window->WindowProperties.Fullscreen)
		{
			Window->FullscreenWidth = Width;
			Window->FullscreenHeight = Height;
		}
		else
		{
			Window->WindowProperties.Width = Width;
			Window->WindowProperties.Height = Height;
		}
	}

	if (Window->OnWindowResize)
	{
		Window->OnWindowResize(Window, Width, Height);
	}
}

void WindowCloseCallback(GLFWwindow* GLFWWindow)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		Window->Closed = true;
	}

	if (Window->OnWindowClose)
	{
		Window->OnWindowClose(Window);
	}
}

void WindowFocusCallback(GLFWwindow* GLFWWindow, TInt32 Focused)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		Window->Focused = Focused;
	}

	if (Window->OnWindowFocus)
	{
		Window->OnWindowFocus(Window, Focused);
	}
}

void WindowIconifyCallback(GLFWwindow* GLFWWindow, TInt32 Iconified)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		Window->Iconified = Iconified;
	}

	if (Window->OnWindowIconify)
	{
		Window->OnWindowIconify(Window, Iconified);
	}
}

void FramebufferSizeCallback(GLFWwindow* GLFWWindow, TInt32 Width, TInt32 Height)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window->OnFramebufferSize)
	{
		Window->OnFramebufferSize(Window, Width, Height);
	}
}

void CursorEnterCallback(GLFWwindow* GLFWWindow, TInt32 Entered)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));
	if (Window->OnCursorEnter)
	{
		Window->OnCursorEnter(Window, Entered);
	}
}

void DropCallback(GLFWwindow* GLFWWindow, TInt32 Count, const TChar** Paths)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));
	if (Window->OnDrop)
	{
		Window->OnDrop(Window, Count, Paths);
	}
}

void KeyCallback(GLFWwindow* GLFWWindow, TInt32 Key, TInt32 Scancode, TInt32 Action, TInt32 Mods)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	TKeyCode KeyCode = ConvertGlfwKeyToTKeyCode(Key);
	switch (Action)
	{
		case GLFW_PRESS:
			InputManagerInstance->SetKeyState(KeyCode, TInputAction::EPress);
			break;
		case GLFW_REPEAT:
			InputManagerInstance->SetKeyState(KeyCode, TInputAction::ERepeat);
			break;
		case GLFW_RELEASE:
			InputManagerInstance->SetKeyState(KeyCode, TInputAction::ERelease);
			break;
		default:
			break;
	}

	if (Window->OnKey)
	{
		Window->OnKey(Window, KeyCode, Scancode, ConvertGlfwInputActionToTInputAction(Action), ConvertGlfwModifierKeyToTModifierKey(Mods));
	}
}

void CharCallback(GLFWwindow* GLFWWindow, TUInt32 Codepoint)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window->OnChar)
	{
		Window->OnChar(Window, static_cast<TChar>(Codepoint));
	}
}

void MouseButtonCallback(GLFWwindow* GLFWWindow, TInt32 Button, TInt32 Action, TInt32 Mods)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	TMouseButton MouseButton = ConvertGlfwMouseToTMouseButton(Button);

	switch (Action)
	{
		case GLFW_PRESS:
			InputManagerInstance->SetMouseState(MouseButton, TInputAction::EPress);
			break;
		case GLFW_RELEASE:
			InputManagerInstance->SetMouseState(MouseButton, TInputAction::ERelease);
			break;
		default:
			break;
	}

	if (Window->OnMouseButton)
	{
		Window->OnMouseButton(Window, MouseButton, ConvertGlfwInputActionToTInputAction(Action), ConvertGlfwModifierKeyToTModifierKey(Mods));
	}
}

void CursorPositionCallback(GLFWwindow* GLFWWindow, TDouble XPosition, TDouble YPosition)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		Window->MouseXPosition = XPosition;
		Window->MouseYPosition = YPosition;

		if (Window->Focused)
		{
			InputManagerInstance->SetCurrentMousePosition((TFloat)XPosition, (TFloat)YPosition);
		}
	}

	if (Window->OnCursorPosition)
	{
		Window->OnCursorPosition(Window, XPosition, YPosition);
	}
}

void ScrollCallback(GLFWwindow* GLFWWindow, TDouble XOffset, TDouble YOffset)
{
	TWindow* Window = static_cast<TWindow*>(glfwGetWindowUserPointer(GLFWWindow));

	if (Window)
	{
		if (Window->Focused)
		{
			InputManagerInstance->SetScrollDelta((TFloat)XOffset, (TFloat)YOffset);
		}
	}

	if (Window->OnScroll)
	{
		Window->OnScroll(Window, XOffset, YOffset);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

static TInt32 GlfwWindowCount = 0;

TWindow::TWindow(const TWindowProperties& WindowProperties, TBool ForceInit)
	: WindowProperties(WindowProperties),
	  Initialized(false),
	  Focused(false),
	  Iconified(false),
	  Closed(true),
	  CursorHidden(false),
	  Opacity(1.0f),
	  OnWindowMove(nullptr),
	  OnWindowResize(nullptr),
	  OnWindowClose(nullptr),
	  OnWindowFocus(nullptr),
	  OnWindowIconify(nullptr),
	  OnFramebufferSize(nullptr),
	  OnCursorEnter(nullptr),
	  OnDrop(nullptr),
	  OnKey(nullptr),
	  OnChar(nullptr),
	  OnMouseButton(nullptr),
	  OnCursorPosition(nullptr),
	  OnScroll(nullptr)
{
	if (ForceInit)
	{
		if (!this->Initialize())
		{
			TLog::Error<TRNT_GET_LOG_INFO(Window)>("Failed to initialize TWindow!");
		}
	}
}

TWindow::~TWindow()
{
	if (Initialized)
	{
		Destroy();
	}
}

TBool TWindow::Initialize()
{
	if (GlfwWindowCount < 1)
	{
		if (glfwInit() != GLFW_TRUE)
		{
			TLog::Error<TRNT_GET_LOG_INFO(Window)>("Could not initialize Glfw library!");

			return false;
		}
	}

	if (!InputManagerInstance)
	{
		InputManagerInstance = TInputManager::GetInstance();
	}

	if (InputManagerInstance == nullptr)
	{
		TLog::Error<TRNT_GET_LOG_INFO(Window)>("TInputManager is not initialized! Please initialize `TInputManager` before creating the window.");

		return false;
	}

	if (WindowProperties.Fullscreen)
	{
		GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);

		if (VideoMode == nullptr)
		{
			TLog::Error<TRNT_GET_LOG_INFO(Window)>("Failed to get video mode of the specified monitor!");
			return false;
		}

		FullscreenWidth = VideoMode->width;
		FullscreenHeight = VideoMode->height;

		WindowHandle = glfwCreateWindow(FullscreenWidth, FullscreenHeight, WindowProperties.Title.GetData(), Monitor, nullptr);
	}
	else
	{
		WindowHandle = glfwCreateWindow(WindowProperties.Width, WindowProperties.Height, WindowProperties.Title.GetData(), nullptr, nullptr);
	}

	if (WindowHandle == nullptr)
	{
		TLog::Error<TRNT_GET_LOG_INFO(Window)>("Failed to create GLFW Window!");
		return false;
	}

	if (WindowProperties.Fullscreen)
	{
		glfwSetWindowPos(WindowHandle, 0, 0);
	}
	else
	{
		glfwSetWindowPos(WindowHandle, WindowProperties.XPosition, WindowProperties.YPosition);
	}

	glfwSetWindowUserPointer(WindowHandle, this);

	glfwSetWindowAttrib(WindowHandle, GLFW_RESIZABLE, WindowProperties.Resizable);
	glfwSetWindowAttrib(WindowHandle, GLFW_FLOATING, WindowProperties.Floating);
	glfwSetWindowAttrib(WindowHandle, GLFW_DECORATED, !WindowProperties.Borderless);
	glfwSetWindowAttrib(WindowHandle, GLFW_FOCUSED, GLFW_TRUE);

	glfwSetWindowOpacity(WindowHandle, Opacity);

	glfwSetWindowPosCallback(WindowHandle, WindowPositionCallback);
	glfwSetWindowSizeCallback(WindowHandle, WindowSizeCallback);
	glfwSetWindowCloseCallback(WindowHandle, WindowCloseCallback);
	glfwSetWindowFocusCallback(WindowHandle, WindowFocusCallback);
	glfwSetWindowIconifyCallback(WindowHandle, WindowIconifyCallback);
	glfwSetFramebufferSizeCallback(WindowHandle, FramebufferSizeCallback);
	glfwSetCursorEnterCallback(WindowHandle, CursorEnterCallback);
	glfwSetDropCallback(WindowHandle, DropCallback);
	glfwSetKeyCallback(WindowHandle, KeyCallback);
	glfwSetCharCallback(WindowHandle, CharCallback);
	glfwSetCursorPosCallback(WindowHandle, CursorPositionCallback);
	glfwSetMouseButtonCallback(WindowHandle, MouseButtonCallback);
	glfwSetScrollCallback(WindowHandle, ScrollCallback);

	glfwShowWindow(WindowHandle);

	Focused = true;
	Initialized = true;
	Closed = false;

	++GlfwWindowCount;

	TLog::Success<TRNT_GET_LOG_INFO(Window)>("Create GLFW Window successfully! Window count: {}", GlfwWindowCount);

	return true;
}

void TWindow::Destroy()
{
	if (Initialized)
	{
		glfwDestroyWindow(WindowHandle);
		WindowHandle = nullptr;
		--GlfwWindowCount;

		TLog::Info<TRNT_GET_LOG_INFO(Window)>("Deleted GLFW Window! Window cound: {}", GlfwWindowCount);
	}

	Initialized = false;
	Closed = true;

	if (GlfwWindowCount < 1)
	{
		glfwTerminate();
	}
}

void TWindow::Maximize()
{
	glfwMaximizeWindow(WindowHandle);
}

void TWindow::Minimize()
{
	glfwIconifyWindow(WindowHandle);
}

void TWindow::Iconify()
{
	glfwIconifyWindow(WindowHandle);
}

void TWindow::Focus()
{
	glfwFocusWindow(WindowHandle);
	Focused = true;
}

void TWindow::Close()
{
	glfwSetWindowShouldClose(WindowHandle, true);
}

void TWindow::SetIcon(const TChar* IconFilePath)
{
	GLFWimage Icon[1];
	Icon[0].pixels = stbi_load(IconFilePath, &Icon[0].width, &Icon[0].height, 0, 4);
	glfwSetWindowIcon(WindowHandle, 1, Icon);
	stbi_image_free(Icon[0].pixels);
}

void TWindow::SetTitle(const TString& Title)
{
	WindowProperties.Title = Title;
	glfwSetWindowTitle(WindowHandle, Title.GetData());
}

void TWindow::SetWidth(TInt32 Width)
{
	glfwSetWindowSize(WindowHandle, Width, WindowProperties.Height);
}

void TWindow::SetHeight(TInt32 Height)
{
	glfwSetWindowSize(WindowHandle, WindowProperties.Width, Height);
}

void TWindow::SetSize(TInt32 Width, TInt32 Height)
{
	glfwSetWindowSize(WindowHandle, Width, Height);
}

void TWindow::SetXPosition(TInt32 XPosition)
{
	glfwSetWindowPos(WindowHandle, XPosition, WindowProperties.YPosition);
}

void TWindow::SetYPosition(TInt32 YPosition)
{
	glfwSetWindowPos(WindowHandle, WindowProperties.XPosition, YPosition);
}

void TWindow::SetPosition(TInt32 XPosition, TInt32 YPosition)
{
	glfwSetWindowPos(WindowHandle, XPosition, YPosition);
}

void TWindow::SetResizable(TBool Resizable)
{
	WindowProperties.Resizable = Resizable;
	glfwSetWindowAttrib(WindowHandle, GLFW_RESIZABLE, Resizable);
}

void TWindow::SetBorderless(TBool Borderless)
{
	WindowProperties.Borderless = Borderless;
	glfwSetWindowAttrib(WindowHandle, GLFW_DECORATED, !Borderless);
}

void TWindow::SetFloating(TBool Floating)
{
	WindowProperties.Floating = Floating;
	glfwSetWindowAttrib(WindowHandle, GLFW_FLOATING, Floating);
}

void TWindow::SetFullscreen(TBool Fullscreen)
{
	WindowProperties.Fullscreen = Fullscreen;

	if (WindowProperties.Fullscreen)
	{
		GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);
		glfwSetWindowMonitor(WindowHandle, Monitor, 0, 0, VideoMode->width, VideoMode->height, GLFW_DONT_CARE);
	}
	else
	{
		glfwSetWindowMonitor(WindowHandle, nullptr, WindowProperties.XPosition, WindowProperties.YPosition, WindowProperties.Width, WindowProperties.Height, GLFW_DONT_CARE);
	}
}

const TChar* TWindow::GetClipboardString() const
{
	return glfwGetClipboardString(WindowHandle);
}

void TWindow::SetClipboard(const TChar* String)
{
	glfwSetClipboardString(WindowHandle, String);
}

void TWindow::SetCursorHide(TBool Hide)
{
	if (CursorHidden = Hide)
	{
		glfwSetInputMode(WindowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		glfwSetInputMode(WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (Focused)
	{
		InputManagerInstance->SetCursorIsHidden(Hide);
	}
}

void TWindow::ProcessInput()
{
	glfwPollEvents();
}

void TWindow::WaitEvents()
{
	glfwWaitEvents();
}

void TWindow::SetOpacity(TFloat Opacity)
{
	this->Opacity = Opacity;
	glfwSetWindowOpacity(WindowHandle, Opacity);
}

#endif

#pragma warning(pop)