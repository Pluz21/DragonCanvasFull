//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.h"
#include "PlayerProjectile.generated.h"

class AProjectileManager;
class ACustomGameMode;
class ADragon;
class UMoveComponent;
UCLASS()
class DRAGONCANVAS_API APlayerProjectile : public ABaseProjectile
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetReached);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetAcquired);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileCreated);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCanStartMoving);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyHitEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCorrectProjectileMeshOverlapEvent, AActor*, actor);
	
	UPROPERTY(EditAnywhere)
	FEnemyHitEvent onEnemyHit;
	UPROPERTY(EditAnywhere)
	FCanStartMoving onCanMove;

	UPROPERTY(EditAnywhere)
	FTargetAcquired onTargetAcquired;

	UPROPERTY(EditAnywhere)
	FTargetReached onTargetReached;

	UPROPERTY(EditAnywhere)
	FProjectileCreated onProjectileCreated;
	UPROPERTY(EditAnywhere)
	FCorrectProjectileMeshOverlapEvent onCorrectProjectileMeshOverlap;

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerProjectile();
	
	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMoveComponent> moveCompo;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> initialMat;

	UPROPERTY(VisibleAnywhere)
	UWorld* myWorld;
	UPROPERTY(VisibleAnywhere)
	float deltaSeconds;
	

	UPROPERTY(EditAnywhere)
	float lifeSpan = 2;
	UPROPERTY(EditAnywhere)
	float enemyLifeSpan = 4;
	UPROPERTY(EditAnywhere)
	float initialImpulseSpeed = 10000;
	UPROPERTY(EditAnywhere)
	float impulseSpeed = 2000;
	UPROPERTY(VisibleAnywhere)
	float moveSpeed;
	UPROPERTY(VisibleAnywhere)
	bool canMove = false;
	UPROPERTY(VisibleAnywhere)
	bool canActivateLineTraceEffect = false;
	UPROPERTY(VisibleAnywhere)
	float launchTime;
	UPROPERTY(VisibleAnywhere)
	FVector forwardVector = FVector(0);
	UPROPERTY(EditAnywhere)
	FVector targetLocation = FVector(0);
	UPROPERTY(VisibleAnywhere)
	FVector actorSpawnLocation = FVector(0);

	UPROPERTY(EditAnywhere)
	float maxDistance = 10000;
	
	UPROPERTY(EditAnywhere)
	float lineTraceDistance;
	UPROPERTY(EditAnywhere)
	float minDistanceToSelfDestruct = 150; // equal to radius of our linetrace/2
	
	UPROPERTY(EditAnywhere)
	float targetDistanceMultiplier = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();
	void EventsInit();
	UFUNCTION()
	void ManageOverlap(AActor* _overlapped, AActor* _overlap);
	void SelfMove();
	void SelfMove(const FVector& _actorForwardVector);
	void SetCanMove(bool _value);
	void SetForwardVector(const FVector& _ownerVector) { forwardVector = _ownerVector; }
	void SetTargetLocation(const FVector& _newTargetLocation) { targetLocation = _newTargetLocation; }
	void SetImpulseSpeed(float _newImpulseSpeed) { impulseSpeed = _newImpulseSpeed; }
	
	//void SetTargetLocation(const FVector& _targetLocation) { targetLocation = _targetLocation; }
	UMoveComponent* GetMoveCompo() { return moveCompo; }
	FVector GetInitialLocation() { return actorSpawnLocation; }
	float GetLaunchTime() { return launchTime; }
	UStaticMeshComponent* GetMeshCompo() { return meshCompo; }

	UFUNCTION() void SetLaunchTime() {launchTime = GetWorld()->GetTimeSeconds();}
	UFUNCTION() void SetMaxDistance(const float& _maxDistance) { maxDistance = _maxDistance; }
	UFUNCTION() void FindEndLocation();
	UFUNCTION() void CheckDistance(FVector& _targetLocation);
	UFUNCTION() void CallLineTraceDisplacement();

	TArray<UStaticMeshComponent*> FindAllChildMeshes(UStaticMeshComponent*& _parentMesh);
	TArray<UStaticMeshComponent*> FindAllChildMeshes(UStaticMeshComponent*& _parentMesh, UStaticMeshComponent*& _parentMesh2, UStaticMeshComponent*& _parentMesh3);
	UMaterialInterface* GetCurrentProjectileMat() { return meshCompo->GetMaterial(0); }
	/**
	* Sets simulate physics to true and Mass to 0.01f kg
	*/
	UFUNCTION()
	void ApplyHitEffect(TArray<UStaticMeshComponent*> _allStaticMeshesToHit);
	void ApplyHitEffect(UStaticMeshComponent* _staticMeshToAffect);
	void UpdateOverlapPhysics(AActor*& _actorToActivatePhysicsOn);
	UFUNCTION()
	void ManageBossEnemyHit(AActor* _actor);
	void ManageEnemyHit(AActor* _actor);
	void ManageDestroyTagHit(AActor* _actor);
	void ManageCanMoveHit(AActor* _actor);

	
	//Events
	UFUNCTION()
	FCorrectProjectileMeshOverlapEvent& GetOnCorrectProjectileMeshOverlap() {
		return onCorrectProjectileMeshOverlap;}
};
