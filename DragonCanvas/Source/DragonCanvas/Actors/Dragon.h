//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Dragon.generated.h"

class ACustomGameMode;
class UGrabber;
class UHealthComponent;
class UManaComponent;
class UAttackComponent;
class UUpgradeComponent;

class AGun;
class AProjectileManager;
class APlayerProjectile;

UCLASS()
class DRAGONCANVAS_API ADragon : public ACharacter
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLineTraceCreated);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileShotEvent);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileReachedTarget, UWorld*, world, FHitResult, result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileReachedTarget);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentProjectileMatEvent, UMaterialInterface*, mat);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenMenuEvent);

	UPROPERTY(BlueprintAssignable)
	FCurrentProjectileMatEvent onCurrentProjectileMatReceived;

	UPROPERTY()
	FProjectileShotEvent onProjectileShot;

	UPROPERTY()
	FLineTraceCreated onLineTraceCreated;
	
	UPROPERTY()
	FProjectileReachedTarget onProjectileTargetReached;
	
	UPROPERTY(BlueprintAssignable)
	FOpenMenuEvent onMenuOpened;

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADragon();
	UPROPERTY()
	TObjectPtr<UWorld> world;

	UPROPERTY()
	TObjectPtr<ACustomGameMode> gameMode;
	UPROPERTY()
	TObjectPtr<AProjectileManager> projectileManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> projectileSpawnPoint;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> gunSpawnPoint;
	UPROPERTY()
	TObjectPtr<APlayerController> playerController;

#pragma region Components

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> springArm;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> camera;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UUpgradeComponent> upgradeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackComponent> attackCompo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UGrabber> grabber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UPhysicsHandleComponent> physicsHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> healthCompo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UManaComponent> manaCompo;

#pragma endregion Components 


	
	UPROPERTY(EditAnywhere)
	FHitResult hitResult;


#pragma region Inputs
	// inputs Movement

	UPROPERTY(EditAnywhere, Category = "Inputs")
	bool canUseMoveInputs = true;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> mappingContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputMappingContext> menuMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToMove;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToRotateYaw;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToPitch;
	// inputs Movement
	// inputs Interactions
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToAction;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToGrab;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToJump;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToScrollUpSelectProjectile;
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToScrollDownSelectProjectile;
	// inputs Interactions
	// inputs Menu
	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UInputAction> inputToOpenMenu;
	
	// inputs Menu

	//input variables

	UPROPERTY(EditAnywhere)
	float moveSpeed = 500.f;
	UPROPERTY(EditAnywhere)
	float rotateSpeed = 20.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	float forwardInputValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	float rightInputValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	float rotateInputValue;

#pragma endregion Inputs

#pragma region Spawn
	// spawn variables
	UPROPERTY(EditAnywhere, Category = "ProjectileSelection")
	TSubclassOf<APlayerProjectile> projectileToSpawn;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileSelection")
	TObjectPtr<UMaterialInterface> currentProjectileMat;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ProjectileSelection")
	TArray<UMaterialInterface*> allProjectileMats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileSelection")
	int currentProjectileIndex = 0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector spawnPointLocation;

	UPROPERTY(VisibleAnywhere)
	FVector spawnedInitialLocation;

	// Gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGun> baseGunToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGun> laserGunToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGun> baseGunRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGun> laserGunRef;

#pragma endregion Spawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "10", ClampMin = "10", UIMax = "100", ClampMax = "100"))
	float maxAmmo = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackSpeed = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxTimeAttackSpeed = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentTimeAttackSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canAttack = true;

	UPROPERTY(EditAnywhere, Category = "LineTrace")
	FVector targetLocation;
	UPROPERTY(EditAnywhere, Category = "LineTrace")
	FVector locationOnLineTraceSpawn;

	//camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Camera Settings")
	float minPitchRotation = -65;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Camera Settings")
	float maxPitchRotation = 40;


	//Debug
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> _coneTraceChannel;


	// Audio
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> projectileSound;

	//Not in use
	UPROPERTY(EditAnywhere, Category = "LineTrace")
	float lineTraceEffectMultiplier = 200;
	UPROPERTY(EditAnywhere, Category = "LineTrace")
	float sphereTracedistance = 3000;

	UPROPERTY(EditAnywhere, Category = "LineTrace")
	float coneTraceRadius = 50;

	UPROPERTY(EditAnywhere)
	float minDistanceToSelfDestruct;




protected:
	virtual void BeginPlay() override;
	//Debug
	void DebugText(const FString& _string, const float& _floatToDebug = 0);

	void Init();
	void InitManagers();
	void InitInput();
	void InitEvents();
	void InitGuns();
	UFUNCTION()
	void EmplaceMatInList(UMaterialInterface* _mat);
	UFUNCTION()
	void UpdateProjectileMaterial(int _allProjectileMatsIndexToUpdate);
	//Inputs
	void Move(const FInputActionValue& _value);
	void RotateYaw(const FInputActionValue& _value);
	void RotatePitch(const FInputActionValue& _value);
	void Action();
	void Grab(const FInputActionValue& _value);
	void ScrollUpSelectProjectile();
	void ScrollDownSelectProjectile();
	void Jump() override;

	// Interface 
	void SetMouseFocusOnGame();
	void OpenMainMenu();
	//Dragon actions
	//Debug
	void SphereTrace();
	// attack compo needs

	//Projectile
	float GetCurrentAmmo() { return currentAmmo; }


	// Audio
	void PlaySound(USoundBase* _audioToPlay);
	UFUNCTION()
	void PlayProjectileSound();

public:

	void SetProjectileSound(USoundBase* _newSound) { projectileSound = _newSound; }
	UFUNCTION() void FireBreath();
	
	UFUNCTION() void AdjustProjectileSpeed(UStaticMeshComponent* _projectileMeshToAdjust);
	FVector  GetSpawnLocation() { return projectileSpawnPoint->GetComponentLocation(); }
	FVector GetProjectileTargetLocation() { return targetLocation; }
	float GetSphereTraceDistance() { return sphereTracedistance; }
	UHealthComponent* GetHealthComponent() { return healthCompo; }
	float GetMinDistanceToSelfDestruct() { return minDistanceToSelfDestruct; }
	void SetCanUseMoveInputs(bool _value) { canUseMoveInputs = _value; }
	UFUNCTION(BlueprintCallable) 
	AProjectileManager* GetProjectileManager() { return projectileManager; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<APlayerProjectile>GetProjectileToSpawn() { return projectileToSpawn; }

	UFUNCTION() void LineTraceDisplacement(UWorld* _world, const FHitResult& _hitResult);
	UFUNCTION() void StartLineTraceAction();

	// Event getters
	UFUNCTION()
	FProjectileReachedTarget& GetOnProjectileReachedTarget() { return onProjectileTargetReached; }
	UFUNCTION()
	FCurrentProjectileMatEvent& GetOnCurrentProjectileMatReceived() { return onCurrentProjectileMatReceived; }
	UFUNCTION()
	FOpenMenuEvent& GetOnMenuOpened() { return onMenuOpened; }
	//UFUNCTION()
	//void BroadcastOnProjectileMatUpdate() { onCurrentProjectileMatReceived.Broadcast(); }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentProjectileIndex() { return currentProjectileIndex; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UMaterialInterface*> GetAllProjectileMats() { return allProjectileMats; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAllProjectileMatsSize() { return allProjectileMats.Num(); };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetSpawnProjectileSocketLocation() { return projectileSpawnPoint->GetSocketLocation("projectileSpawnPoint"); }

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UpdateMinDistanceToSelfDestruct();

	//void SetStartAlphaCount();
	void SetAttackSpeed(float _newAttackSpeed) { attackSpeed = _newAttackSpeed; }
	void IncreaseAttackRateTime();

	// Gun functions
	void AttachGun(AGun* _gunToAttach, USceneComponent* _attachPoint);
	

};
