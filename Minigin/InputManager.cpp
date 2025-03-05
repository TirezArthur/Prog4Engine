#include "InputManager.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <algorithm>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;
		case SDL_KEYDOWN:
			//Process down
			if (e.key.repeat != 0) break;
			std::for_each(m_KeyboardMappings.lower_bound(e.key.keysym.scancode), m_KeyboardMappings.upper_bound(e.key.keysym.scancode), [this](const decltype(m_KeyboardMappings)::value_type& mapping) {
				this->ExecuteMapping(mapping.second, TriggerState::down, -1);
				});
			break;
		case SDL_KEYUP:
			//Process up
			std::for_each(m_KeyboardMappings.lower_bound(e.key.keysym.scancode), m_KeyboardMappings.upper_bound(e.key.keysym.scancode), [this](const decltype(m_KeyboardMappings)::value_type& mapping) {
				this->ExecuteMapping(mapping.second, TriggerState::up, -1);
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

	//const Uint8* keys = SDL_GetKeyboardState(nullptr);

	return true;
}

void dae::InputManager::AddBinding(SDL_Scancode key, TriggerState trigger, std::unique_ptr<Command> command)
{
	m_KeyboardMappings.emplace(std::make_pair(key, KeyMapping{ trigger, -1, std::move(command) }));
}

void dae::InputManager::AddBinding(GamepadButton key, TriggerState trigger, std::unique_ptr<Command> command, int8_t deviceId)
{
	m_GamePadMappings.emplace(std::make_pair(key, KeyMapping{ trigger, deviceId, std::move(command) }));
}

void dae::InputManager::ExecuteMapping(const KeyMapping& mapping, TriggerState trigger, int8_t deviceId)
{
	if (mapping.trigger != trigger) return;
	if (mapping.deviceId != deviceId) return;
	mapping.command->Execute();
}
