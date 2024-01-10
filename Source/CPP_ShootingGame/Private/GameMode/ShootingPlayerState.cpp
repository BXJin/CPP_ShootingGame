// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ����� ���� �߰�
#include "GameMode/ShootingHUD.h"

// ������ ��Ģ�̶� ������Ͽ� ������ص���, Replicate������ �ݵ�� �������
void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, m_CurHP);
	DOREPLIFETIME(AShootingPlayerState, m_Mag);
}

AShootingPlayerState::AShootingPlayerState() : m_CurHP(100), m_Mag(0)
{
}

void AShootingPlayerState::AddDamage(float Damage)
{
	m_CurHP = m_CurHP - Damage;

	//CPP������ OnRep�Լ��� �������� �ҷ�������Ѵ�
	OnRep_CurHP(); 
}

void AShootingPlayerState::AddHeal(float Heal)
{
	m_CurHP = m_CurHP + Heal;
	m_CurHP = FMath::Clamp(m_CurHP, 0, 100);
	OnRep_CurHP();
}

void AShootingPlayerState::AddMag()
{
	m_Mag = m_Mag + 1;
	OnRep_Mag();
}

void AShootingPlayerState::UseMag()
{
}

void AShootingPlayerState::OnRep_CurHP()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OnRep_CurHP = %f") , m_CurHP) );

	// ���ε尡 �ɷ��ִ���(IsBound) , Call -> Broadcast
	if(m_Dele_UpdateHP.IsBound())
		m_Dele_UpdateHP.Broadcast(m_CurHP, 100.0f);
}

void AShootingPlayerState::OnRep_Mag()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("OnRep_Mag = %d"), m_Mag));
	if (m_Dele_UpdateMag.IsBound())
		m_Dele_UpdateMag.Broadcast(m_Mag);

	//APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();

	//AShootingHUD* pHud = Cast<AShootingHUD>(pPlayer0->GetHUD());
	//if (pHud == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NoHud"));
	//	return;
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnRep_Mag = d"));
	//pHud->OnUpdateMyMag(m_Mag);
}
