// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Mag UMETA(DisplayName = "Mag"),
	IT_HEAL UMETA(DisplayName = "Heal"),
};

UINTERFACE(Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class BASEITEM_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);
};
