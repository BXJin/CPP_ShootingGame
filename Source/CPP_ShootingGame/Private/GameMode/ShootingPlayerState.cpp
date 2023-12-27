// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ����� ���� �߰�

// ������ ��Ģ�̶� ������Ͽ� ������ص���, Replicate������ �ݵ�� �������
void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, m_CurHP);
}

AShootingPlayerState::AShootingPlayerState() : m_CurHP(100)
{
}

void AShootingPlayerState::AddDamage(float Damage)
{
	m_CurHP = m_CurHP - Damage;

	//CPP������ OnRep�Լ��� �������� �ҷ�������Ѵ�
	OnRep_CurHP(); 
}

void AShootingPlayerState::OnRep_CurHP()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OnRep_CurHP = %f") , m_CurHP) );

	// ���ε尡 �ɷ��ִ���(IsBound) , Call -> Broadcast
	if(m_Dele_UpdateHP.IsBound())
		m_Dele_UpdateHP.Broadcast(m_CurHP, 100.0f);
}
