// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "MyBaseItem.generated.h"

UCLASS()
class BASEITEM_API AMyBaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USceneComponent* m_Scene;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EItemType m_eItemType;
};   
