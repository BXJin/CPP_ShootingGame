// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

// 블루프린트에서 한 디스패처
// Dynamic 블루프린트에서도 호출가능, Multicast 여러개를 동시에 바인드 가능
// 블루프린트에서 만든 함수도 바인드 가능
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP , float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateMag, int, Mag);

UCLASS()
class CPP_SHOOTINGGAME_API AShootingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShootingPlayerState();

public:
	UFUNCTION()
	void AddDamage(float Damage);

	UFUNCTION()
	void AddHeal(float Heal);

	UFUNCTION()
	void AddMag();

	UFUNCTION()
	void UseMag();

public:
	UFUNCTION()
	void OnRep_CurHP();

	UFUNCTION()
	void OnRep_Mag();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float m_CurHP;

	//블루프린트에서 할당 가능하게
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHP m_Dele_UpdateHP;

	UPROPERTY(ReplicatedUsing = OnRep_Mag)
	int m_Mag;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateMag m_Dele_UpdateMag;

};
