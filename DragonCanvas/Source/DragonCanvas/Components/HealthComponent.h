//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONCANVAS_API UHealthComponent : public UActorComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable)
	FDeathEvent onDeath;
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	//Health variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxHealth = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDead = false;

	// Colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor currentColor = FLinearColor(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor highHealthColor = FLinearColor(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor midHealthColor = FLinearColor(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor lowHealthColor = FLinearColor(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor criticalHealthColor = FLinearColor(0,0,0);

	// Health Fractions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "1.5"))
	float maxHealthFraction = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.6", ClampMax = "2.5"))
	float midHealthFraction = 1.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2.5", ClampMax = "3.5"))
	float lowHealthFraction = 3.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "3.6", ClampMax = "4.5"))
	float criticalHealthFraction = 4.2f;

	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Init();

	UFUNCTION(BlueprintCallable)
	void CheckDeathState();
	UFUNCTION(BlueprintCallable)
	void AddHealth(int _value);
	UFUNCTION(BlueprintCallable)
	void RemoveHealth(int _value);
	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth() { return currentHealth; }
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(int _value) { maxHealth = _value; }

	UFUNCTION(BlueprintCallable)
	void SetIsDead(bool _value);
	
	UFUNCTION(BlueprintCallable)
	void SetHealthColor();
	UFUNCTION(BlueprintCallable)
	void TestDeath();
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	UFUNCTION(BlueprintCallable)
	void SetMouseCursor();
};
