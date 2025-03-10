#pragma once
#include "Singleton.h"
#include "SDL_keycode.h"
#include "Command.h"
#include <map>
#include <memory>
#include <vector>

namespace dae
{
	enum GamepadButton : uint8_t {
		Invalid,
		Gamepad_UP,
		Gamepad_DOWN,
		Gamepad_LEFT,
		Gamepad_RIGHT,
		Gamepad_A,
		Gamepad_B,
		Gamepad_X,
		Gamepad_Y,
	};

	enum class TriggerType : uint8_t {
		down,
		up,
		held
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		Command* AddBinding(SDL_Scancode key, TriggerType trigger, std::unique_ptr<Command> command);
		Command* AddBinding(GamepadButton key, TriggerType trigger, std::unique_ptr<Command> command, int8_t deviceId = 0);
		void RemoveBindings(SDL_Scancode key);
		void RemoveBindings(GamepadButton key);
		void RemoveBinding(Command* command);
	private:
		friend Singleton<InputManager>;
		explicit InputManager();
		~InputManager();

		struct KeyMapping {
			TriggerType trigger;
			int8_t deviceId;
			std::unique_ptr<Command> command;
		};

		static void ExecuteMapping(const KeyMapping& mapping, TriggerType trigger, int8_t deviceId = 0);

		//TODO maybe merge into one singular map using std::variant or similar. Still unsure if that's more clear or not
		std::multimap<GamepadButton, KeyMapping> m_GamePadMappings;
		std::multimap<SDL_Scancode, KeyMapping> m_KeyboardMappings;

		class GamePad;
		std::vector<std::unique_ptr<GamePad>> m_Gamepads;
	};
}
