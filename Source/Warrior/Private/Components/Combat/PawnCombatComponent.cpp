// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h" 

#include "Characters/WarriorBaseCharacter.h"


void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
	AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A Tag named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	/*
	* Unreal Engine 委托常见绑定方法还有：
	* - `BindUObject`：绑定到 UObject 成员函数，适用于需要访问对象成员的 C++ 逻辑。
	* - `BindUFunction`：绑定到 UObject 的 UFUNCTION，适合蓝图交互或反射调用。
	* - `BindLambda`：绑定到 Lambda 表达式，适合临时、轻量、无需成员变量的回调。
	* - `BindStatic`：绑定到静态函数，适合无状态、工具类函数。
	* - `AddDynamic`：用于动态委托，支持蓝图和序列化，适合事件广播。
	* 选择依据：是否需要对象状态、是否与蓝图交互、是否需要序列化、是否为临时回调。
	 */
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnWeaponHitTargetActor);
	InWeaponToRegister->OnWeaponEndHitTarget.BindUObject(this, &ThisClass::OnWeaponPullback);
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		/**
		*总结如下：
		*	- `AWarriorWeaponBase*`：普通指针，指向 `AWarriorWeaponBase` 对象，指针和对象内容都可修改。
		*	- `AWarriorWeaponBase* const*`：指向“常量指针”的指针，可以修改指针本身的指向，但不能修改被指向的指针地址。
		*	- `const AWarriorWeaponBase*`：指向常量对象的指针，不能通过该指针修改对象内容，但可以修改指针本身的指向。
		*	- `AWarriorWeaponBase* const`：常量指针，指向 `AWarriorWeaponBase` 对象，指针地址不可变，但可以修改对象内容。
		 **/
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())	return nullptr;
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bEnableCollision, EToggleDamageType ToggleDamageType)
{
	UBoxComponent* BoxToToggle = nullptr;
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
		check(WeaponToToggle);
		BoxToToggle = WeaponToToggle->GetWeaponCollisionBox();
	}
	else if (ToggleDamageType == EToggleDamageType::LeftHand)
	{
		BoxToToggle = GetOwningPawn<AWarriorBaseCharacter>()->GetLeftHandCollisionBox();
	}
	else if (ToggleDamageType == EToggleDamageType::RightHand)
	{
		BoxToToggle = GetOwningPawn<AWarriorBaseCharacter>()->GetRightHandCollisionBox();
	}
	check(BoxToToggle);
	ToggleBoxCollsion(BoxToToggle, bEnableCollision);
}

void UPawnCombatComponent::ToggleBoxCollsion(UBoxComponent* Box, bool bEnableCollision)
{
	if (bEnableCollision)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	
	}
	else
	{
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
	}
}

void UPawnCombatComponent::OnWeaponHitTargetActor(AActor* TargetActor)
{
}

void UPawnCombatComponent::OnWeaponPullback(AActor* TargetActor)
{
}
