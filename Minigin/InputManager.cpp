#include "InputManager.h"
#include "GamePadXinput.h"
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <algorithm>

using namespace dae;

InputManager::InputManager()
{
	m_Gamepads.reserve(MAX_GAMEPADS);
	for (int8_t deviceId{}; deviceId < MAX_GAMEPADS; ++deviceId)
	{
		m_Gamepads.emplace_back(std::make_unique<GamePad>(deviceId));
	}
}

InputManager::~InputManager() = default;

bool InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;
		case SDL_KEYDOWN:
			if (e.key.repeat != 0) break;
			std::for_each(m_KeyboardMappings.lower_bound(e.key.keysym.scancode), m_KeyboardMappings.upper_bound(e.key.keysym.scancode), [this](const auto& mapping) {
				this->ExecuteMapping(mapping.second, TriggerType::down, -1);
				});
			break;
		case SDL_KEYUP:
			std::for_each(m_KeyboardMappings.lower_bound(e.key.keysym.scancode), m_KeyboardMappings.upper_bound(e.key.keysym.scancode), [this](const auto& mapping) {
				this->ExecuteMapping(mapping.second, TriggerType::up, -1);
				});
			break;
		case SDL_MOUSEBUTTONDOWN:
			//Process mouse down
			break;
		case SDL_MOUSEBUTTONUP:
			//Process mouse up
			break;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	TriggerType trigger{};
	GamepadButton button{};
	for (auto& gamePad : m_Gamepads) {
		while (gamePad->UpdateState(trigger, button)) {
			std::for_each(m_GamePadMappings.lower_bound(button), m_GamePadMappings.upper_bound(button), [this, trigger, &gamePad](const auto& mapping) {
				this->ExecuteMapping(mapping.second, trigger, gamePad->GetDeviceId());
				});
		}
	}

	//const Uint8* keys = SDL_GetKeyboardState(nullptr);

	return true;
}

void InputManager::AddBinding(SDL_Scancode key, TriggerType trigger, std::unique_ptr<Command> command)
{
	m_KeyboardMappings.emplace(std::make_pair(key, KeyMapping{ trigger, -1, std::move(command) }));
}

void InputManager::AddBinding(GamepadButton key, TriggerType trigger, std::unique_ptr<Command> command, int8_t deviceId)
{
	m_GamePadMappings.emplace(std::make_pair(key, KeyMapping{ trigger, deviceId, std::move(command) }));
}

void InputManager::ExecuteMapping(const KeyMapping& mapping, TriggerType trigger, int8_t deviceId)
{
	if (mapping.trigger != trigger) return;
	if (mapping.deviceId != deviceId) return;
	mapping.command->Execute();
}

