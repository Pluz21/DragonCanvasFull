// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UManaComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	// ...
	
}

void UManaComponent::Init()
{
	currentMana = maxMana;
}

// Called every frame
void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	IdleManaRegen();
	SetOutOfMana();
	// ...
}


void UManaComponent::IdleManaRegen()
{
	if (currentMana < maxMana)
	{
	currentMana += manaRegenRate * GetWorld()->DeltaTimeSeconds;
	}
}

void UManaComponent::AddMana(float _value)
{
	if (currentMana >= maxMana)return;
	currentMana += _value;
}

void UManaComponent::RemoveMana(float _value)
{
	currentMana -= _value;
}

void UManaComponent::SetOutOfMana()
{
	if (currentMana <= 1)
	{
		currentMana = currentMana;
		isOutOfMana = true;
	}
	else isOutOfMana = false;
	//isOutOfMana = false;
}



