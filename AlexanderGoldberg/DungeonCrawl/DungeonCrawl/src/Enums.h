#pragma once
namespace Enums {
	enum class DamageType {
		Fire,
		Ice,
		Piercing,
		Acid,
		Bludgeoning,
		Electric,
		Concussive

	};


	static std::string PrintDTName(DamageType _type)
	{
		std::string names[7] = { "Fire",
		"Ice",
		"Piercing",
		"Acid",
		"Bludgeoning",
		"Electric",
		"Concussive" };
		return names[(int)_type];

	}
};