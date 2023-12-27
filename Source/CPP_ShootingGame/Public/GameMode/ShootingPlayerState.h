// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

// �������Ʈ���� �� ����ó
// Dynamic �������Ʈ������ ȣ�Ⱑ��, Multicast �������� ���ÿ� ���ε� ����
// �������Ʈ���� ���� �Լ��� ���ε� ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP , float, MaxHP);

UCLASS()
class CPP_SHOOTINGGAME_API AShootingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShootingPlayerState();

public:
	UFUNCTION()
	void AddDamage(float Damage);

public:
	UFUNCTION()
	void OnRep_CurHP();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float m_CurHP;

	//�������Ʈ���� �Ҵ� �����ϰ�
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHP m_Dele_UpdateHP;
};
