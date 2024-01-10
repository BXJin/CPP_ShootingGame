// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AMyBaseItem::AMyBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = m_Scene;
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_StaticMesh->SetupAttachment(m_Scene);
	m_StaticMesh->SetCollisionProfileName("OverlapAllDynamic");

	bReplicates = true;
	SetReplicateMovement(true);

	m_StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyBaseItem::OnItemBeginOverlap);
}

// Called when the game starts or when spawned
void AMyBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StaticMesh->AddRelativeRotation(FRotator(0, 1.0f, 0));
}

void AMyBaseItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IItemInterface* InterfaceObj = Cast<IItemInterface>(OtherActor);
	if (nullptr == InterfaceObj)
		return;
	InterfaceObj->Execute_EventGetItem(OtherActor, m_eItemType);
}


