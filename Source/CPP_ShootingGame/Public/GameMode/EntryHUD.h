// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EntryHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_SHOOTINGGAME_API AEntryHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();

public:
	//TSubclassOf : 지정한 클래스와 클래스를 상속한 모든 클래스를 지정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HudWidget;
};
