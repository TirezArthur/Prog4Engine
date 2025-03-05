#pragma once
#include "Singleton.h"
#include "SDL_keycode.h"
#include "Command.h"
#include <map>
#include <memory>
#include <variant>

namespace dae
{
	enum class GamepadButton : uint16_t {
		Gamepad_UP = 0x0001,
		Gamepad_DOWN = 0x0002,
		Gamepad_LEFT = 0x0004,
		Gamepad_RIGHT = 0x0008,
		Gamepad_A = 0x1000,
		Gamepad_B = 0x2000,
		Gamepad_X = 0x4000,
		Gamepad_Y = 0x8000,
	};

	enum class TriggerState : uint8_t {
		down,
		up,
		held
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddBinding(SDL_Scancode key, TriggerState trigger, std::unique_ptr<Command> command);
		void AddBinding(GamepadButton key, TriggerState trigger, std::unique_ptr<Command> command, int8_t deviceId = 0);
	private:
		struct KeyMapping {
			TriggerState trigger;
			int8_t deviceId;
			std::unique_ptr<Command> command;
		};

		static void ExecuteMapping(const KeyMapping& mapping, TriggerState trigger, int8_t deviceId = 0);

		std::multimap<GamepadButton, KeyMapping> m_GamePadMappings;
		std::multimap<SDL_Scancode, KeyMapping> m_KeyboardMappings;
	};
}
