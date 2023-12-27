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
	//UPROPERTY 언리얼 내에서 사용할 수 있는 프로퍼티를 만들겠다, 빈칸이 생김 틀 생김
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressFAction;

	/** PressF Input Action */
	//UPROPERTY 언리얼 내에서 사용할 수 있는 프로퍼티를 만들겠다, 빈칸이 생김 틀 생김
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
	//네트워크 코드 영역
	UFUNCTION(Server, Reliable) //Run On Server
	void ReqPressF();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, 모든 클라이언트에 전달
	void ResPressF(AActor* PickUpActor);

	UFUNCTION(Client, Reliable) // MultiCast, 모든 클라이언트에 전달
	void ResPressFClient();

	UFUNCTION(Server, Reliable) //Run On Server
	void ReqTrigger();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, 모든 클라이언트에 전달
	void ResTrigger();

	UFUNCTION(Server, Reliable) //Run On Server
	void ReqReload();

	UFUNCTION(NetMulticast, Reliable) // MultiCast, 모든 클라이언트에 전달
	void ResReload();

public:
	UFUNCTION(BlueprintCallable)
	void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION()
	void TestWeaponSetOwner();

	AActor* FindNearestWeapon();

public :
	//변수의 Replicated
	UPROPERTY(Replicated, BlueprintReadWrite)
	FRotator ControlRot;
	//multicast는 서버에서 클라이언트에게 알려준다,함수
	//replicate는 서버의 값과 클라이언트의 값을 동일시 한다,변수
	//Weapon 장착
	UPROPERTY(BlueprintReadWrite)
	AActor* m_EquipWeapon;

	//임시
	FTimerHandle th_BindSetOwner;
};



