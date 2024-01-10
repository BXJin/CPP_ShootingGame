// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/EntryHUD.h"
#include "Blueprint/UserWidget.h"

void AEntryHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass); // 에러날거 같은 곳에 

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (IsValid(pPlayer0))
	{
		pPlayer0->SetShowMouseCursor(true);
		pPlayer0->SetInputMode(FInputModeUIOnly());
	}
}
