//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "Enemy.h"
#include "DragonCanvas/Actors/Dragon.h"
#include "DragonCanvas/Actors/PlayerProjectile.h"

#include "DragonCanvas/Components/MoveComponent.h"
#include "DragonCanvas/Components/HealthComponent.h"
#include "DragonCanvas/Components/MaterialCheckerComponent.h"
#include "DragonCanvas/Components/AttackComponent.h"
#include "DragonCanvas/Actors/ItemPickUp.h"

#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"




// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>("root");
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("baseMesh");
	secondMesh = CreateDefaultSubobject<UStaticMeshComponent>("secondMesh");
	spawnPoint = CreateDefaultSubobject<USceneComponent>("spawnLocation");
	
	moveCompo = CreateDefaultSubobject<UMoveComponent>("moveCompo");
	attackCompo = CreateDefaultSubobject<UAttackComponent>("attackCompo");
	

	materialCheckerCompo = CreateDefaultSubobject<UMaterialCheckerComponent>("MaterialChecker");
	
	//hitPlayerSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Sounds/Enemy_Sounds/Enemy_Hit_Meta")).Object;
	//onDeathSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("Game/Sounds/Enemy_Sounds/Enemy_Breaking_Meta")).Object;
	spawnPoint->SetupAttachment(root);
	baseMesh->SetupAttachment(root);
	secondMesh->SetupAttachment(baseMesh);
	AddOwnedComponent(moveCompo);
	AddOwnedComponent(attackCompo);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Init();
}



void AEnemy::Init()
{
	InitEvents();
	SetLifeSpan(lifeSpan);  // TODO Make variable
	playerRef = moveCompo->GetChaseTarget();


	revealHiddenCompo = GetComponentByClass<URevealHiddenComponent>();
	
}

void AEnemy::InitEvents()
{

	onHit.AddDynamic(this, &AEnemy::PlayProjectileHitSound);
	onDeath.AddDynamic(this, &AEnemy::ManageOnDeath);
	OnDestroyed.AddDynamic(this, &AEnemy::SpawnItem);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SelfDestroy();
	currentTime = IncreaseTime(currentTime, maxTime);

	Chase();

}

bool AEnemy::CheckDistance()
{
	if (!playerRef || hasBeenhit == true)return false;
	float _distance = FVector::Dist(playerRef->GetActorLocation(), GetActorLocation());
	if(_distance < minDistanceAllowed)
		return true;
	return false;
}

void AEnemy::SelfDestroy()
{
	if (CheckDistance())
	{
		if (!playerRef)return;   // Enemy will join on top of you
		Destroy(); // Play Animation
		if (playerRef->healthCompo->isDead)return;
		ApplyDamage();
		PlayHitPlayerSound(hitPlayerSound);
		UE_LOG(LogTemp, Warning, TEXT("You got hit by an enemy!"));

	}
	// TO DO 
	// EnemyManager->RemoveEnemy(this);
	// Add explosion effect
	// Add health loss to player
}

void AEnemy::SpawnItem(AActor* _actor)
{
	if (allItemsToDrop.Num() <= 0)return;
	FVector _itemSpawnLocation = baseMesh->GetComponentLocation();
	FActorSpawnParameters _itemToDropParams;
	_itemToDropParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	int _size = allItemsToDrop.Num();
	for (int i = 0; i < _size; i++)
	{
		//find location of first [i] add 100 y and x to i +1 location
		AItemPickUp* _itemDropped = GetWorld()->SpawnActor<AItemPickUp>(allItemsToDrop[i],
			_itemSpawnLocation, FRotator::ZeroRotator, _itemToDropParams);
		FVector _itemLocation = _itemDropped->GetActorLocation();
		_itemSpawnLocation = _itemLocation + FVector(100, 0, 100);
		//allItemsToDrop.Remove(allItemsToDrop[i]);
	}

}

void AEnemy::ApplyDamage()
{
	playerRef->healthCompo->RemoveHealth(damageToApply);
}

void AEnemy::PlayHitPlayerSound(USoundBase* _audioToPlay)
{
	if (!_audioToPlay)return;
	UGameplayStatics::PlaySound2D(GetWorld(), _audioToPlay);

}

void AEnemy::PlaySound(USoundBase* _audioToPlay)
{
	if (!_audioToPlay || hasBeenhit)return;
	UGameplayStatics::PlaySound2D(GetWorld(), _audioToPlay);
}


float AEnemy::IncreaseTime(float _current, float _max)
{
	_current += GetWorld()->DeltaTimeSeconds;
	if (_current >= _max)
	{
		_current = 0;
		onAttackTimerReset.Broadcast();
		return _current;
	}

	return _current;
}

void AEnemy::SetCanStartDestroyTimer(bool _value)
{
	//canStartDestroytimer = _value;
}

void AEnemy::ManageOnDeath()
{
	PlaySound(onDeathSound);
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed!"));

}

void AEnemy::PlayProjectileHitSound()
{
	PlaySound(onDeathSound);

}

void AEnemy::SetMeshMaterialAtIndex(TArray<UStaticMeshComponent*> _meshArray, int _index, UMaterialInterface* _newMat)
{
	_meshArray[_index]->SetMaterial(0, _newMat);
}

void AEnemy::SetMeshMaterialChildIncluded(TArray<UStaticMeshComponent*> _meshesToAffect, UMaterialInterface* _newMat)
{
		int _size = _meshesToAffect.Num();
		for (int i = 0; i < _size; i++)
		{
			_meshesToAffect[i]->SetMaterial(0, _newMat);
		}
}

void AEnemy::Chase()
{
	if (!moveCompo)return;
	moveCompo->ChasePlayer();
}





