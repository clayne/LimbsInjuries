#include "WeaponHandler.h"
#include "Settings.h"
#include <UselessFenixUtils.h>

constexpr RE::FormID ArmorMaterialHide = 0x0006BBDD;
constexpr RE::FormID ArmorMaterialStormcloak = 0x000AC13A;
constexpr RE::FormID ArmorMaterialImperialLight = 0x0006BBE0;
constexpr RE::FormID ArmorMaterialStudded = 0x0006BBDF;
constexpr RE::FormID ArmorMaterialLeather = 0x0006BBDB;
constexpr RE::FormID ArmorMaterialElven = 0x0006BBD9;
constexpr RE::FormID ArmorMaterialElvenGilded = 0x0006BBDA;
constexpr RE::FormID DLC2ArmorMaterialChitinLight = 0x04024102;
constexpr RE::FormID ArmorMaterialScaled = 0x0006BBDE;
constexpr RE::FormID ArmorMaterialGlass = 0x0006BBDC;
constexpr RE::FormID DLC2ArmorMaterialStalhrimLight = 0x04024107;
constexpr RE::FormID ArmorMaterialDragonscale = 0x0006BBD6;
constexpr RE::FormID ArmorMaterialImperialStuddedLight = 0x0006BBE1;  // duplicate ArmorMaterialImperialStudded

constexpr RE::FormID ArmorMaterialIron = 0x0006BBE3;
constexpr RE::FormID ArmorMaterialIronBanded = 0x0006BBE4;
constexpr RE::FormID ArmorMaterialImperialHeavy = 0x0006BBE2;
constexpr RE::FormID ArmorMaterialImperialStuddedHeavy = 0x0006BBE1;  // duplicate ArmorMaterialImperialStudded
constexpr RE::FormID ArmorMaterialSteel = 0x0006BBE6;
constexpr RE::FormID ArmorMaterialSteelPlate = 0x0006BBE7;
constexpr RE::FormID DLC2ArmorMaterialBonemoldHeavy = 0x04024101;
constexpr RE::FormID ArmorMaterialDwarven = 0x0006BBD7;
constexpr RE::FormID DLC2ArmorMaterialChitinHeavy = 0x04024103;
constexpr RE::FormID ArmorMaterialOrcish = 0x0006BBE5;
constexpr RE::FormID ArmorMaterialEbony = 0x0006BBD8;
constexpr RE::FormID DLC2ArmorMaterialNordicHeavy = 0x04024105;
constexpr RE::FormID ArmorMaterialDragonplate = 0x0006BBD5;
constexpr RE::FormID DLC2ArmorMaterialStalhrimHeavy = 0x04024106;
constexpr RE::FormID ArmorMaterialDaedric = 0x0006BBD4;

constexpr RE::FormID WeapMaterialWood = 0x0001E717;
constexpr RE::FormID WeapMaterialIron = 0x0001E718;
constexpr RE::FormID WeapMaterialSteel = 0x0001E719;
constexpr RE::FormID WeapMaterialImperial = 0x000C5C00;
constexpr RE::FormID WeapMaterialDraugr = 0x000C5C01;
constexpr RE::FormID WeapMaterialSilver = 0x0010AA1A;
constexpr RE::FormID WeapMaterialOrcish = 0x0001E71C;
constexpr RE::FormID WeapMaterialFalmer = 0x000C5C03;
constexpr RE::FormID WeapMaterialDwarven = 0x0001E71A;
constexpr RE::FormID WeapMaterialDraugrHoned = 0x000C5C02;
constexpr RE::FormID DLC2WeaponMaterialNordic = 0x04026230;
constexpr RE::FormID WeapMaterialFalmerHoned = 0x000C5C04;
constexpr RE::FormID WeapMaterialElven = 0x0001E71B;
constexpr RE::FormID WeapMaterialGlass = 0x0001E71D;
constexpr RE::FormID DLC2WeaponMaterialStalhrim = 0x0402622F;
constexpr RE::FormID WeapMaterialEbony = 0x0001E71E;
constexpr RE::FormID WeapMaterialDaedric = 0x0001E71F;

bool is_human(RE::Actor* a)
{
	auto race = a->GetRace();
	if (!race)
		return false;

	auto flag = race->validEquipTypes.underlying();
	auto mask = static_cast<uint32_t>(RE::TESRace::EquipmentFlag::kOneHandSword) |
	            static_cast<uint32_t>(RE::TESRace::EquipmentFlag::kTwoHandSword);
	return (flag & mask) > 0;
}

float get_discount(float attack, float defence)
{
	float a = -0.007142857143f, b = 0.01666666667f, c = -0.7f;
	float ans = a * attack + b * defence + c;
	return FenixUtils::clamp01(ans * static_cast<float>(*Settings::BlockedMult));
}

float get_penetration_prop(float discount, float attack)
{
	float ans = (1.0f - discount) + attack / 100.0f;
	return FenixUtils::clamp01(ans * static_cast<float>(*Settings::PenMult));
}

float get_injury_prop(float discount)
{
	float a = 0.3f, b = 0.9f;
	if (discount >= b)
		return 0.0f;
	float ans = a * (b - discount) * (b - discount);
	return FenixUtils::clamp01(ans * static_cast<float>(*Settings::InjuryMult));
}

static float get_armor__1401D7330(RE::InventoryEntryData* item, float armorPerks, float ArmorRating)
{
	using func_t = decltype(&get_armor__1401D7330);
	REL::Relocation<func_t> func{ REL::ID(15779) };
	return func(item, armorPerks, ArmorRating);
}

const static float ArmorPartCoeffs[] = { 1.0f / 0.22f, 1.0f / 0.47f, 1.0f / 0.16f, 1.0f / 0.16f, 1.0f / 0.47f };

float get_armor(RE::Actor* victim, uint32_t slot_mask, float armorPerks, float lightArmorRating, float heavyArmorRating, uint32_t limb)
{
	auto changes = victim->GetInventoryChanges()->entryList;
	if (!changes)
		return 0.0f;

	for (auto it = changes->begin(); it != changes->end(); ++it) {
		auto cur = (*it);
		auto cur_armo = cur->GetObject()->As<RE::TESObjectARMO>();
		if (!cur_armo)
			continue;

		if ((static_cast<uint32_t>(cur_armo->GetSlotMask()) & slot_mask) > 0) {
			float ArmorRating;

			if (cur_armo->GetArmorType() == RE::BGSBipedObjectForm::ArmorType::kHeavyArmor)
				ArmorRating = heavyArmorRating;
			else
				ArmorRating = lightArmorRating;

			return get_armor__1401D7330(cur, armorPerks, ArmorRating) * ArmorPartCoeffs[limb];
		}
	}
	return 0.0f;
}

float ActorValueOwner__get_ArmorRating__1403BE980(RE::ActorValueOwner* a, float armoskill)
{
	using func_t = decltype(&ActorValueOwner__get_ArmorRating__1403BE980);
	REL::Relocation<func_t> func{ REL::ID(25858) };
	return func(a, armoskill);
}

float ExtraContainerChanges__get_max_something(RE::InventoryEntryData* item)
{
	return _generic_foo_<15752, decltype(ExtraContainerChanges__get_max_something)>::eval(item);
}

static float smth_smithed(float mb_smithed) {
	return _generic_foo_<25915, decltype(smth_smithed)>::eval(mb_smithed);
}

static float ActorValueOwner__get_skill(RE::ActorValueOwner* a1, RE::ActorValue skill)
{
	return _generic_foo_<26616, decltype(ActorValueOwner__get_skill)>::eval(a1, skill);
}

static float get_skill_mult_fair(float skill) {
	return FenixUtils::lerp<1.0f, 3.0f>(skill * 0.01f);
}

static float get_damage(RE::InventoryEntryData* _weap, RE::ActorValueOwner* a, float DamageMult)
{
	auto weap = _weap->GetObject()->As<RE::TESObjectWEAP>();
	if (!weap || weap->formType != RE::FormType::Weapon)
		return 0.0;
	
	auto skill_ind = weap->weaponData.skill.get();
	float damage = weap->GetAttackDamage();
	auto type = weap->weaponData.animationType.get();
	float av_damage = 0.0f;
	float smithed_bonus = (type == RE::WEAPON_TYPE::kCrossbow) ? 0.0f : smth_smithed(ExtraContainerChanges__get_max_something(_weap));
	switch (type) {
	case RE::WEAPON_TYPE::kHandToHandMelee:
		av_damage = a->GetActorValue(RE::ActorValue::kUnarmedDamage);
		break;
	case RE::WEAPON_TYPE::kOneHandSword:
	case RE::WEAPON_TYPE::kOneHandDagger:
	case RE::WEAPON_TYPE::kOneHandAxe:
	case RE::WEAPON_TYPE::kOneHandMace:
	case RE::WEAPON_TYPE::kTwoHandSword:
	case RE::WEAPON_TYPE::kTwoHandAxe:
		av_damage = a->GetActorValue(RE::ActorValue::kMeleeDamage);
		break;
	default:
		break;
	}

	float skill_mult = get_skill_mult_fair(ActorValueOwner__get_skill(a, skill_ind));
	return (smithed_bonus + damage) * skill_mult * DamageMult + av_damage;
}

static float get_damage_mult(RE::Actor* attacker)
{
	if (!attacker->currentProcess || !attacker->currentProcess->high || attacker->currentProcess->high->attackData.get())
		return 1.0f;
	return attacker->currentProcess->high->attackData.get()->data.damageMult;
}

float get_damage(RE::Actor* attacker)
{
	float unarmed = attacker->GetActorValue(RE::ActorValue::kUnarmedDamage);

	auto _weap = attacker->GetAttackingWeapon();
	if (!_weap)
		return unarmed;

	auto __weap = _weap->GetObject();
	if (!__weap)
		return unarmed;

	auto weap = __weap->As<RE::TESObjectWEAP>();
	if (!weap)
		return unarmed;

	return get_damage(_weap, attacker->As<RE::ActorValueOwner>(), get_damage_mult(attacker));
}
