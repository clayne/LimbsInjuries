#pragma once
#include <array>

namespace Sounds
{
	enum class SoundTypes
	{
		ArroBounClot,
		ArroBounIron,
		ArroBounSmth,

		WeapBounClot,
		WeapBounIronAxe_,
		WeapBounIronBlad,
		WeapBounIronBlun,
		WeapBounSmth,

		InjuHead,
		InjuArms,
		InjuBody,
		InjuLegs,

		Total
	};

	void play_sound(RE::TESObjectREFR* a, SoundTypes type);
	void play_sound_nopen_melee(RE::Actor* attacker, RE::Actor* victim);
	void play_sound_nopen_arrow(RE::Actor* attacker, RE::Actor* victim);
}
