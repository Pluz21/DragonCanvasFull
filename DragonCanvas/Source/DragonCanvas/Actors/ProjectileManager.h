//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager.generated.h"
class ABaseProjectile;
class ADragon;

UCLASS()
class DRAGONCANVAS_API AProjectileManager : public AActor
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaterialAcquired, UMaterialInterface*, mat);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatAlreadyInArrayEvent, UMaterialInterface*, mat);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FMaterialAcquired onMatAcquired;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FMatAlreadyInArrayEvent onMatAlreadyExists;

	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<ABaseProjectile*> allProjectiles;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ADragon> playerRef;
	
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> allCollectedMats;
	
public:	
	// Sets default values for this actor's properties
	AProjectileManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init();
	void AddItem(TObjectPtr<ABaseProjectile> _item);
	void RemoveItem(TObjectPtr<ABaseProjectile> _item);
	bool Exists(TObjectPtr<ABaseProjectile> _item);
	bool Exists(const int& _index);
	TObjectPtr<ABaseProjectile> GetItem(const int& _index); //we need the index. 
	TArray<ABaseProjectile*> GetAllProjectiles() { return allProjectiles; }
	int GetAllProjectilesSize() { return GetAllProjectiles().Num(); }

	void AddMaterial(TObjectPtr<UMaterialInterface> _mat);
	void RemoveMaterial(TObjectPtr<UMaterialInterface> _mat);
	bool MatExists(TObjectPtr <UMaterialInterface> _mat);
	bool MatExists(TObjectPtr<UMaterialInterface> _mat, TArray<UMaterialInterface*> _arrayToCheck);
	bool MatExists(const int& _index);
	TObjectPtr<UMaterialInterface> GetMaterial(const int& _index);
	TArray<UMaterialInterface*> GetAllCollectedMaterialInstances() { return allCollectedMats; }
	int GetAllCollectedMaterialInstancesSize() { return allCollectedMats.Num(); }

	UFUNCTION()
	FMaterialAcquired& GetOnMatAcquired() { return onMatAcquired; }
	UFUNCTION()
	FMatAlreadyInArrayEvent& GetOnMatAlreadyExists() { return onMatAlreadyExists; }
};
