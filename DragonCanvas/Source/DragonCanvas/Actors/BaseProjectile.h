// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "BaseProjectile.generated.h"

class ACustomGameMode;
class AProjectileManager;

UCLASS()
class DRAGONCANVAS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> meshCompo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> sphereCollider;


	UPROPERTY(EditAnywhere)
	TObjectPtr<ACustomGameMode> gameMode;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AProjectileManager> projectileManager;

	UPROPERTY(EditAnywhere)
	int damage = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init();
	UStaticMeshComponent* GetStaticMeshComponent() { if (!meshCompo)return nullptr; return meshCompo; }
	void SetStaticMeshComponent(UStaticMeshComponent* _mesh) { meshCompo = _mesh; }
	void SetSimulatePhysics();
	UFUNCTION() void SelfDestruct();

};
