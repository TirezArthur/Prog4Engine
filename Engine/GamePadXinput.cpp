#define WIN32_LEAN_AND_MEAN
#include "GamePad.h"
#include <Windows.h>
#include <Xinput.h>

using namespace dae;

class GamePad::Impl {
public:
	explicit Impl(int8_t deviceId) : m_DeviceId{ deviceId } {};
	bool UpdateState(TriggerType& trigger, GamepadButton& button);
	int8_t GetDeviceId() const { return m_DeviceId; }
	std::array<bool, 16> GetButtonState() const;
private:
	GamepadButton StateToButton(WORD state) const;
	int8_t m_DeviceId{};
	std::bitset<16> m_PrevState{};
};

GamepadButton GamePad::Impl::StateToButton(WORD state) const
{
	switch (state)
	{
	case XINPUT_GAMEPAD_DPAD_UP: return GamepadButton::Gamepad_UP;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN: return GamepadButton::Gamepad_DOWN;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT: return GamepadButton::Gamepad_LEFT;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT: return GamepadButton::Gamepad_RIGHT;
		break;
	case XINPUT_GAMEPAD_START: return GamepadButton::Gamepad_Start;
		break;
	case XINPUT_GAMEPAD_BACK: return GamepadButton::Gamepad_Back;
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB: return GamepadButton::Gamepad_Left_Thumb;
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB: return GamepadButton::Gamepad_Right_Thumb;
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER: return GamepadButton::Gamepad_Left_Shoulder;
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER: return GamepadButton::Gamepad_Right_Shoulder;
		break;
	case XINPUT_GAMEPAD_A: return GamepadButton::Gamepad_A;
		break;
	case XINPUT_GAMEPAD_B: return GamepadButton::Gamepad_B;
		break;
	case XINPUT_GAMEPAD_X: return GamepadButton::Gamepad_X;
		break;
	case XINPUT_GAMEPAD_Y: return GamepadButton::Gamepad_Y;
		break;
	default: return GamepadButton::Invalid;
		break;
	}
}

bool GamePad::Impl::UpdateState(TriggerType & trigger, GamepadButton & button)
{
	XINPUT_STATE state{};
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(m_DeviceId, &state);
	if (result != ERROR_SUCCESS) return false;
	if (state.Gamepad.wButtons == m_PrevState) return false;

	std::bitset<16> newState = state.Gamepad.wButtons;

	for (int buttonIndex{}; buttonIndex < 16; ++buttonIndex) {
		if (newState[buttonIndex] != m_PrevState[buttonIndex]) {
			button = StateToButton(0x1 << buttonIndex);
			trigger = (newState[buttonIndex] == 0) ? TriggerType::up : TriggerType::down;
			m_PrevState[buttonIndex] = newState[buttonIndex];
			return true;
		}
	}

	return false;
}

std::array<bool, 16> GamePad::Impl::GetButtonState() const
{
	std::array<bool, 16> buttons{};

	for (int buttonIndex{}; buttonIndex < 16; ++buttonIndex) {
		if (m_PrevState[buttonIndex] == 1) {
			GamepadButton button = StateToButton(0x1 << buttonIndex);
			buttons[button] = true;
		}
	}

	return buttons;
}

GamePad::GamePad(int8_t deviceId) :
	m_Impl{ std::make_unique<Impl>(deviceId) }
{
}

GamePad::~GamePad() = default;

bool GamePad::UpdateState(TriggerType& trigger, GamepadButton& button)
{
	return m_Impl->UpdateState(trigger, button);
}

int8_t GamePad::GetDeviceId() const
{
	return m_Impl->GetDeviceId();
}

std::array<bool, 16> GamePad::GetButtonState() const
{
	return m_Impl->GetButtonState();
}
