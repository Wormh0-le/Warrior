// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Items/Weapons/WarriorHeroWeapon.h"
#include "GameplayAbilitySpecHandle.h"


void AWarriorHeroWeapon::AssignGrantedAbilitySpecHandles(
	const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
