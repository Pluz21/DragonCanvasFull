//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileTriggerComponent.generated.h"

class ADragon;
class ASnapManager;
class ACustomGameMode;
class ASpawner;
class AEnemy;
class AColorActivator; 
class AProjectileManager;

class URevealHiddenComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class DRAGONCANVAS_API UProjectileTriggerComponent : public UActorComponent
{

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSnapEvent);
	UPROPERTY()
	FSnapEvent onSnap;

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileTriggerComponent();
	UPROPERTY()
	TObjectPtr<ADragon> dragonRef; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> mat;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> targetMat;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ACustomGameMode> gameMode;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AProjectileManager> projectileManager;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<URevealHiddenComponent> revealHiddenCompo;


	UPROPERTY(EditAnywhere)
	TArray<AActor*> allSpawners;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> allVessels;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpawner> spawnerToFind;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AColorActivator> vesselToFind;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> snapSound;;

	UPROPERTY(EditAnywhere)
	bool canSpawn = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void SnapTarget(AActor* _targetActor);
	void Init();
	void InitGameMode();
	UFUNCTION(BlueprintCallable)
	bool MaterialChecker(AActor*& _targetToCheck);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMaterialInterface* GetMatInterface() { return mat; }

	UFUNCTION()
	FSnapEvent& OnSnap() { return onSnap; }
	UFUNCTION()
	void HandleSnap();
	UFUNCTION()
	void PlaySound();

	//Other interactions
	UFUNCTION()
	void HandleReveal();

};
