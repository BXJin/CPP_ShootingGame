// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_SHOOTINGGAME_API AShootingHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();
	
public:
	void BindMyPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyHP(float CurHP, float MaxHP);

	void OnUpdateMyHP_Implementation(float CurHP, float MaxHP);

public:
	//TSubclassOf : ������ Ŭ������ Ŭ������ ����� ��� Ŭ������ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HudWidget;

	FTimerHandle th_BindMyPlayerState;
};
