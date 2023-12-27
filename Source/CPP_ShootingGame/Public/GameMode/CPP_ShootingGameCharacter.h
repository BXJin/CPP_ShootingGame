// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CPP_ShootingGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACPP_ShootingGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TriggerAction;
	
	/** PressF Input Action */
	//UPROPERTY �𸮾� ������ ����� �� �ִ� ������Ƽ�� ����ڴ�, ��ĭ�� ���� Ʋ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressFAction;

	/** PressF Input Action */
	//UPROPERTY �𸮾� ������ ����� �� �ִ� ������Ƽ�� ����ڴ�, ��ĭ�� ���� Ʋ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

public:
	ACPP_ShootingGameCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for Shoot input */
	void Trigger(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressF(const FInputActionValue& Value);

	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	//��Ʈ��ũ �ڵ� ����
	UFUNCTION(Server, Reliable) //Run On Server
	void ReqPressF();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, ��� Ŭ���̾�Ʈ�� ����
	void ResPressF(AActor* PickUpActor);

	UFUNCTION(Client, Reliable) // MultiCast, ��� Ŭ���̾�Ʈ�� ����
	void ResPressFClient();

	UFUNCTION(Server, Reliable) //Run On Server
	void ReqTrigger();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, ��� Ŭ���̾�Ʈ�� ����
	void ResTrigger();

	UFUNCTION(Server, Reliable) //Run On Server
	void ReqReload();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, ��� Ŭ���̾�Ʈ�� ����
	void ResReload();

public:
	UFUNCTION(BlueprintCallable)
	void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION()
	void TestWeaponSetOwner();

	AActor* FindNearestWeapon();

public :
	//������ Replicated
	UPROPERTY(Replicated, BlueprintReadWrite)
	FRotator ControlRot;
	//multicast�� �������� Ŭ���̾�Ʈ���� �˷��ش�,�Լ�
	//replicate�� ������ ���� Ŭ���̾�Ʈ�� ���� ���Ͻ� �Ѵ�,����
	//Weapon ����
	UPROPERTY(BlueprintReadWrite)
	AActor* m_EquipWeapon;

	//�ӽ�
	FTimerHandle th_BindSetOwner;
};



