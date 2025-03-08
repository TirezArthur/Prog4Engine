#pragma once
#define WIN32_LEAN_AND_MEAN
#include "InputManager.h"
#include <Windows.h>
#include <Xinput.h>
#include <bitset>
#include <array>

using namespace dae;

#define MAX_GAMEPADS XUSER_MAX_COUNT

class InputManager::GamePad
{
public:
	explicit GamePad(int8_t deviceId = 0) : m_DeviceId{ deviceId } {};
	bool UpdateState(TriggerType& trigger, GamepadButton& button);
	int8_t GetDeviceId() const { return m_DeviceId; }
	std::array<bool, sizeof(WORD) * 8> GetButtonState() const;
private:
	GamepadButton StateToButton(WORD state) const;
	int8_t m_DeviceId{};
	std::bitset<sizeof(WORD) * 8> m_PrevState{};
};

bool InputManager::GamePad::UpdateState(TriggerType& trigger, GamepadButton& button)
{
	XINPUT_STATE state{};
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(m_DeviceId, &state);
	if (result != ERROR_SUCCESS) return false;
	if (state.Gamepad.wButtons == m_PrevState) return false;

	std::bitset<sizeof(WORD) * 8> newState = state.Gamepad.wButtons;

	for (int buttonIndex{}; buttonIndex < sizeof(WORD) * 8; ++buttonIndex) {
		if (newState[buttonIndex] != m_PrevState[buttonIndex]) {
			button = StateToButton(0x1 << buttonIndex);
			trigger = (newState[buttonIndex] == 0) ? TriggerType::up : TriggerType::down;
			m_PrevState[buttonIndex] = newState[buttonIndex];
			return true;
		}
	}

	return false;
}

std::array<bool, sizeof(WORD) * 8> InputManager::GamePad::GetButtonState() const
{
	std::array<bool, sizeof(WORD) * 8> buttons{};

	for (int buttonIndex{}; buttonIndex < sizeof(WORD) * 8; ++buttonIndex) {
		if (m_PrevState[buttonIndex] == 1) {
			GamepadButton button = StateToButton(0x1 << buttonIndex);
			buttons[button] = true;
		}
	}

	return buttons;
}

GamepadButton InputManager::GamePad::StateToButton(WORD state) const
{
	GamepadButton button{ GamepadButton::Invalid };
	switch (state)
	{
	case XINPUT_GAMEPAD_DPAD_UP: button = GamepadButton::Gamepad_UP;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN: button = GamepadButton::Gamepad_DOWN;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT: button = GamepadButton::Gamepad_LEFT;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT: button = GamepadButton::Gamepad_RIGHT;
		break;
	case XINPUT_GAMEPAD_A: button = GamepadButton::Gamepad_A;
		break;
	case XINPUT_GAMEPAD_B: button = GamepadButton::Gamepad_B;
		break;
	case XINPUT_GAMEPAD_X: button = GamepadButton::Gamepad_X;
		break;
	case XINPUT_GAMEPAD_Y: button = GamepadButton::Gamepad_Y;
		break;
	}
	return button;
}
