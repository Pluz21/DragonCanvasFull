//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "AttackComponent.h"
#include "DragonCanvas/Actors/PlayerProjectile.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::Attack()
{
}

ABaseProjectile* UAttackComponent::SpawnProjectile(const FVector& _spawnLocation,AActor* _owner)
{
	FActorSpawnParameters _projectileSpawnParams;
	_projectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABaseProjectile* _spawned = GetWorld()->
		SpawnActor<ABaseProjectile>(projectileRef,  _spawnLocation,
			FRotator::ZeroRotator, _projectileSpawnParams);
	if (_spawned && _owner)
	{
		_spawned->SetOwner(_owner);
		//UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile Owner: %s"), *_owner->GetName());

	}

	return _spawned;
}

