// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/CPP_ShootingGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 사용을 위해 추가
#include "GameMode/ShootingPlayerState.h"
#include "Blueprints/Weapon.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACPP_ShootingGameCharacter

ACPP_ShootingGameCharacter::ACPP_ShootingGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// 내부적 규칙이라 헤더파일에 선언안해도됨, Replicate변수는 반드시 해줘야함
void ACPP_ShootingGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_ShootingGameCharacter, ControlRot);
}

void ACPP_ShootingGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	NameTagWidget = CreateWidget<UUserWidget>(GetWorld(), NameTagClass);
	NameTagWidget->AddToViewport();
	
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_Nametag, this,
		&ACPP_ShootingGameCharacter::EventUpdateNametag, 0.01f, true);

	BindPlayerState();
}

void ACPP_ShootingGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 서버에만 동작하게
	if (HasAuthority() == true) 
	{
		ControlRot = GetControlRotation();
	}
}

float ACPP_ShootingGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		FString::Printf( TEXT("TakeDamage Damage=%f, EventInstigator=%s"),DamageAmount,*EventInstigator->GetName()));

	//총 맞으면 데미지 입음
	AShootingPlayerState* ps = Cast<AShootingPlayerState>(GetPlayerState());
	if (!IsValid(ps))
	{
		UE_LOG(LogTemp, Error, TEXT("Error"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("PS is not Valid"));
		return 0.0f;
	}
	ps->AddDamage(DamageAmount);

	return DamageAmount;
}

// NetWork CPP에는 Implementation
void ACPP_ShootingGameCharacter::ReqPressF_Implementation()
{
	// PrintString
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("ReqPressF"));
	AActor* pNearestActor = FindNearestWeapon();
	if (!IsValid(pNearestActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("NoActor"));
		return;
	}
	if (m_EquipWeapon != nullptr)
	{
		m_EquipWeapon->SetOwner(nullptr);
	}

	pNearestActor->SetOwner(GetController()); // 액터의 소유권이 없으면 클라이언트가 서버로 메시지를 보낼 수 없다
	ResPressF(pNearestActor);
}

void ACPP_ShootingGameCharacter::ResPressF_Implementation(AActor* PickUpActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("PickUpActor"));

	if (m_EquipWeapon != nullptr)
	{
		IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
		if (nullptr == InterfaceObj)
			return;
		InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
		m_EquipWeapon = nullptr;
	}

	m_EquipWeapon = PickUpActor;
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;
	InterfaceObj->Execute_EventPickUP(m_EquipWeapon, this);
}

void ACPP_ShootingGameCharacter::ResPressFClient_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("ResPressFClient"));
}

void ACPP_ShootingGameCharacter::ReqTrigger_Implementation()
{
	ResTrigger();
}

void ACPP_ShootingGameCharacter::ResTrigger_Implementation()
{
	//PlayAnimMontage(ShootMontage);
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;
	InterfaceObj->Execute_EventTrigger(m_EquipWeapon);
}

void ACPP_ShootingGameCharacter::ReqReload_Implementation()
{
	ResReload();
}

void ACPP_ShootingGameCharacter::ResReload_Implementation()
{
	//PlayAnimMontage(ReloadMontage);
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;
	InterfaceObj->Execute_EventReload(m_EquipWeapon);
	//UGameplayStatics::SpawnSound2D
}

void ACPP_ShootingGameCharacter::ReqDrop_Implementation()
{
	if (!IsValid(m_EquipWeapon)) return;
	m_EquipWeapon->SetOwner(nullptr);
	ResDrop();
}

void ACPP_ShootingGameCharacter::ResDrop_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;
	InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
	m_EquipWeapon = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACPP_ShootingGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// 블루프린트의 JumpAction에 접근하겄다, Start -> pressed, Completed -> released
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving 
		// Trigger는 틱처럼
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_ShootingGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_ShootingGameCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(TriggerAction, ETriggerEvent::Started, this, &ACPP_ShootingGameCharacter::Trigger);

		// PressF
		EnhancedInputComponent->BindAction(PressFAction, ETriggerEvent::Started, this, &ACPP_ShootingGameCharacter::PressF);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ACPP_ShootingGameCharacter::Reload);

		// Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ACPP_ShootingGameCharacter::Drop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACPP_ShootingGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACPP_ShootingGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPP_ShootingGameCharacter::Trigger(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Trigger"));
	ReqTrigger();
}

void ACPP_ShootingGameCharacter::PressF(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("PressF"));
	ReqPressF();
}

void ACPP_ShootingGameCharacter::Reload(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Reload"));
	ReqReload();
}

void ACPP_ShootingGameCharacter::Drop(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Drop G"));
	ReqDrop();
}


void ACPP_ShootingGameCharacter::EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass)
{
	if (!HasAuthority()) return;
	m_EquipWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	AWeapon* pWeapon = Cast<AWeapon>(m_EquipWeapon);
	if (!IsValid(pWeapon)) return;
	pWeapon->m_pOwnChar = this;
	TestWeaponSetOwner();
	m_EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void ACPP_ShootingGameCharacter::TestWeaponSetOwner()
{
	if (IsValid(GetController()))
	{
		m_EquipWeapon->SetOwner(GetController());
		return;
	}
	
	FTimerManager& tm = GetWorld()->GetTimerManager();
	tm.SetTimer(th_BindSetOwner, this, &ACPP_ShootingGameCharacter::TestWeaponSetOwner, 0.1f, false);
}

AActor* ACPP_ShootingGameCharacter::FindNearestWeapon()
{
	TArray<AActor*> actors;
	GetCapsuleComponent()->GetOverlappingActors(actors, AWeapon::StaticClass());

	double nearestDist = 999999999.0f;
	AActor* pNearestActor = nullptr;
	for (auto pTarget : actors)
	{
		if (m_EquipWeapon == pTarget) continue;
		double dist = FVector::Distance(GetActorLocation(), pTarget->GetActorLocation());
		if (dist >= nearestDist)
			continue;
		nearestDist = dist;
		pNearestActor = pTarget;
	}

	return pNearestActor;
}

void ACPP_ShootingGameCharacter::EventUpdateNametag_Implementation()
{
}

void ACPP_ShootingGameCharacter::EventUpdateNametagHp_Implementation(float CurHp, float MaxHp)
{
}

void ACPP_ShootingGameCharacter::BindPlayerState()
{
	AShootingPlayerState* pPS = Cast<AShootingPlayerState>(GetPlayerState());
	if (IsValid(pPS))
	{
		pPS->m_Dele_UpdateHP.AddDynamic(this, &ACPP_ShootingGameCharacter::EventUpdateNametagHp);
		EventUpdateNametagHp(pPS->m_CurHP, 100);
		return;
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_BindPlayerState, this, 
		&ACPP_ShootingGameCharacter::BindPlayerState, 0.01f, false);
}

void ACPP_ShootingGameCharacter::EventGetItem_Implementation(EItemType itemType)
{
	switch (itemType)
	{
	case EItemType::IT_Mag:
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("EventGetItem Mag"));
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(GetPlayerState());
		if (!IsValid(ps)) return;
		ps->AddMag();
	}
	break;

	case EItemType::IT_HEAL:
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("EventGetItem Heal"));
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(GetPlayerState());
		if (!IsValid(ps)) return;
		ps->AddHeal(100);
	}
	break;

	default:
		break;
	}
}
