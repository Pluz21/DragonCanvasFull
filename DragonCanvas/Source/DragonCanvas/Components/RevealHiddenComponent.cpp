// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Components/RevealHiddenComponent.h"
#include "DragonCanvas/Actors/HiddenActors.h"
#include "Kismet/GameplayStatics.h"
#include "RevealHiddenComponent.h"

// Sets default values for this component's properties
URevealHiddenComponent::URevealHiddenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URevealHiddenComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}


// Called every frame
void URevealHiddenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URevealHiddenComponent::Init()
{
	onConditionToRevealIsMet.AddDynamic(this, &URevealHiddenComponent::RevealHidden);
	onReveal.AddDynamic(this, &URevealHiddenComponent::PlayRevealSound);
}



void URevealHiddenComponent::RevealHidden()
{
	UE_LOG(LogTemp, Warning, TEXT("REVEAL HIDDEN CALLED FROM WIHTIN COMPO"));
	if (allHiddenActors.Num() <= 0)return;
	int _size = allHiddenActors.Num();
	for (int i = 0; i < _size; i++)
	{
		if (allHiddenActors[i] == nullptr)continue;
		UStaticMeshComponent* _meshCompo = allHiddenActors[i]->
			GetComponentByClass<UStaticMeshComponent>();
		if (!_meshCompo) return;
		_meshCompo->SetVisibility(true, true);
		allHiddenActors[i]->GetComponentByClass<UStaticMeshComponent>()->
			SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		onReveal.Broadcast();
	}

	
}

void URevealHiddenComponent::PlayRevealSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(),revealSound);
}

