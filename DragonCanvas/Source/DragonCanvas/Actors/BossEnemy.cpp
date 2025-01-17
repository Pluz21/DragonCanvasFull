// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "BossEnemy.h"
#include "DragonCanvas/Actors/BaseProjectile.h"

#include "DragonCanvas/Actors/ProjectileBossEnemy.h"
#include "DragonCanvas/Actors/PlayerProjectile.h"
#include "DragonCanvas/Components/MaterialCheckerComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

#include "DragonCanvas/Actors/HealthPickUp.h"
#include "DragonCanvas/Components/MoveComponent.h"
#include "DragonCanvas/Components/RevealHiddenComponent.h"
#include "DragonCanvas/Components/AttackComponent.h"

ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	thirdMesh = CreateDefaultSubobject<UStaticMeshComponent>("thirdMesh");
	fourthMesh = CreateDefaultSubobject<UStaticMeshComponent>("fourthMesh");
	fifthMesh = CreateDefaultSubobject<UStaticMeshComponent>("fifthMesh");
	faceMesh = CreateDefaultSubobject<UStaticMeshComponent>("faceMesh");
	deathMesh = CreateDefaultSubobject<UStaticMeshComponent>("deathMesh");
	
	pawnSensor = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensor");

	baseMesh->SetupAttachment(root);
	secondMesh->SetupAttachment(root);
	thirdMesh->SetupAttachment(root);
	fourthMesh->SetupAttachment(root);
	fifthMesh->SetupAttachment(root);
	faceMesh->SetupAttachment(root);
	deathMesh->SetupAttachment(root);

	AddOwnedComponent(pawnSensor);

}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BossEnemyGameLogic();
	// Assuming pawnSensor->SightRadius is the length of the cone
	float _coneLength = pawnSensor->SightRadius;

	// Calculate the end position of the cone
	FVector _coneEnd = GetActorLocation() + GetActorForwardVector() * _coneLength;

	// DrawDebugCone
	DrawDebugCone(GetWorld(), GetActorLocation(), GetActorForwardVector(), _coneLength, FMath::DegreesToRadians(30), FMath::DegreesToRadians(60), 12, FColor::Red);

	// DrawDebugCircle at the end of the cone
	DrawDebugSphere(GetWorld(), _coneEnd, 100, 20, FColor::Green, false, -1, 0, 1);


}

void ABossEnemy::Init()
{
	secondMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnSecondMeshOverlapBegin);
	thirdMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnThirdMeshOverlapBegin);
	fourthMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnFourthMeshOverlapBegin);
	fifthMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnFifthMeshOverlapBegin);

	pawnSensor->OnSeePawn.AddDynamic(this, &ABossEnemy::SetTarget);
	onAggro.AddDynamic(this, &ABossEnemy::PlayAggroSound);
	onEnterAttackRange.AddDynamic(this, &ABossEnemy::SetCanAttack);
	onEnterMinRange.AddDynamic(this, &ABossEnemy::StopMove);
	onAttackTimerReset.AddDynamic(this, &ABossEnemy::Attack);

	onMeshLoss.AddDynamic(this, &ABossEnemy::PlayMeshLossSound);
	onMeshLoss.AddDynamic(this, &ABossEnemy::UpdateSpawnPointLocation);
	onAllMeshesLost.AddDynamic(this, &ABossEnemy::ManageDeathEvent);
	onAllMeshesLost.AddDynamic(this, &ABossEnemy::ApplyDeathMeshes);

	OnDestroyed.AddDynamic(this, &ABossEnemy::ManageDestructionEvent);
	
	onDestruction.AddDynamic(this, &ABossEnemy::HandleReveal); // rather use my own delegate

	initialLocation = GetActorLocation();
	//SetInitialRotation();
	OverlapEventManager();  // AddDynamic overlap to all meshes
	SetAllMeshes();
	SetLifeSpan(500); 
	InitAI();
}



void ABossEnemy::InitAI()
{
	bossAIController = Cast<AAIController>(GetController());

}
void ABossEnemy::SetInitialRotation()
{
	initialRotation = GetActorRotation();
	SetActorRotation(initialRotation);
}
void ABossEnemy::BossEnemyGameLogic()
{
	CheckAllMeshes();
	AIBehaviourLogic();
}

void ABossEnemy::CheckAllMeshes()
{
	if (allMeshes.Num() <= 0)
	{
		onAllMeshesLost.Broadcast();

	}

}
void ABossEnemy::AIBehaviourLogic()
{
	if (target)
	{
		ChasePlayer();
		moveCompo->SetChaseSpeed(0.2f);
	}
	if(!target)
	{
		ReturnToInitialLocation();

	}
	if (target && canAttack)
	{
		//StopMove();
	}
	Detect();
	CheckReturnedToInitialLocation();
	RotateToPlayer();
	CheckIsInRangeToAttack();
	CheckMinRange();
	
}


void ABossEnemy::SetAllMeshes()
{
	allMeshes.Add(secondMesh); allMeshes.Add(thirdMesh); allMeshes.Add(fourthMesh); allMeshes.Add(fifthMesh);

}

void ABossEnemy::RemoveMesh(int _index)
{
	allMeshes.RemoveAt(_index);
}

void ABossEnemy::PlayMeshLossSound()
{
	PlaySound(onMeshLossSound);

}

void ABossEnemy::PlayDeathSound()
{
	PlaySound(onBossDeathSound);
}

void ABossEnemy::PlayAggroSound()
{
	if (!hasPlayedAggroSound)
	{
		PlaySound(onBossAggroSound);
		hasPlayedAggroSound = true;
	}
}

void ABossEnemy::ManageDeathEvent()
{
	baseMesh->SetSimulatePhysics(true);
	baseMesh->SetMassOverrideInKg(NAME_None,2000);

	moveCompo->SetChaseSpeed(0);
	PlayDeathSound();
	onAllMeshesLost.Clear();
	onEnterAttackRange.Clear();
	onTargetSet.Clear();
	pawnSensor->SightRadius = 0;
	target = nullptr;
	SetLifeSpan(5);
	UE_LOG(LogTemp, Warning, TEXT("ManageDeath Event called"));

}

void ABossEnemy::ManageAllMeshesOverlap(UPrimitiveComponent* _overlappedCompo, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int _meshIndex = INDEX_NONE;
	int _size = allMeshes.Num();
	for (int i = 0; i < _size; i++)
	{
		if (allMeshes[i] == _overlappedCompo)
		{
			_meshIndex = i;
			break;
		}
	}
}

void ABossEnemy::OverlapEventManager()
{
	int _size = allMeshes.Num();
	for (int i = 0; i < _size; i++)
	{
		UStaticMeshComponent* _overlappedMesh = allMeshes[i];
		if (!_overlappedMesh)return;
		
		FMeshData meshData;
		meshData.canBeDestroyed = false;
		allMeshData.Add(meshData);
		_overlappedMesh->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::ManageAllMeshesOverlap);
		
	}
}


void ABossEnemy::OnSecondMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleMeshOverlap(OtherActor, 0);// Since we remove the index after hitting
									//	we can always call the first 
}

void ABossEnemy::OnThirdMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleMeshOverlap(OtherActor, 0); 
}

void ABossEnemy::OnFourthMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleMeshOverlap(OtherActor, 0);

}

void ABossEnemy::OnFifthMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleMeshOverlap(OtherActor, 0);

}

void ABossEnemy::HandleMeshOverlap(AActor* _otherActor, int _meshIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT(" Overlap Event Triggered!"));

	APlayerProjectile* _projectileRef = Cast<APlayerProjectile>(_otherActor);
	if (!materialCheckerCompo || !_projectileRef)return;
	if (materialCheckerCompo->GetAllMatsSize() <= 0) return;
	// INDEX OF ALLMATS IS HARD CODED
	// TO DO - Replace the function ManageBossEnemyHit in 
	// projectile.cpp with ApplyMaterialToMatchingMesh?
	// FindChildMeshes Function should be in Enemy.cpp
	if (_projectileRef->GetCurrentProjectileMat() == materialCheckerCompo->allMatsToCheck[_meshIndex])// testing with the fire arm
	{
		//canDestroySecondMesh = true;
		_projectileRef->GetOnCorrectProjectileMeshOverlap().Broadcast(this);

		UE_LOG(LogTemp, Warning, TEXT("%i Mesh Overlap Event Triggered!"), _meshIndex);
	}
}



void ABossEnemy::ApplyMaterialToMatchingMesh(UMaterialInterface* _matToApply)
{
	////APlayerProjectile* _projectileRef = Cast<APlayerProjectile>(OtherActor);
	//UMaterialCheckerComponent* matChecker = GetMaterialCheckerComponent();

	//if (!matChecker)
	//{
	//	// Handle the case where matChecker is not available.
	//	return;
	//}

	//TArray<UStaticMeshComponent*> allMeshes = GetAllMeshes();
	//int32 size = matChecker->GetAllMatsSize();

	//for (int32 i = 0; i < size; ++i)
	//{
	//	if (i < allMeshes.Num() && i < matChecker->allMatsToCheck.Num())
	//	{
	//		UMaterialInterface* matFromChecker = matChecker->allMatsToCheck[i];
	//		UStaticMeshComponent* currentMesh = allMeshes[i];

	//		if (matToApply == matFromChecker && currentMesh->GetMaterial(0) != matFromChecker)
	//		{
	//			TArray<UStaticMeshComponent*> allChildMeshes = FindAllChildMeshes(currentMesh);

	//			SetMeshMaterialAtIndex(allMeshes, i, matToApply);
	//			SetMeshMaterialChildIncluded(allChildMeshes, matToApply);

	//			ApplyHitEffect(currentMesh);
	//			ApplyHitEffect(allChildMeshes[0]);  // Assuming there's only one child mesh, adjust as needed.

	//			GetOnHit().Broadcast();
	//			RemoveMesh(i);
	//			matChecker->allMatsToCheck.RemoveAt(i);
	//			GetOnMeshLoss().Broadcast();

	//			--i;
	//		}
	//	}
	//	else
	//	{
	//		// Handle the case where index is out of bounds.
	//		return;
	//	}
	//}
}

void ABossEnemy::UpdateSpawnPointLocation()
{
	if (allMeshes.Num() <= 0)return;
	int _size = allMeshes.Num();
	float _spawnOffSet = 150;
	FVector _currentMesh = allMeshes[0]->GetComponentLocation() +(GetActorForwardVector() * _spawnOffSet);
	spawnPoint->SetWorldLocation(_currentMesh);

}

void ABossEnemy::ChasePlayer()
{
		moveCompo->BossChasePlayer();

}
void ABossEnemy::ReturnToInitialLocation()
{
	if (!lastTarget)return; // To avoid rotating on init
		moveCompo->MoveToLocation(initialLocation);
		moveCompo->SetChaseSpeed(1);
		
}
void ABossEnemy::SetTarget(APawn* _pawn)
{
	target = _pawn;
	lastTarget = _pawn;
	moveCompo->SetCanChase(true);
	onTargetSet.Broadcast(true);
	onAggro.Broadcast();
}

void ABossEnemy::Chase()
{
	return;
}


void ABossEnemy::Detect()
{
	if (!target)return;
	const bool _canSee = pawnSensor->CouldSeePawn(target);
	if (!_canSee)
		target = nullptr;
}
void ABossEnemy::CheckReturnedToInitialLocation()
{
	float _distance = FVector::Dist(GetActorLocation(), initialLocation);
	//	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), _distance);
	if (_distance >= 100)
	{
		canRotateTowardsPlayer = false;

	}
	else 
	canRotateTowardsPlayer = true;

}






void ABossEnemy::ApplyDeathMeshes()
{
	deathMesh->SetVisibility(true, true);
	faceMesh->SetSimulatePhysics(true);
	deathMesh->SetSimulatePhysics(true);
	baseMesh->SetVisibility(false, true);
	faceMesh->SetVisibility(false, true);


	
}

void ABossEnemy::ManageDestructionEvent(AActor* _actor)
{
	onDestruction.Broadcast();
}

void ABossEnemy::CheckIsInRangeToAttack()
{
	if (!target) return;
	float _distance = FVector::Dist(GetActorLocation(), target->GetActorLocation());
	if (_distance <= attackRange)
	{

		onEnterAttackRange.Broadcast();
		moveCompo->SetCanChase(false);

	}
	else
	{
		canAttack = false;

	}

}

void ABossEnemy::CheckMinRange()
{
	if (!target) return;
	float _distance = FVector::Dist(GetActorLocation(), target->GetActorLocation());

	if (_distance <= minDistanceToPlayer)
	{

		onEnterMinRange.Broadcast();
		moveCompo->SetCanChase(false);

	}
	else
	{
		//canAttack = false;

	}
}

void ABossEnemy::RotateToPlayer()
{
	if (!lastTarget)return;
	if (!canRotateTowardsPlayer)return;
	if (canRotateTowardsPlayer || lastTarget)
	{

	float _speed = GetWorld()->DeltaTimeSeconds * rotateBackSpeed;
	FVector _directionToPlayer = lastTarget->GetActorLocation() - GetActorLocation();
	FRotator _rotationToPlayer = _directionToPlayer.Rotation();
	FRotator _interpolatedRotation = FMath::RInterpTo(GetActorRotation(), _rotationToPlayer, _speed , 10.f);
	SetActorRotation(_interpolatedRotation);
	}

}


void ABossEnemy::SetCanAttack()
{
	canAttack = true;

}
void ABossEnemy::StopMove()
{
	moveCompo->SetChaseSpeed(0);

}

void ABossEnemy::Attack()
{

	if (!canAttack)return;
	spawnLocation = spawnPoint->GetComponentLocation();
	ABaseProjectile* _projectileSpawned = attackCompo->SpawnProjectile(spawnLocation, this);
	if (!_projectileSpawned)return;
	if (allMeshes.Num() <= 0)
	{
		return;
	}
	else
	{
	_projectileSpawned->meshCompo->SetMaterial(0,allMeshes[0]->GetMaterial(0));
	}
	if (!target)return;
	FVector _direction = (target->GetActorLocation() - GetActorLocation());
	_projectileSpawned->meshCompo->AddImpulse(GetActorForwardVector() * projectileSpeed + _direction, NAME_None, true);
	UE_LOG(LogTemp, Warning, TEXT("Projectile from boss added impulse"));
	//_projectileSpawned->SetCanMove(true);


}
void ABossEnemy::HandleReveal()
{
	if (revealHiddenCompo)
	{
		revealHiddenCompo->GetOnConditionToRevealIsMet().Broadcast();
	}
}



