#pragma once

float LocationalDamage_melee(float ArmorRating, float DamageResist, RE::Actor* victim, RE::Actor* attacker);
float LocationalDamage_proj(float ArmorRating, float DamageResist, RE::Actor* victim, RE::Actor* attacker, RE::ArrowProjectile* proj);
void draw_player_weapon(float delta);
