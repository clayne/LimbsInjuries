#include "Hooks.h"
#include "SkeletonHandler.h"
#include "Injuries.h"

namespace Hooks
{
	void MainUpdateHook::Update(RE::Main* a_this, float a2)
	{
		_Update(a_this, a2);

		DebugAPI::Update();

		draw_player_weapon(a2);
	}

	void Character_ProcessEvent(RE::BSAnimationGraphEvent* a_event)
	{
		if (a_event && a_event->holder) {
			std::string_view eventTag = a_event->tag.data();

			RE::Actor* a = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());
			if (a) {
				switch (hash(eventTag.data(), eventTag.size())) {
				case "weaponSwing"_h:
				case "weaponLeftSwing"_h:
				case "bashExit"_h:
					{
						Injuries::on_attack(a);
						break;
					}
				case "FootSprintLeft"_h:
				case "FootSprintRight"_h:
					{
						Injuries::on_sprint(a);
						break;
					}
				default:
					//logger::info("{}: {}", a_event->holder->GetName(), a_event->tag.data());
					break;
				}
			}
		}
	}

	void PlayerCharacterHook::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher)
	{
		Character_ProcessEvent(a_event);
		_ProcessEvent(a_this, a_event, a_dispatcher);
	}

	void CharacterHook::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher)
	{
		Character_ProcessEvent(a_event);
		_ProcessEvent(a_this, a_event, a_dispatcher);
	}

	template <size_t BRANCH_TYPE, uint64_t ID, size_t offset = 0, bool call = false>
	void add_trampoline(Xbyak::CodeGenerator* xbyakCode)
	{
		constexpr REL::ID funcOffset = REL::ID(ID);
		auto funcAddr = funcOffset.address();
		auto size = xbyakCode->getSize();
		auto& trampoline = SKSE::GetTrampoline();
		auto result = trampoline.allocate(size);
		std::memcpy(result, xbyakCode->getCode(), size);
		if constexpr (!call)
			trampoline.write_branch<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
		else
			trampoline.write_call<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
	}

	template <int ID, int OFFSET>
	void apply_call(std::uintptr_t func)
	{
		struct Code : Xbyak::CodeGenerator
		{
			Code(uintptr_t jump_addr)
			{
				mov(rax, jump_addr);
				jmp(rax);
			}
		} xbyakCode{ func };
		add_trampoline<5, ID, OFFSET, true>(&xbyakCode);
	}

	template <int ID>
	void apply_func(std::uintptr_t func)
	{
		struct Code : Xbyak::CodeGenerator
		{
			Code(uintptr_t jump_addr)
			{
				mov(rax, jump_addr);
				jmp(rax);
			}
		} xbyakCode{ func };
		add_trampoline<6, ID>(&xbyakCode);
	}

	void apply_LocationalDamage() {
		// SkyrimSE.exe+628DDC
		constexpr REL::ID smth_with_hitdata_melee_ID = REL::ID(37673);
		uintptr_t smth_with_hitdata_melee_ret = smth_with_hitdata_melee_ID.address() + 0x1BC;

		// SkyrimSE.exe+628DCD
		constexpr REL::ID smth_with_hitdata_proj_ID = REL::ID(37673);
		uintptr_t smth_with_hitdata_proj_ret = smth_with_hitdata_proj_ID.address() + 0x1AD;

		// SkyrimSE.exe+742BD1
		constexpr REL::ID init_hitdata_melee_ID = REL::ID(42832);
		uintptr_t init_hitdata_melee_ret = init_hitdata_melee_ID.address() + 0x381;

		// SkyrimSE.exe+742D43
		constexpr REL::ID init_hitdata_proj_ID = REL::ID(42833);
		uintptr_t init_hitdata_proj_ret = init_hitdata_proj_ID.address() + 0x143;

		// SkyrimSE.exe+743625
		constexpr REL::ID funcOffset = REL::ID(42842);
		uintptr_t ret_addr = funcOffset.address() + 0x10C + 0x9;

		struct Code : Xbyak::CodeGenerator
		{
			Code(uintptr_t func_melee, uintptr_t func_proj, uintptr_t ret_addr, uintptr_t init_hitdata_melee_ret, uintptr_t init_hitdata_proj_ret, uintptr_t smth_with_hitdata_melee_ret, uintptr_t smth_with_hitdata_proj_ret)
			{
				Xbyak::Label L__origin;
				Xbyak::Label L__restore;
				Xbyak::Label L__nomelee;

				mov(rax, smth_with_hitdata_melee_ret);
				cmp(qword[rsp + 0x168], rax);
				jne(L__nomelee);
				mov(rax, init_hitdata_melee_ret);
				cmp(qword[rsp + 0x108], rax);
				jne(L__nomelee);

				sub(rsp, 0x20);
				mov(r8, rbx);                     // target
				mov(r9, ptr[rbp + 0x57 - 0xA8]);  // aggressor
				mov(rax, func_melee);
				call(rax);
				add(rsp, 0x20);
				movss(xmm1, xmm0);
				jmp(L__restore);

				L(L__nomelee);
				mov(rax, smth_with_hitdata_proj_ret);
				cmp(qword[rsp + 0x158], rax);
				jne(L__origin);
				mov(rax, init_hitdata_proj_ret);
				cmp(qword[rsp + 0x108], rax);
				jne(L__origin);

				mov(rax, ptr[rsp + 0xF0]);
				sub(rsp, 0x30);
				mov(r8, rbx);                     // target
				mov(r9, ptr[rbp + 0x57 - 0xA8]);  // aggressor
				mov(ptr[rsp + 0x20], rax);
				mov(rax, func_proj);
				call(rax);
				add(rsp, 0x30);
				movss(xmm1, xmm0);
				jmp(L__restore);

				L(L__origin);
				addss(xmm1, xmm0);  // restore hook1
				L(L__restore);
				movss(ptr[rbp + 0x77], xmm1);  // restore hook2
				mov(rax, ret_addr);
				jmp(rax);
			}
		} xbyakCode{ 
			(uintptr_t)LocationalDamage_melee,
			(uintptr_t)LocationalDamage_proj,
			ret_addr, init_hitdata_melee_ret,
			init_hitdata_proj_ret,
			smth_with_hitdata_melee_ret,
			smth_with_hitdata_proj_ret
		};
		add_trampoline<5, 42842, 0x10C>(&xbyakCode);  // SkyrimSE.exe+74361C
	}

	void apply_NoPenetration() {
		// SkyrimSE.exe+743AFB
		constexpr REL::ID funcOffset = REL::ID(42842);
		uintptr_t ret_addr_clear = funcOffset.address() + 0x5EB;

		// SkyrimSE.exe+743B13
		uintptr_t ret_addr_noclear = funcOffset.address() + 0x603;

		struct Code : Xbyak::CodeGenerator
		{
			Code(uintptr_t ret_addr_clear, uintptr_t ret_addr_noclear)
			{
				Xbyak::Label L__clear;

				call(qword[rax + 0x20]);
				test(al, al);
				jnz(L__clear);
				mov(eax, 0xBF800000);
				cmp(dword[rsp + 0x120], eax);
				je(L__clear);

				mov(rax, ret_addr_noclear);
				jmp(rax);

				L(L__clear);
				mov(rax, ret_addr_clear);
				jmp(rax);
			}
		} xbyakCode{ ret_addr_clear, ret_addr_noclear };
		add_trampoline<5, 42842, 0x5E4>(&xbyakCode);  // SkyrimSE.exe+743AF4
	}

	void apply_hooks()
	{
		SKSE::AllocTrampoline(1 << 10);

		apply_LocationalDamage();
		apply_NoPenetration();

		MainUpdateHook::Hook();
		PlayerCharacterHook::Hook();
		CharacterHook::Hook();
	}
}
