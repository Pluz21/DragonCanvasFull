//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class ABaseProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONCANVAS_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> projectileRef;
	UPROPERTY()
	TObjectPtr<AActor> ownerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Attack();
	ABaseProjectile* SpawnProjectile(const FVector& _spawnLocation, AActor* _owner);

		
};
