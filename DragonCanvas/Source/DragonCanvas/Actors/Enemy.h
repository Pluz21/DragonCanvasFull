//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class UMoveComponent;
class UMaterialCheckerComponent;
class UAttackComponent;
class URevealHiddenComponent;

class ADragon;
class AItemPickUp;

UCLASS()
class DRAGONCANVAS_API AEnemy : public APawn
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestructionEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitEvent);
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackTimerEvent);
protected:
	UPROPERTY()
	FAttackTimerEvent onAttackTimerReset;
	UPROPERTY()
	FDestructionEvent onDestruction;
	UPROPERTY()
	FDeathEvent onDeath;
	UPROPERTY()
	FHitEvent onHit;
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> root;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> baseMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> secondMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> spawnPoint;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMoveComponent> moveCompo;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttackComponent> attackCompo;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialCheckerComponent> materialCheckerCompo;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URevealHiddenComponent> revealHiddenCompo;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> hitPlayerSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> onDeathSound;
protected:
	UPROPERTY(EditAnywhere,meta = (MetaClass = IsBlueprintBase = "true"))
	TArray<TSubclassOf<AItemPickUp>> allItemsToDrop;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ADragon> playerRef;
	
	UPROPERTY(EditAnywhere)
	float lifeSpan = 20;
	UPROPERTY(EditAnywhere)
	float minDistanceAllowed = 150.f;
	UPROPERTY(EditAnywhere)
	int damageToApply = 1;
	UPROPERTY(EditAnywhere)
	bool hasBeenhit = false;
	UPROPERTY(EditAnywhere)
	bool canDestroySecondMesh = false;
	// Timer
	UPROPERTY(EditAnywhere)
	float currentTime = 0;
	UPROPERTY(EditAnywhere)
	float maxTime = 1;
	UPROPERTY(EditAnywhere)
	bool canStartDestroytimer = false;

	//AI
	UPROPERTY(EditAnywhere)
	FVector spawnLocation = FVector(0);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init();
	void InitEvents();

	bool CheckDistance();

	UFUNCTION(BlueprintCallable)
	void SelfDestroy();
	UFUNCTION()
	virtual void SpawnItem(AActor* _actor);


	UFUNCTION(BlueprintCallable)
	void ApplyDamage();
	UFUNCTION(BlueprintCallable)
	void PlayHitPlayerSound(USoundBase* _audioToPlay);
	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* _audioToPlay);

	// Timer functions
	void StartDestroyTimer();
	float IncreaseTime(float _current, float _max);
	void SetCanStartDestroyTimer(bool _value);

	void SetHasBeenHit(bool _value) { hasBeenhit = _value; }

	FDeathEvent& GetOnDeath() { return onDeath; }
	FHitEvent& GetOnHit() { return onHit; }
	UFUNCTION()
	void ManageOnDeath();
	UFUNCTION()
	void PlayProjectileHitSound();

	UStaticMeshComponent* GetBaseMesh() { return baseMesh; }
	UStaticMeshComponent* GetSecondMesh() { return secondMesh; }
	void SetBaseMeshMaterial(UMaterialInterface* _newMat) { baseMesh->SetMaterial(0,_newMat) ; }
	UMaterialCheckerComponent* GetMaterialCheckerComponent() { return materialCheckerCompo; }

	virtual void SetSecondMeshMaterial(UMaterialInterface* _newMat) { secondMesh->SetMaterial(0, _newMat);};
	virtual void SetMeshMaterialAtIndex(TArray<UStaticMeshComponent*> _meshArray, int _index, UMaterialInterface* _newMat);
	virtual void SetMeshMaterialChildIncluded(TArray<UStaticMeshComponent*> _meshesToAffect, UMaterialInterface* _newMat);

	virtual void Chase();

	void SetSecondMeshCanBeDestroyed(bool _value) { canDestroySecondMesh = _value; }
	//bool GetCanDestroySecondMesh() { return canDestroySecondMesh; }
};
