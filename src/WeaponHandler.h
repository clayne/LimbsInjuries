#pragma once

using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;

float get_penetration_prop(float, float);
float get_discount(float attack, float);
float get_penetraion_prop(float damage_mult, float attack);
float get_injury_prop(float damage_mult);
float get_armor(RE::Actor* victim, uint32_t slot_mask, float armorPerks, float lightArmorRating, float heavyArmorRating, uint32_t limb);
float ActorValueOwner__get_ArmorRating__1403BE980(RE::ActorValueOwner* a, float armoskill);
float get_damage(RE::Actor* attacker);
