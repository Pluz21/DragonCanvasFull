//Copyright © 2023 Pluz21(TVL).All rights reserved.
#include "PlayerProjectile.h"
#include "Dragon.h"

#include "DragonCanvas/World/CustomGameMode.h"

#include "ProjectileManager.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include "Gun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DragonCanvas/Components/MaterialCheckerComponent.h"
#include "DragonCanvas/Components/MoveComponent.h"
#include "DragonCanvas/Components/HealthComponent.h"


APlayerProjectile::APlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;


	moveCompo = CreateDefaultSubobject<UMoveComponent>("moveCompo");
	AddOwnedComponent(moveCompo);

}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	Init();

}


// Called every frame
void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	deltaSeconds = GetWorld()->DeltaTimeSeconds;
	CheckDistance(targetLocation);
	SelfMove(forwardVector);
	//SelfMove();
	moveCompo->Rotate();// or call moveCompo->Move();
}

void APlayerProjectile::Init()
{
	EventsInit();
	

	//projectileManager->AddItem(this); // Not necessary. Safety extra call but already called on spawn from Dragon
	actorSpawnLocation = GetActorLocation();
	if (GetOwner()) 
	{
	forwardVector = GetOwner()->GetActorForwardVector();
	}
	//moveSpeed = moveCompo->GetMoveSpeed(); // MoveSpeed will always be set through the component
	SelfMove();
	SetLifeSpan(lifeSpan);

}

void APlayerProjectile::EventsInit()
{
	OnActorBeginOverlap.AddDynamic(this, &APlayerProjectile::ManageOverlap);
	onTargetReached.AddDynamic(this, &APlayerProjectile::SelfDestruct);
	onCanMove.AddDynamic(this, &APlayerProjectile::FindEndLocation);
	onProjectileCreated.Broadcast();
	onCorrectProjectileMeshOverlap.AddDynamic(this, &APlayerProjectile::ManageBossEnemyHit);
}

void APlayerProjectile::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	

	if (_overlap && (_overlap != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("OVERLAPPING PROJECTILE WITH %s "), *_overlap->GetName());
	}
	else 
		UE_LOG(LogTemp, Warning, TEXT("FAILED OVERLAPCALL"));
	ManageEnemyHit(_overlap);
	//ManageBossEnemyHit(_overlap);

	onEnemyHit.Broadcast();
	
	ManageDestroyTagHit(_overlap);
	ManageCanMoveHit(_overlap);
	

}

void APlayerProjectile::SelfMove()
{
	APawn* _pawnRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!_pawnRef)return;
	ADragon* _dragonRef = Cast<ADragon>(_pawnRef);
	if (!_dragonRef || !_dragonRef->baseGunRef)return;
	//_dragonRef->GetPawnViewLocation();
	FRotator CameraRotation;
	FVector _cameraForwardVector;
	FVector _spawnPointRef; // only used to get the rotation from GetPlayerViewPoint since it OUTs FVector and FRotator;

	FVector _spawnPointVector = _dragonRef->baseGunRef->materialChangerMesh->GetComponentLocation();
	_dragonRef->playerController->GetPlayerViewPoint(_spawnPointRef, CameraRotation);
	
	_cameraForwardVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::X);

	FVector _lookAtLocation = _spawnPointVector + (_cameraForwardVector * 10000);

	/*DrawDebugSphere(GetWorld(), _spawnPointVector, 10, 12, FColor::Orange,true);
	DrawDebugLine(GetWorld(), _spawnPointVector, _lookAtLocation, FColor::Red, true);*/

	FVector _projectileSpawnLocationFVector = _dragonRef->projectileSpawnPoint->GetForwardVector();
	FVector _projectileDirection = (_lookAtLocation - _spawnPointVector).GetSafeNormal();
	meshCompo->AddImpulse(_projectileDirection * impulseSpeed, NAME_None, true);
	

	
	
}


void APlayerProjectile::SelfMove(const FVector& _actorForwardVector)
{
	if (canMove)
	{
	moveCompo->SelfMove(_actorForwardVector);
	onCanMove.Broadcast();
	}

}

void APlayerProjectile::SetCanMove(bool _value)
{
	canMove = _value;
}




void APlayerProjectile::ManageBossEnemyHit(AActor* _actor)
{
	if (_actor->IsA(ABossEnemy::StaticClass()))
	{
		ABossEnemy* _bossEnemy = Cast<ABossEnemy>(_actor);
		if (!_bossEnemy) return;
		//if (!_bossEnemy->GetCanDestroySecondMesh()) return; // Onlyi for secondMesh
		UMaterialCheckerComponent* _matChecker = _bossEnemy->GetMaterialCheckerComponent();
		int _size = _matChecker->GetAllMatsSize();
		UMaterialInterface* _matToApply = meshCompo->GetMaterial(0);
		TArray<UStaticMeshComponent*> _allChildStaticMeshes;
		for (int i = 0; i < _size; i++)
		{
			if (i < _bossEnemy->GetAllMeshes().Num() && i < _matChecker->allMatsToCheck.Num())
			{			
				if (_matToApply == _matChecker->allMatsToCheck[i] && _bossEnemy->GetAllMeshes()[i]->GetMaterial(0) != _matChecker->allMatsToCheck[i]) // testing with the fire arm
				{
					_allChildStaticMeshes = FindAllChildMeshes(_bossEnemy->GetAllMeshes()[i]);
					_bossEnemy->SetMeshMaterialAtIndex(_bossEnemy->GetAllMeshes(), i, _matToApply); // parent
					_bossEnemy->SetMeshMaterialChildIncluded(_allChildStaticMeshes, _matToApply);
					ApplyHitEffect(_allChildStaticMeshes[i]);
					ApplyHitEffect(_bossEnemy->GetAllMeshes()[i]);
					_bossEnemy->GetOnHit().Broadcast();
					_bossEnemy->RemoveMesh(i);
					_matChecker->allMatsToCheck.RemoveAt(i);
					_bossEnemy->GetOnMeshLoss().Broadcast();
					i--;

				}
				else return;

			}
			else return;
			
		}
	}


}

void APlayerProjectile::ManageEnemyHit(AActor* _actor)
{
	if (_actor->IsA(ABossEnemy::StaticClass()))return;
	AEnemy* _enemy = Cast<AEnemy>(_actor);
	if (!_enemy)return;
	UMaterialCheckerComponent* _matChecker = _enemy->GetMaterialCheckerComponent();
	int _size = _matChecker->GetAllMatsSize();

	TArray<UStaticMeshComponent*> _allStaticMeshes;
	UStaticMeshComponent* _baseMesh = _enemy->GetBaseMesh();
	_allStaticMeshes = FindAllChildMeshes(_baseMesh);
	for (int i = 0; i < _size; i++)
	{
		UMaterialInterface* _matToApply = meshCompo->
			GetMaterial(0);
		if (_matToApply == _matChecker->allMatsToCheck[i])
		{
			_enemy->SetBaseMeshMaterial(_matToApply);
			_enemy->moveCompo->SetChaseSpeed(0);
			_enemy->SetLifeSpan(enemyLifeSpan);
			ApplyHitEffect(_allStaticMeshes);  // Dismantles static meshes
			_enemy->GetOnDeath().Broadcast(); // not ideal
			_enemy->SetHasBeenHit(true);
			onEnemyHit.Broadcast();
		}
	}

}

void APlayerProjectile::ManageDestroyTagHit(AActor* _actor)
{
	if (_actor->ActorHasTag("Destroy"))
	{
		_actor->Destroy();
		SelfDestruct();
	}

}
void APlayerProjectile::ManageCanMoveHit(AActor* _actor)
{
	if (_actor->ActorHasTag("CanMove"))
	{
		FVector _overlapLocation = _actor->GetActorLocation();
		FVector _displacedLocation = _overlapLocation + forwardVector * 200;
		_actor->SetActorLocation(_displacedLocation);
		SelfDestruct();
	}


}


void APlayerProjectile::FindEndLocation()
{
	ADragon* _dragonRef = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));
	//targetLocation = _dragonRef->GetProjectileTargetLocation();
	//lineTraceDistance = _dragonRef->GetSphereTraceDistance();
	minDistanceToSelfDestruct = _dragonRef->GetMinDistanceToSelfDestruct();
	//UE_LOG(LogTemp, Warning, TEXT("targetlocaiton is %s"), *targetLocation.ToString());
	onTargetAcquired.Broadcast();

}

void APlayerProjectile::CheckDistance(FVector& _targetLocation)
{
	/*if (hasTarget) 
	{*/

	float _distanceToLineTraceEndLocation = FVector::Dist(GetActorLocation(), _targetLocation);
	
	//UE_LOG(LogTemp, Warning, TEXT("targetLocation : %s"), *targetLocation.ToString());

	if (_distanceToLineTraceEndLocation <= minDistanceToSelfDestruct)// || Overlapped )
	{
		onTargetReached.Broadcast(); // Calling destruct
	}
	//}
}



void APlayerProjectile::CallLineTraceDisplacement()
{	

	//canActivateLineTraceEffect = true;

	//ADragon* _dragonRef = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));
	//_dragonRef->StartLineTraceAction();
}

TArray<UStaticMeshComponent*> APlayerProjectile::FindAllChildMeshes(UStaticMeshComponent*& _parentMesh)
{
	TArray<USceneComponent*> _allSceneCompos;
	_allSceneCompos = _parentMesh->GetAttachChildren();
	TArray<UStaticMeshComponent*> _allStaticMeshes;
	for (USceneComponent* _childCompo : _allSceneCompos)  // FOREACH DIDNT WORK HERE
	{
		UStaticMeshComponent* _mesh = Cast<UStaticMeshComponent>(_childCompo);
		_allStaticMeshes.Add(_mesh);

	}
	return _allStaticMeshes;
}

TArray<UStaticMeshComponent*> APlayerProjectile::FindAllChildMeshes(UStaticMeshComponent*& _parentMesh, UStaticMeshComponent*& _parentMesh2, UStaticMeshComponent*& _parentMesh3)
{
	TArray<USceneComponent*> _allSceneCompos;
	_allSceneCompos = _parentMesh->GetAttachChildren(),_parentMesh2->GetAttachChildren(),
		_parentMesh3->GetAttachChildren();
	TArray<UStaticMeshComponent*> _allStaticMeshes;
	for (USceneComponent* _childCompo : _allSceneCompos)  // FOREACH DIDNT WORK HERE
	{
		UStaticMeshComponent* _mesh = Cast<UStaticMeshComponent>(_childCompo);
		_allStaticMeshes.Add(_mesh);

	}
	return _allStaticMeshes;
}

void APlayerProjectile::ApplyHitEffect(TArray<UStaticMeshComponent*> _allStaticMeshesToHit)
{
	int _size = _allStaticMeshesToHit.Num();
	for (int i = 0; i < _size; i++)
	{
		_allStaticMeshesToHit[i]->SetSimulatePhysics(true);
		_allStaticMeshesToHit[i]->SetMassOverrideInKg(NAME_None, 0.01f);
	}
}

void APlayerProjectile::ApplyHitEffect(UStaticMeshComponent* _staticMeshToAffect)
{
	_staticMeshToAffect->SetSimulatePhysics(true);
	_staticMeshToAffect->SetMassOverrideInKg(NAME_None, 0.01f);
}

void APlayerProjectile::UpdateOverlapPhysics(AActor*& _actorToActivatePhysicsOn)
{
	meshCompo->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//meshCompo->SetCollisionEnabled(initialCollisionSetting);
//	initialCollisionSetting = meshCompo->GetCollisionEnabled();

}






