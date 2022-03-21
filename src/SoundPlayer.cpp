#include "SoundPlayer.h"
#include <UselessFenixUtils.h>


namespace Sounds
{
	class Storage
	{
	public:
		static constexpr std::array<int, static_cast<size_t>(SoundTypes::Total)> sounds_storage = { 0x0003C845, 0x0004D2DA, 0x0004D2DE, 0x000AF63E, 0x0003C856, 0x0003C73C, 0x0003C85A, 0x0003C744, 0x0009CE5B, 0x000DAB82, 0x000E8E8B, 0x000DCAF7 };
		static constexpr std::array<int, static_cast<size_t>(SoundTypes::Total)> impact_storage = { 0x000193B5, 0x0004BB54, 0x000193B6, 0x00013CBA, 0x0004BB52, 0x0004BB52, 0x0004BB52, 0x000193B6, 0x0004BB52, 0x0004BB52, 0x0004BB52, 0x0004BB52 };
	};

	void play_sound(RE::TESObjectREFR* a, SoundTypes type)
	{
		FenixUtils::play_sound(a, Storage::sounds_storage[static_cast<size_t>(type)]);
	}

	enum class ArmorTypes
	{
		Clot,
		Iron,
		Smth
	};

	ArmorTypes get_armor_type(RE::Actor* a)
	{
		auto armo = a->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody);
		if (!armo)
			return ArmorTypes::Clot;

		switch (armo->GetArmorType()) {
		case RE::BIPED_MODEL::ArmorType::kClothing:
			return ArmorTypes::Clot;
		case RE::BGSBipedObjectForm::ArmorType::kLightArmor:
			return ArmorTypes::Iron;
		case RE::BGSBipedObjectForm::ArmorType::kHeavyArmor:
			return ArmorTypes::Smth;
		default:
			return ArmorTypes::Clot;
		}
	}

	enum class WeaponTypes
	{
		Iron,
		Smth
	};

	std::pair<WeaponTypes, RE::stl::enumeration<RE::WEAPON_TYPE, std::uint8_t>> get_weapon_type(RE::Actor* a)
	{
		auto _weap = a->GetEquippedObject(false);
		if (!_weap)
			return { WeaponTypes::Smth, RE::WEAPON_TYPE::kHandToHandMelee };

		auto weap = _weap->As<RE::TESObjectWEAP>();
		if (!weap)
			return { WeaponTypes::Smth, RE::WEAPON_TYPE::kHandToHandMelee };

		auto type = weap->weaponData.animationType;
		if (type.any(RE::WEAPON_TYPE::kOneHandAxe, RE::WEAPON_TYPE::kOneHandDagger, RE::WEAPON_TYPE::kOneHandSword, RE::WEAPON_TYPE::kTwoHandAxe, RE::WEAPON_TYPE::kTwoHandSword))
			return { WeaponTypes::Iron, type };
		else
			return { WeaponTypes::Smth, RE::WEAPON_TYPE::kHandToHandMelee };
	}

	SoundTypes get_nopen_sound_type_melee(ArmorTypes armo, const std::pair<WeaponTypes, RE::stl::enumeration<RE::WEAPON_TYPE, std::uint8_t>> weap)
	{
		if (armo == ArmorTypes::Smth || weap.first == WeaponTypes::Smth)
			return SoundTypes::WeapBounSmth;

		if (armo == ArmorTypes::Clot)
			return SoundTypes::WeapBounClot;

		if (weap.second.any(RE::WEAPON_TYPE::kOneHandDagger, RE::WEAPON_TYPE::kOneHandSword, RE::WEAPON_TYPE::kTwoHandSword))
			return SoundTypes::WeapBounIronBlad;

		if (weap.second.any(RE::WEAPON_TYPE::kOneHandAxe, RE::WEAPON_TYPE::kTwoHandAxe))
			return SoundTypes::WeapBounIronAxe_;

		if (weap.second.any(RE::WEAPON_TYPE::kOneHandMace))
			return SoundTypes::WeapBounIronBlun;

		return SoundTypes::WeapBounSmth;
	}

	SoundTypes get_nopen_sound_type_arrow(ArmorTypes armo)
	{
		switch (armo) {
		case ArmorTypes::Clot:
			return SoundTypes::ArroBounClot;
		case ArmorTypes::Iron:
			return SoundTypes::ArroBounIron;
		case ArmorTypes::Smth:
			return SoundTypes::ArroBounSmth;
		default:
			return SoundTypes::ArroBounClot;
		}
	}

	void play_sound_nopen_melee(RE::Actor* attacker, RE::Actor* victim)
	{
		play_sound(victim, get_nopen_sound_type_melee(get_armor_type(victim), get_weapon_type(attacker)));
	}

	void play_sound_nopen_arrow(RE::Actor*, RE::Actor* victim)
	{
		play_sound(victim, get_nopen_sound_type_arrow(get_armor_type(victim)));
	}
}
