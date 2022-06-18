#pragma once
#include <array>

namespace Injuries
{
	enum Limbs
	{
		Head,
		Body,
		Arms,
		Legs,
		Back,

		Total
	};

	class LimbsInjuriesESP
	{
		static constexpr auto ESP_NAME = "LimbsInjuries.esp";
	public:
		static void init() {
			static bool init{ false };
		}

		static const std::array<RE::SpellItem*, Total>& spells()
		{
			auto handler = RE::TESDataHandler::GetSingleton();

			static std::array<RE::SpellItem*, Total> a = {
				handler->LookupForm<RE::SpellItem>(RE::FormID(0x81C), ESP_NAME),  // Head
				handler->LookupForm<RE::SpellItem>(RE::FormID(0x81B), ESP_NAME),  // Body
				handler->LookupForm<RE::SpellItem>(RE::FormID(0x833), ESP_NAME),  // Arms
				handler->LookupForm<RE::SpellItem>(RE::FormID(0x81D), ESP_NAME),  // Legs
				nullptr                                                           // Back
			};

			return a;
		}

		static const std::array<RE::EffectSetting*, Total>& mgeffs()
		{
			auto handler = RE::TESDataHandler::GetSingleton();

			static std::array<RE::EffectSetting*, Total> a = {
				handler->LookupForm<RE::EffectSetting>(RE::FormID(0x821), ESP_NAME),  // Head
				handler->LookupForm<RE::EffectSetting>(RE::FormID(0x828), ESP_NAME),  // Body
				handler->LookupForm<RE::EffectSetting>(RE::FormID(0x845), ESP_NAME),  // Arms
				handler->LookupForm<RE::EffectSetting>(RE::FormID(0x834), ESP_NAME),  // Legs
				nullptr                                                               // Back
			};

			return a;
		}

		static const std::array<RE::BGSMessage*, Total>& msgs()
		{
			auto handler = RE::TESDataHandler::GetSingleton();

			static std::array<RE::BGSMessage*, Total> a = {
				handler->LookupForm<RE::BGSMessage>(RE::FormID(0x841), ESP_NAME),  // Head
				handler->LookupForm<RE::BGSMessage>(RE::FormID(0x842), ESP_NAME),  // Body
				handler->LookupForm<RE::BGSMessage>(RE::FormID(0x843), ESP_NAME),  // Arms
				handler->LookupForm<RE::BGSMessage>(RE::FormID(0x844), ESP_NAME),  // Legs
				nullptr                                                            // Back
			};

			return a;
		}
	};

	void apply_injury(Limbs ind, RE::Actor* victim, RE::Actor* attacker);
	void on_attack(RE::Actor* a);
	void on_sprint(RE::Actor* a);
	void on_attacked(RE::Actor* victim, RE::Actor* aggressor, SKSE::stl::enumeration<RE::TESHitEvent::Flag, std::uint8_t> flags);
	void on_equipped(RE::Actor* a, bool equipped);
}
using Injuries::Limbs;
using Injuries::LimbsInjuriesESP;
