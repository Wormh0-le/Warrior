// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Components/UI/EnemyUIComponent.h"

#include "Widgets/WarriorWidget.h"


void UEnemyUIComponent::RegisterWidget(UWarriorWidget* InWidgetToRegister)
{
	EnemyOwnedWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::UnregisterEnemyOwnedWidgets()
{
	if (EnemyOwnedWidgets.IsEmpty())	return;
	for (UWarriorWidget* Widget : EnemyOwnedWidgets)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}
	EnemyOwnedWidgets.Empty();
}
