// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/ShootingPlayerState.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass); // �������� ���� ���� 

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();

	BindMyPlayerState();
}

void AShootingHUD::BindMyPlayerState()
{
	// GetFirstPlayerController() -> Player 0��
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (IsValid(pc))
	{
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		if (IsValid(ps))
		{
			ps->m_Dele_UpdateHP.AddDynamic(this, &AShootingHUD::OnUpdateMyHP);
			OnUpdateMyHP(ps->m_CurHP, 100.0f);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("BindSuccess")));
			return;
		}
	}

	//BluePrint, Set timer by Function Name
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.01f, false);
}

void AShootingHUD::OnUpdateMyHP_Implementation(float CurHP, float MaxHP)
{
}
