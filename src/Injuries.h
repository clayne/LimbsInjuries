#pragma once

namespace Injuries
{
	enum Limbs
	{
		Head,
		Body,
		Arms,
		Legs,
		Back
	};

	class LimbsInjuriesESP
	{
		static const inline auto ESP_NAME = "LimbsInjuries.esp";

	public:
		static RE::SpellItem* get_Spell_Head();
		static RE::SpellItem* get_Spell_Chest();
		static RE::SpellItem* get_Spell_Leg();
		static RE::SpellItem* get_Spell_Arms();

		static RE::EffectSetting* get_Mgeff_Head();
		static RE::EffectSetting* get_Mgeff_Chest();
		static RE::EffectSetting* get_Mgeff_Legs();
		static RE::EffectSetting* get_Mgeff_Arms();

		static RE::BGSMessage* get_Msg_Arms();
		static RE::BGSMessage* get_Msg_Head();
		static RE::BGSMessage* get_Msg_Chest();
		static RE::BGSMessage* get_Msg_Legs();
	};

	void apply_injury(Limbs ind, RE::Actor* victim, RE::Actor* attacker);
	void on_attack(RE::Actor* a);
	void on_sprint(RE::Actor* a);
	void on_attacked(RE::Actor* victim, RE::Actor* aggressor, SKSE::stl::enumeration<RE::TESHitEvent::Flag, std::uint8_t> flags);
	void on_equipped(RE::Actor* a, bool equipped);
}
using Injuries::Limbs;
using Injuries::LimbsInjuriesESP;
