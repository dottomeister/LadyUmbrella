// 
// UpgradeComponent.cpp
// 
// Implementation of the UmbrellaComponent class. 
// 
// Copyright Zulo Interactive. All Rights Reserved.
// 

#include "UpgradeComponent.h"

UUpgradeComponent::UUpgradeComponent()
{
	UpgradeCost.Emplace(EUpgradeType::AmmoCapacity, 1);
	UpgradeCost.Emplace(EUpgradeType::Handling, 3);
	UpgradeCost.Emplace(EUpgradeType::FireRate, 1);
	UpgradeCost.Emplace(EUpgradeType::ReloadSpeed, 2);

	// UpgradeCost.Emplace(EUpgradeType::Bounciness, 1);
	UpgradeCost.Emplace(EUpgradeType::Endurance, 4);
	UpgradeCost.Emplace(EUpgradeType::ShieldRegen, 1);
	// UpgradeCost.Emplace(EUpgradeType::ShieldMotion, 1);
}

void UUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Shotgun upgrades.
	Upgrades.Emplace(EUpgradeType::AmmoCapacity, FUpgrade("Ammo Capacity", "Increase the ammo capacity", 1,
	                                                      TArray{UpgradeCost[EUpgradeType::AmmoCapacity]}));
	Upgrades.Emplace(EUpgradeType::Handling, FUpgrade("Handling", "Increase the handling of the upgrade", 1,
	                                                  TArray{UpgradeCost[EUpgradeType::Handling]}));
	Upgrades.Emplace(EUpgradeType::FireRate, FUpgrade("Fire Rate", "Increase the fire rate of the shotgun", 1,
	                                                  TArray{UpgradeCost[EUpgradeType::FireRate]}));
	Upgrades.Emplace(EUpgradeType::ReloadSpeed, FUpgrade("Reload Speed", "Increase the reload speed", 1,
	                                                     TArray{UpgradeCost[EUpgradeType::ReloadSpeed]}));

	// Shield upgrades. 
	// Upgrades.Emplace(EUpgradeType::Bounciness, FUpgrade("Bounciness","Increase the bounciness", 1,
	// 	TArray<uint8>({UpgradeCost[EUpgradeType::Bounciness]})));
	Upgrades.Emplace(EUpgradeType::Endurance, FUpgrade("Endurance", "Increase the health of the shield", 1,
	                                                   TArray<uint8>({UpgradeCost[EUpgradeType::Endurance]})));
	Upgrades.Emplace(EUpgradeType::ShieldRegen, FUpgrade("Shield Regeneration", "Increases the shield regeneration speed", 1,
												   TArray<uint8>({UpgradeCost[EUpgradeType::ShieldRegen]})));
	// Upgrades.Emplace(EUpgradeType::ShieldMotion, FUpgrade("Shield Motion","Increase the shield agility", 1,
	// 	TArray<uint8>({UpgradeCost[EUpgradeType::ShieldMotion]})));
}

FUpgrade* UUpgradeComponent::Get(const EUpgradeType Type)
{
	return Upgrades.Find(Type);
}
