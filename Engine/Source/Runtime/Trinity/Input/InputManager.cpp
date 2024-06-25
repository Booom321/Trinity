#include "TrinityPCH.h"
#include "InputManager.h"

#include <cstring>

void TInputManager::Reset()
{
	memset(InputManagerData.KeyState, static_cast<TInt32>(TInputAction::ERelease), TData::KeyCodeCount * sizeof(TInputAction));
	memset(InputManagerData.MouseState, static_cast<TInt32>(TInputAction::ERelease), TData::MouseButtonCount * sizeof(TInputAction));

	InputManagerData.ScrollDelta = { 0.0f, 0.0f };

	InputManagerData.CursorIsHidden = false;
}

void TInputManager::SetCurrentMousePosition(TFloat MousePosX, TFloat MousePosY)
{
	InputManagerData.LastMousePosition.First = InputManagerData.CurrentMousePosition.First;
	InputManagerData.LastMousePosition.Second = InputManagerData.CurrentMousePosition.Second;

	InputManagerData.CurrentMousePosition.First = MousePosX;
	InputManagerData.CurrentMousePosition.Second = MousePosY;
}