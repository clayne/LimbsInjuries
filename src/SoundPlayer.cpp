#include "SoundPlayer.h"


namespace Sounds
{
	void* BSAudioManager__GetSingleton()
	{
		using func_t = decltype(&BSAudioManager__GetSingleton);
		REL::Relocation<func_t> func{ RE::Offset::BSAudioManager::GetSingleton };
		return func();
	}

	int PlaySound_func1_140BEEE70(void* manager, RE::BSSoundHandle* a2, int a3, int a4)
	{
		using func_t = decltype(&PlaySound_func1_140BEEE70);
		REL::Relocation<func_t> func{ REL::ID(66401) };
		return func(manager, a2, a3, a4);
	}

	char set_sound_position(RE::BSSoundHandle* a1, float x, float y, float z)
	{
		using func_t = decltype(&set_sound_position);
		REL::Relocation<func_t> func{ REL::ID(66370) };
		return func(a1, x, y, z);
	}

	void PlaySound_func3_140BEDB10(RE::BSSoundHandle* a1, RE::NiAVObject* source_node)
	{
		using func_t = decltype(&PlaySound_func3_140BEDB10);
		REL::Relocation<func_t> func{ REL::ID(66375) };
		return func(a1, source_node);
	}

	char PlaySound_func4_140BEDB10(RE::BSSoundHandle* a1)
	{
		using func_t = decltype(&PlaySound_func4_140BEDB10);
		REL::Relocation<func_t> func{ REL::ID(66355) };
		return func(a1);
	}

	void play_sound(RE::TESObjectREFR* a, int formid)
	{
		RE::BSSoundHandle handle;
		handle.soundID = static_cast<uint32_t>(-1);
		handle.assumeSuccess = false;
		*(uint32_t*)&handle.state = 0;

		auto manager = BSAudioManager__GetSingleton();
		PlaySound_func1_140BEEE70(manager, &handle, formid, 16);
		if (set_sound_position(&handle, a->data.location.x, a->data.location.y, a->data.location.z)) {
			PlaySound_func3_140BEDB10(&handle, a->Get3D());
			PlaySound_func4_140BEDB10(&handle);
		}
	}

	class Storage
	{
	public:
		static constexpr std::array<int, static_cast<size_t>(SoundTypes::Total)> storage = { 0x0003C845, 0x0004D2DA, 0x0004D2DE, 0x000AF63E, 0x0003C856, 0x0003C73C, 0x0003C85A, 0x0003C744, 0x0009CE5B, 0x000DAB82, 0x000E8E8B, 0x000DCAF7 };
	};

	void play_sound(RE::TESObjectREFR* a, SoundTypes type)
	{
		play_sound(a, Storage::storage[static_cast<size_t>(type)]);
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
		case Sounds::ArmorTypes::Clot:
			return SoundTypes::ArroBounClot;
		case Sounds::ArmorTypes::Iron:
			return SoundTypes::ArroBounIron;
		case Sounds::ArmorTypes::Smth:
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
