//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "HealthComponent.h"
#include "DragonCanvas/Actors/Dragon.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::Init()
{
	currentHealth = maxHealth;
	onDeath.AddDynamic(this, &UHealthComponent::TestDeath);
	onDeath.AddDynamic(this, &UHealthComponent::HandleDeath);
	SetHealthColor();
}

void UHealthComponent::CheckDeathState()
{
	if (currentHealth <= 0)
	{
		SetHealthColor();   
		SetIsDead(true);


		return;
	}
}

void UHealthComponent::AddHealth(int _value)
{

	if (currentHealth >= maxHealth)return;
	currentHealth += _value;
	SetHealthColor();
	UE_LOG(LogTemp, Error, TEXT("Current Health = %i"), currentHealth);
	CheckDeathState();

	
}

void UHealthComponent::RemoveHealth(int _value)
{
	currentHealth -= _value;
	SetHealthColor();
	CheckDeathState();

}

void UHealthComponent::SetIsDead(bool _value)
{

		isDead = _value;
		onDeath.Broadcast();
		UE_LOG(LogTemp, Error, TEXT("You are dead"));


}

void UHealthComponent::SetHealthColor()
{

	if (currentHealth == maxHealth)
	{
		UE_LOG(LogTemp, Error, TEXT("Called HealthColor INIT"));

		currentColor = highHealthColor;
	}

	float maxHealthThreshHold = maxHealth / maxHealthFraction;
	float midHealthThreshHold = maxHealth / midHealthFraction;
	float lowHealthThreshHold = maxHealth / lowHealthFraction;
	float criticalHealthThreshHold = maxHealth / criticalHealthFraction;
	UE_LOG(LogTemp, Error, TEXT("MaxHealththreshold = %f"), maxHealthThreshHold);
	UE_LOG(LogTemp, Error, TEXT("MidHealththreshold = %f"), midHealthThreshHold);
	UE_LOG(LogTemp, Error, TEXT("LowHealththreshold = %f"), lowHealthThreshHold);
	UE_LOG(LogTemp, Error, TEXT("CriticalHealththreshold = %f"), criticalHealthFraction);

	if (currentHealth >= maxHealthThreshHold)
		currentColor = highHealthColor;

	if (currentHealth >= midHealthThreshHold && currentHealth <= maxHealthThreshHold)
		currentColor = midHealthColor;
	
	if (currentHealth >= lowHealthThreshHold && currentHealth <= midHealthThreshHold)
		currentColor = lowHealthColor;
	
	if (currentHealth >= criticalHealthFraction && currentHealth <= lowHealthThreshHold)
		currentColor = criticalHealthColor;  
}

void UHealthComponent::TestDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Triggered Death Event : %d"), onDeath.IsBound());

}

void UHealthComponent::HandleDeath()
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("No owner FOUND IN HEALTHCOMPO"));
		return;
	}
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller)
	{
		UE_LOG(LogTemp, Error, TEXT("No CONTROLLER found in HealthCompo"));

		return;
	}
	ADragon* _playerRef = Cast<ADragon>(GetOwner());
	_playerRef->SetCanUseMoveInputs(false);
	//GetOwner()->DisableInput(nullptr);
	UE_LOG(LogTemp, Error, TEXT("Disable inputs"));

}

void UHealthComponent::SetMouseCursor()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameAndUI _inputMode;
	_inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	_controller->SetInputMode(_inputMode);
}


