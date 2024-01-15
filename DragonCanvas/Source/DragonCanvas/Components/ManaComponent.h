//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONCANVAS_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100"))
	float maxMana = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentMana = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float manaRegenRate = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float projectileManaCost = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isOutOfMana = false;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();

	UFUNCTION(BlueprintCallable)
	void IdleManaRegen();
	UFUNCTION(BlueprintCallable)
	void AddMana(float _value);
	UFUNCTION(BlueprintCallable)
	void RemoveMana(float _value);
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentMana() { return currentMana; }
	UFUNCTION(BlueprintCallable)
	float GetManaRegenRate() { return manaRegenRate; }
	UFUNCTION(BlueprintCallable)
	void SetManaRegenRate(float _newRegenRate) { manaRegenRate = _newRegenRate; }
	UFUNCTION(BlueprintCallable)
	void SetProjectileManaCost(float _manaCost) { projectileManaCost = _manaCost; }
	UFUNCTION(BlueprintCallable)
	void SetOutOfMana();

};
