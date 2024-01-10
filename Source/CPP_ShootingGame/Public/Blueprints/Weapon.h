// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParams(FDele_UpdateAmmo, int, Ammo);

UCLASS()
class CPP_SHOOTINGGAME_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	virtual void EventTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShot();

	virtual void EventShot_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	virtual void EventReload_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUP(ACharacter* pOwnChar);
	virtual void EventPickUP_Implementation(ACharacter* pOwnChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);

	virtual void EventDrop_Implementation(ACharacter* pOwnChar) override;

public:
	UFUNCTION(Server, Reliable)
	void ReqShoot(FVector vStart, FVector vEnd);

public:
	float GetFireStartLength();

	UFUNCTION(BlueprintPure)
	bool IsCanShoot();

	bool UseAmmo();

	void SetAmmo(int Ammo);

	UFUNCTION(BlueprintCallable)
	void OnUpdateAmmoToHud(int Ammo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadWrite)
	ACharacter* m_pOwnChar;

	//EditAnywhere Detail창에서 수정가능, BlueprintReadWrite (Get, Set) 가능 ReadOnly (Get) 만
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_ShootMontage;

	//EditAnywhere Detail창에서 수정가능, BlueprintReadWrite (Get, Set) 가능 ReadOnly (Get) 만
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* m_FireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_SoundBase;

	UFUNCTION()
	void OnRep_Ammo();

	UPROPERTY(ReplicatedUsing = OnRep_Ammo)
	int m_Ammo;
};
