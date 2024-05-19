#pragma once

#include "Trinity/Core/Config.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"

#include "Trinity/Core/Types/Pair.h"

#include "KeyCodes.h"
#include "MouseButtons.h"
#include "InputAction.h"

class TRNT_API TInputManager
{
	class TData
	{
	public:
		using PositionType = TPair<TFloat, TFloat>;

		static constexpr TInt32 KeyCodeCount = static_cast<TInt32>(TKeyCode::EMax);
		static constexpr TInt32 MouseButtonCount = static_cast<TInt32>(TMouseButton::EMax);

		TInputAction KeyState[KeyCodeCount];
		TInputAction MouseState[MouseButtonCount];

		PositionType CurrentMousePosition;
		PositionType LastMousePosition;

		PositionType ScrollDelta{ 0.0f, 0.0f };

		TBool CursorIsHidden = false;
	};

public:
	TRNT_FORCE_INLINE TInputAction GetKeyState(TKeyCode KeyCode)
	{
		return InputManagerData.KeyState[static_cast<TInt32>(KeyCode)];
	}

	TRNT_FORCE_INLINE TInputAction GetMouseState(TMouseButton MouseButton)
	{
		return InputManagerData.MouseState[static_cast<TInt32>(MouseButton)];
	}

	TRNT_FORCE_INLINE TBool IsKeyPressed(TKeyCode KeyCode)
	{
		return InputManagerData.KeyState[static_cast<TInt32>(KeyCode)] == TInputAction::EPress;
	}

	TRNT_FORCE_INLINE TBool IsKeyHeld(TKeyCode KeyCode)
	{
		return InputManagerData.KeyState[static_cast<TInt32>(KeyCode)] == TInputAction::ERepeat;
	}

	TRNT_FORCE_INLINE TBool IsKeyReleased(TKeyCode KeyCode)
	{
		return InputManagerData.KeyState[static_cast<TInt32>(KeyCode)] == TInputAction::ERelease;
	}

	TRNT_FORCE_INLINE TBool IsMouseButtonClicked(TMouseButton MouseButton)
	{
		return InputManagerData.MouseState[static_cast<TInt32>(MouseButton)] == TInputAction::EPress;
	}

	TRNT_FORCE_INLINE TBool IsMouseButtonHeld(TMouseButton MouseButton)
	{
		return InputManagerData.MouseState[static_cast<TInt32>(MouseButton)] == TInputAction::ERepeat;
	}

	TRNT_FORCE_INLINE TBool IsMouseButtonReleased(TMouseButton MouseButton)
	{
		return InputManagerData.MouseState[static_cast<TInt32>(MouseButton)] == TInputAction::ERelease;
	}

	void Reset();

	TRNT_FORCE_INLINE void SetKeyState(TKeyCode KeyCode, TInputAction State)
	{
		InputManagerData.KeyState[static_cast<TInt32>(KeyCode)] = State;
	}

	TRNT_FORCE_INLINE void SetMouseState(TMouseButton MouseButton, TInputAction State)
	{
		InputManagerData.MouseState[static_cast<TInt32>(MouseButton)] = State;
	}

public:
	void SetCurrentMousePosition(TFloat MousePosX, TFloat MousePosY);

	TRNT_FORCE_INLINE const TPair<TFloat, TFloat>& GetCurrentMousePosition()
	{
		return InputManagerData.CurrentMousePosition;
	}

	TRNT_FORCE_INLINE const TPair<TFloat, TFloat>& GetLastMousePosition()
	{
		return InputManagerData.LastMousePosition;
	}

	TRNT_FORCE_INLINE const TPair<TFloat, TFloat>& GetScrollDelta()
	{
		return InputManagerData.ScrollDelta;
	}

	TRNT_FORCE_INLINE void SetScrollDelta(TFloat XDelta, TFloat YDelta)
	{
		InputManagerData.ScrollDelta = { XDelta, YDelta };
	}

	TRNT_FORCE_INLINE TBool CursorIsHidden()
	{
		return InputManagerData.CursorIsHidden;
	}

	TRNT_FORCE_INLINE void SetCursorIsHidden(TBool Hidden)
	{
		InputManagerData.CursorIsHidden = Hidden;
	}

public:
	static TRNT_FORCE_INLINE void Initialize()
	{
		if (!Instance)
		{
			Instance = new TInputManager();
			Instance->Reset();
		}
	}

	static TRNT_FORCE_INLINE TInputManager* GetInstance()
	{
		return Instance;
	}

	static TRNT_FORCE_INLINE void DeleteInstance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static TInputManager* Instance;

	TData InputManagerData;
};