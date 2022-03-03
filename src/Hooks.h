#pragma once

#include "PCH.h"
#include "SKSE/Trampoline.h"
#include <xbyak\xbyak.h>

namespace Hooks
{
	class MainUpdateHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<uintptr_t> hook{ REL::ID(35551) };

			auto& trampoline = SKSE::GetTrampoline();
			_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);  // SkyrimSE.exe+5AF4EF
		}

	private:
		static void Update(RE::Main* a_this, float a2);
		static inline REL::Relocation<decltype(Update)> _Update;
	};

	class PlayerCharacterHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> PlayerCharacter_BSTEventSink_BSAnimationGraphEventVtbl{ REL::ID(261918) };
			_ProcessEvent = PlayerCharacter_BSTEventSink_BSAnimationGraphEventVtbl.write_vfunc(0x1, ProcessEvent);
		}

	private:
		static void ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher);

		static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
	};

	class CharacterHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> Character_BSTEventSink_BSAnimationGraphEventVtbl{ REL::ID(261399) };
			_ProcessEvent = Character_BSTEventSink_BSAnimationGraphEventVtbl.write_vfunc(0x1, ProcessEvent);
		}

	private:
		static void ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher);

		static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
	};

	void apply_hooks();
}
