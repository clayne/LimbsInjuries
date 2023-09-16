#pragma once

struct Settings
{
private:
	using iSetting = AutoTOML::iSetting;
	using bSetting = AutoTOML::bSetting;
	using fSetting = AutoTOML::fSetting;
	using sSetting = AutoTOML::sSetting;

public:
	static void load()
	{
		try {
			const auto table = toml::parse_file("Data/SKSE/Plugins/LimbsInjuries.toml"s);
			for (const auto& setting : AutoTOML::ISetting::get_settings()) {
				setting->load(table);
			}
		} catch (const toml::parse_error& e) {
			std::ostringstream ss;
			ss
				<< "Error parsing file \'" << *e.source().path << "\':\n"
				<< '\t' << e.description() << '\n'
				<< "\t\t(" << e.source().begin << ')';
			logger::error(ss.str());
			throw std::runtime_error("failed to load settings"s);
		}
	}

	static inline fSetting BlockedMult{ "Blocked"s, "Mult"s, 1.0f };
	static inline fSetting InjuryMult{ "Injury"s, "Mult"s, 1.0f };
	static inline bSetting InjuryArmUnequip{ "Injury"s, "Unequip"s, true };
	static inline fSetting PenMult{ "Penetration"s, "Mult"s, 1.0f };

	enum class NotificationModes
	{
		All, Player, Radius, Disable
	};
	static inline struct NotificationSetting
	{
		iSetting mode;
		fSetting r;
		NotificationModes get_mode() {
			return static_cast<NotificationModes>(*mode);
		}
		float get_radius() {
			return static_cast<float>(*r);
		}
	} Notification{
		{ "General"s, "NotificationMode"s, 0 },
		{ "General"s, "NotificationRadius"s, 10000000.0f }
	};

	static inline sSetting DataImpactBounce = { "DataImpacts"s, "Bounce"s, "effects\\impacteffects\\FXMetalSparkImpactSlice.nif" };
	static inline sSetting DataImpactInjury = { "DataImpacts"s, "Injury"s, "effects\\impacteffects\\mpsbloodspraybigmist.nif" };
};
