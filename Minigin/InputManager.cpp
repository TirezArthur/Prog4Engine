#include "InputManager.h"
#include "GamePad.h"
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

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	std::for_each(m_KeyboardMappings.begin(), m_KeyboardMappings.end(), [this, keys](const auto& mapping) {
		if (keys[mapping.first] != 0) this->ExecuteMapping(mapping.second, TriggerType::held, -1);
		});

	for (auto& gamePad : m_Gamepads) {
		const auto buttons = gamePad->GetButtonState();
		std::for_each(m_GamePadMappings.begin(), m_GamePadMappings.end(), [this, buttons, &gamePad](const auto& mapping) {
			if (buttons[mapping.first] != 0) this->ExecuteMapping(mapping.second, TriggerType::held, gamePad->GetDeviceId());
			});
	}

	return true;
}

Command* InputManager::AddBinding(SDL_Scancode key, TriggerType trigger, std::unique_ptr<Command> command)
{
	auto it = m_KeyboardMappings.emplace(std::make_pair(key, KeyMapping{ trigger, -1, std::move(command) }));
	return (*it).second.command.get();
}

Command* InputManager::AddBinding(GamepadButton key, TriggerType trigger, std::unique_ptr<Command> command, int8_t deviceId)
{
	auto it = m_GamePadMappings.emplace(std::make_pair(key, KeyMapping{ trigger, deviceId, std::move(command) }));
	return (*it).second.command.get();
}

void InputManager::RemoveBindings(SDL_Scancode key)
{
	m_KeyboardMappings.erase(m_KeyboardMappings.lower_bound(key), m_KeyboardMappings.upper_bound(key));
}

void InputManager::RemoveBindings(GamepadButton key)
{
	m_GamePadMappings.erase(m_GamePadMappings.lower_bound(key), m_GamePadMappings.upper_bound(key));
}

void InputManager::RemoveBinding(Command* command)
{
	//TODO Fix this, I cannot currently figure out why this gives an error about the end iterator not being able to be dereferenced, as far as I can tell the implementation doesn't try to do that
	std::erase_if(m_KeyboardMappings, [command](auto& mapping)-> bool {
		return (mapping.second.command.get() == command);
		});
	std::erase_if(m_GamePadMappings, [command](auto& mapping)-> bool {
		return (mapping.second.command.get() == command);
		});
}

void InputManager::ExecuteMapping(const KeyMapping& mapping, TriggerType trigger, int8_t deviceId)
{
	if (mapping.trigger != trigger) return;
	if (mapping.deviceId != deviceId) return;
	mapping.command->Execute();
}

