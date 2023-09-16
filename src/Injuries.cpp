#include <Injuries.h>
#include "SoundPlayer.h"
#include <UselessFenixUtils.h>

namespace Injuries
{
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

	template<int id>
	void debug_notification(RE::Actor* victim, RE::Actor* attacker)
	{
		auto mode = Settings::Notification.get_mode();

		if (mode == Settings::NotificationModes::Disable)
			return;

		if (mode == Settings::NotificationModes::Radius && !victim->IsPlayerRef() && !attacker->IsPlayerRef()) {
			float dist1 = FenixUtils::get_dist2(RE::PlayerCharacter::GetSingleton(), attacker);
			float dist2 = FenixUtils::get_dist2(RE::PlayerCharacter::GetSingleton(), victim);
			float dist = std::min(dist1, dist2);
			//logger::info("dist = {}, r = {}", dist, Settings::Notification.get_radius());
			if (dist > Settings::Notification.get_radius())
				return;
		}

		return debug_notification(victim, attacker, LimbsInjuriesESP::msgs()[id]);
	}

	template <int id>
	void cast_spell(RE::Actor* victim, RE::Actor* attacker)
	{
		return FenixUtils::cast_spell(victim, attacker, LimbsInjuriesESP::spells()[id]);
	}

	void apply_head_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification<Head>(victim, attacker);
		Sounds::play_sound(victim, Sounds::SoundTypes::InjuHead);

		FenixUtils::damageav(victim, RE::ActorValue::kMagicka, -1000000.0f);

		auto helmet = victim->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead);
		if (!helmet)
			helmet = victim->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHair);
		FenixUtils::UnequipItem(victim, helmet);
		
		cast_spell<Head>(victim, victim);
	}

	void apply_body_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification<Body>(victim, attacker);
		Sounds::play_sound(victim, Sounds::SoundTypes::InjuBody);

		FenixUtils::damageav(victim, RE::ActorValue::kStamina, -0.5f * FenixUtils::get_total_av(victim, RE::ActorValue::kStamina));

		RE::MagicCaster* caster = victim->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		auto spell = LimbsInjuriesESP::spells()[Body];
		if (caster && spell) {
			float sum = 0.0f;
			sum += victim->GetBaseActorValue(RE::ActorValue::kHealth);
			sum += victim->GetBaseActorValue(RE::ActorValue::kMagicka);
			sum += victim->GetBaseActorValue(RE::ActorValue::kStamina);
			caster->CastSpellImmediate(spell, false, victim, 1.0f, false, sum / 6.0f, victim);
		}
	}

	void apply_arms_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification<Arms>(victim, attacker);
		Sounds::play_sound(victim, Sounds::SoundTypes::InjuArms);

		if (*Settings::InjuryArmUnequip)
			unequip_weapons(victim);

		cast_spell<Arms>(victim, victim);
	}

	void apply_legs_injury(RE::Actor* victim, RE::Actor* attacker)
	{
		debug_notification<Legs>(victim, attacker);
		Sounds::play_sound(victim, Sounds::SoundTypes::InjuLegs);

		//if (victim->IsPlayerRef())
		//	FenixUtils::stagger<1.0f>(victim, attacker);
		//else
		//	FenixUtils::knock(victim, attacker, 5.0f);
		FenixUtils::stagger<1.0f>(victim, attacker);

		cast_spell<Legs>(victim, victim);
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
			return;
		}
	}

	template <int id>
	bool has_mgef(RE::Actor* a)
	{
		return a->HasMagicEffect(LimbsInjuriesESP::mgeffs()[id]);
	}

	void on_attack(RE::Actor* a) {
		using AttackFlag = RE::AttackData::AttackFlag;

		if (!a->currentProcess || !a->currentProcess->high || !a->currentProcess->high->attackData.get())
			return;
		
		auto flags = a->currentProcess->high->attackData.get()->data.flags;
		if (flags.any(AttackFlag::kBashAttack, AttackFlag::kPowerAttack) && has_mgef<Arms>(a)) {
			FenixUtils::stagger(a);
		}
	}

	void on_sprint(RE::Actor* a)
	{
		if (has_mgef<Legs>(a)) {
			FenixUtils::stagger<0.2f>(a);
		}
	}

	void on_attacked(RE::Actor* victim, RE::Actor* aggressor, SKSE::stl::enumeration<RE::TESHitEvent::Flag, std::uint8_t> flags)
	{
		using HitFlag = RE::TESHitEvent::Flag;

		if (flags.any(HitFlag::kBashAttack, HitFlag::kPowerAttack) && has_mgef<Body>(victim)) {
			FenixUtils::stagger(victim, aggressor);
		}
	}

	void on_equipped(RE::Actor*, bool)
	{
		//using HitFlag = RE::TESHitEvent::Flag;
		//
		//if (equipped && has_mgef<Arms>(a)) {
		//	if (a->IsPlayerRef())
		//		unequip_weapons(a);
		//}
	}
}
