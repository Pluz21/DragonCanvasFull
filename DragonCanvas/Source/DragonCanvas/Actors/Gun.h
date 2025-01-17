// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dragon.h"

#include "Gun.generated.h"

UCLASS()
class DRAGONCANVAS_API AGun : public AActor
{

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBarrelMaterialReceivedEvent);
	
	UPROPERTY(EditAnywhere)
	FBarrelMaterialReceivedEvent onBarrelMaterialReceived;
	
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> gunMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> gunSubMesh;
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> materialChangerMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ADragon> playerRef;
	UPROPERTY(EditAnywhere)
	bool canRotate = false;
	UPROPERTY(EditAnywhere)
	float rotateSpeed = 30;
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();
	void InitPlayerRef();
	void InitEvents();
	UFUNCTION()
	void UpdateBarrelMaterial(UMaterialInterface* _matToApply);
	TArray<UStaticMeshComponent*> GetAllChildStaticMeshes(UStaticMeshComponent* _parentMesh);
	UFUNCTION()
	void UpdateBarrelVisibility();
	UFUNCTION() 
	void SetCanRotate();
	virtual void Rotate(UStaticMeshComponent* _meshToRotate);

	UFUNCTION()
	virtual void GunSwap(AGun* _gunToUse);
};
