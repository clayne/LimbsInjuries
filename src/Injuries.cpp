#include <Injuries.h>

namespace Injuries
{
	RE::SpellItem* LimbsInjuriesESP::get_Spell_Chest()
	{
		static RE::SpellItem* Spell_Chest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(RE::FormID(0x81B), ESP_NAME);
		return Spell_Chest;
	}

	RE::SpellItem* LimbsInjuriesESP::get_Spell_Head()
	{
		static RE::SpellItem* Spell_Head = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(RE::FormID(0x81C), ESP_NAME);
		return Spell_Head;
	}

	RE::SpellItem* LimbsInjuriesESP::get_Spell_Leg()
	{
		static RE::SpellItem* Spell_Leg = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(RE::FormID(0x81D), ESP_NAME);
		return Spell_Leg;
	}

	RE::SpellItem* LimbsInjuriesESP::get_Spell_Arms()
	{
		static RE::SpellItem* Spell_Arms = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(RE::FormID(0x833), ESP_NAME);
		return Spell_Arms;
	}

	RE::EffectSetting* LimbsInjuriesESP::get_Mgeff_Chest()
	{
		static RE::EffectSetting* Mgeff_Chest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(RE::FormID(0x828), ESP_NAME);
		return Mgeff_Chest;
	}

	RE::EffectSetting* LimbsInjuriesESP::get_Mgeff_Head()
	{
		static RE::EffectSetting* Mgeff_Head = RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(RE::FormID(0x821), ESP_NAME);
		return Mgeff_Head;
	}

	RE::EffectSetting* LimbsInjuriesESP::get_Mgeff_Legs()
	{
		static RE::EffectSetting* Mgeff_Leg = RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(RE::FormID(0x834), ESP_NAME);
		return Mgeff_Leg;
	}

	RE::EffectSetting* LimbsInjuriesESP::get_Mgeff_Arms()
	{
		static RE::EffectSetting* Mgeff_Arms = RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(RE::FormID(0x830), ESP_NAME);
		return Mgeff_Arms;
	}

	RE::BGSMessage* LimbsInjuriesESP::get_Msg_Arms()
	{
		static RE::BGSMessage* Msg_Arms = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSMessage>(RE::FormID(0x843), ESP_NAME);
		return Msg_Arms;
	}

	RE::BGSMessage* LimbsInjuriesESP::get_Msg_Chest()
	{
		static RE::BGSMessage* Msg_Chest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSMessage>(RE::FormID(0x842), ESP_NAME);
		return Msg_Chest;
	}

	RE::BGSMessage* LimbsInjuriesESP::get_Msg_Head()
	{
		static RE::BGSMessage* Msg_Head = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSMessage>(RE::FormID(0x841), ESP_NAME);
		return Msg_Head;
	}

	RE::BGSMessage* LimbsInjuriesESP::get_Msg_Legs()
	{
		static RE::BGSMessage* Msg_Legs = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSMessage>(RE::FormID(0x844), ESP_NAME);
		return Msg_Legs;
	}

	RE::TESObjectWEAP* Actor__GetWeapon_140625EB0(RE::Actor* a, bool left)
	{
		using func_t = decltype(&Actor__GetWeapon_140625EB0);
		REL::Relocation<func_t> func{ REL::ID(37621) };
		return func(a, left);
	}

	void unequip_weapons(RE::Actor* a)
	{
		auto manager = RE::ActorEquipManager::GetSingleton();

		auto weap = Actor__GetWeapon_140625EB0(a, false);
		if (weap)
			manager->UnequipObject(a, weap);

		weap = Actor__GetWeapon_140625EB0(a, true);
		if (weap)
			manager->UnequipObject(a, weap);
	}

	void debug_notification(RE::Actor* victim, RE::Actor* attacker, RE::BGSMessage* msg)
	{
		static char sMsgBuff[128] = { 0 };
		RE::BSString a;
		msg->GetDescription(a, msg);
		sprintf_s(sMsgBuff, a.c_str(), attacker->GetName(), victim->GetName());

		RE::DebugNotification(sMsgBuff);
	}

	void apply_head_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification(victim, attacker, LimbsInjuriesESP::get_Msg_Head());

		damageav(victim, RE::ACTOR_VALUE_MODIFIERS::kDamage, RE::ActorValue::kMagicka, -1000000.0f, attacker);

		auto helmet = victim->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead);
		if (!helmet)
			helmet = victim->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHair);
		UnequipItem(victim, helmet);
		
		cast_spell(victim, attacker, LimbsInjuriesESP::get_Spell_Head());
	}

	void apply_body_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification(victim, attacker, LimbsInjuriesESP::get_Msg_Chest());

		damageav(victim, RE::ACTOR_VALUE_MODIFIERS::kDamage, RE::ActorValue::kStamina, -1000000000.0f, attacker);

		RE::MagicCaster* caster = attacker->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		auto spell = LimbsInjuriesESP::get_Spell_Chest();
		if (caster && spell) {
			float sum = 0.0f;
			sum += victim->GetBaseActorValue(RE::ActorValue::kHealth);
			sum += victim->GetBaseActorValue(RE::ActorValue::kMagicka);
			sum += victim->GetBaseActorValue(RE::ActorValue::kStamina);
			caster->InstantCast(spell, false, victim, 1.0f, false, sum / 6.0f, attacker);
		}
	}

	void apply_arms_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification(victim, attacker, LimbsInjuriesESP::get_Msg_Arms());

		unequip_weapons(victim);

		cast_spell(victim, attacker, LimbsInjuriesESP::get_Spell_Arms());
	}

	void apply_legs_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification(victim, attacker, LimbsInjuriesESP::get_Msg_Legs());

		stagger<1.0f>(victim, attacker);

		cast_spell(victim, attacker, LimbsInjuriesESP::get_Spell_Leg());
	}

	void apply_injury(Limbs ind, RE::Actor* victim, RE::Actor* attacker)
	{
		switch (ind) {
		case Limbs::Head:
			return apply_head_injury(victim, attacker);
		case Limbs::Body:
			return apply_body_injury(victim, attacker);
		case Limbs::Arms:
			return apply_arms_injury(victim, attacker);
		case Limbs::Legs:
			return apply_legs_injury(victim, attacker);
		default:
			break;
		}
	}

	void on_attack(RE::Actor* a) {
		using AttackFlag = RE::AttackData::AttackFlag;

		if (!a->currentProcess || !a->currentProcess->high || !a->currentProcess->high->attackData.get())
			return;
		
		auto flags = a->currentProcess->high->attackData.get()->data.flags;
		if (flags.any(AttackFlag::kBashAttack, AttackFlag::kPowerAttack) && a->HasMagicEffect(LimbsInjuriesESP::get_Mgeff_Arms())) {
			stagger(a);
		}
	}

	void on_sprint(RE::Actor* a)
	{
		if (a->HasMagicEffect(LimbsInjuriesESP::get_Mgeff_Legs())) {
			stagger<0.2f>(a);
		}
	}

	void on_attacked(RE::Actor* victim, RE::Actor* aggressor, SKSE::stl::enumeration<RE::TESHitEvent::Flag, std::uint8_t> flags)
	{
		using HitFlag = RE::TESHitEvent::Flag;

		if (flags.any(HitFlag::kBashAttack, HitFlag::kPowerAttack) && victim->HasMagicEffect(LimbsInjuriesESP::get_Mgeff_Chest())) {
			stagger(victim, aggressor);
		}
	}

	void on_equipped(RE::Actor* a, bool equipped)
	{
		using HitFlag = RE::TESHitEvent::Flag;

		if (equipped && a->HasMagicEffect(LimbsInjuriesESP::get_Mgeff_Arms())) {
			if (a->IsPlayerRef())
				unequip_weapons(a);
		}
	}
}
