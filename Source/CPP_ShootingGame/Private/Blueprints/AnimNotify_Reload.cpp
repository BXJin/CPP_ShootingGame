// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/AnimNotify_Reload.h"
#include "GameMode/CPP_ShootingGameCharacter.h"
#include "Blueprints/WeaponInterface.h"

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	ACPP_ShootingGameCharacter* pChar = Cast< ACPP_ShootingGameCharacter>(MeshComp->GetOwner());
	if (!IsValid(pChar))
		return;
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(pChar->m_EquipWeapon);
	if (InterfaceObj == nullptr)
		return;
	InterfaceObj->Execute_EventResetAmmo(pChar->m_EquipWeapon);
}
