//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "ProjectileTriggerComponent.h"

#include "Grabber.h"
#include "DragonCanvas/Actors/Spawner.h"
#include "DragonCanvas/World/CustomGameMode.h"
#include "DragonCanvas/Actors/Enemy.h"
#include "DragonCanvas/Actors/ColorActivator.h"
#include "DragonCanvas/Actors/Dragon.h"
#include "DragonCanvas/Actors/ProjectileManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DragonCanvas/Actors/PickUps.h"

#include "DragonCanvas/Components/MaterialCheckerComponent.h"
#include "DragonCanvas/Components/RevealHiddenComponent.h"

#include <Kismet/GameplayStatics.h>

UProjectileTriggerComponent::UProjectileTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UProjectileTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProjectileTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	InitGameMode();
	
	
}

void UProjectileTriggerComponent::Init()
{
	
	onSnap.AddDynamic(this, &UProjectileTriggerComponent::HandleSnap);
	onSnap.AddDynamic(this, &UProjectileTriggerComponent::PlaySound);
	onSnap.AddDynamic(this, &UProjectileTriggerComponent::HandleReveal);
	
	ADragon* _dragonRef = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));
	dragonRef = _dragonRef;
	if(GetOwner())
	revealHiddenCompo = GetOwner()->GetComponentByClass<URevealHiddenComponent>();
}


void UProjectileTriggerComponent::InitGameMode()
{
		gameMode = GetWorld()->GetAuthGameMode<ACustomGameMode>(); // Grab game mode
		if (!gameMode)return;
		projectileManager = gameMode->GetProjectileManager();
		if (!projectileManager)return;
}



void UProjectileTriggerComponent::SnapTarget(AActor* _targetActor) // Target Actor is Actor to Snap
{
	if (!_targetActor)return;

	UPrimitiveComponent* _primitiveCompo = _targetActor->
		GetComponentByClass<UPrimitiveComponent>();

	if (!_primitiveCompo)return;

	FAttachmentTransformRules _worldTransform = FAttachmentTransformRules::KeepWorldTransform;
	FAttachmentTransformRules _snap = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (!MaterialChecker(_targetActor))return; // Check if the material is the same 
	// add safety
	//if(GetOwner()->FindComponentByClass<UMaterialCheckerComponent>()->MaterialChecker(_targetActor))
	// Add To Manager mat

	_primitiveCompo->SetSimulatePhysics(false);				//Snap
	_targetActor->AttachToComponent(GetOwner()->
		GetComponentByClass<UStaticMeshComponent>(), _snap);

	//_targetPickUp->SetIsSnapped();  // Make target not snappable more
	// The grabber is on the player not on the actor to snap
	if (!dragonRef) 
	{
		ADragon* _dragonRef = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));
		dragonRef = _dragonRef;
	} // Adding this security if the dragonRef came to change during play. 

	UGrabber* _grabberCompo = dragonRef->GetComponentByClass<UGrabber>();

	if (!_grabberCompo)return;

	_grabberCompo->Release();

	onSnap.Broadcast();


}

void UProjectileTriggerComponent::HandleSnap()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleSnap Call"));

	AColorActivator* _vessel = Cast<AColorActivator>(GetOwner()); //vessel with triggercompo
	if (_vessel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vessel Name: %s, IsSpawner: %d"), *_vessel->GetName(), _vessel->GetIsSpawner());
		allVessels.Add(_vessel);

	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), vesselToFind, allVessels);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), spawnerToFind, allSpawners);


	int _sizeAllSpawners = allSpawners.Num();
	for (int i = 0; i < _sizeAllSpawners; i++)
	{
		ASpawner* _spawnerRef = Cast<ASpawner>(allSpawners[i]);

		if (_spawnerRef)
		{
			AActor* _spawnedEnemy = _spawnerRef->Spawn();
		}
	}

}

void UProjectileTriggerComponent::PlaySound()
{
	UGameplayStatics::PlaySound2D(GetWorld(),snapSound);
}

bool UProjectileTriggerComponent::MaterialChecker(AActor*& _targetToCheck)
{
	AActor* _owner = GetOwner(); 

	if (!_owner)return false; 
	UStaticMeshComponent* _ownerMesh =
		_owner->GetComponentByClass<UStaticMeshComponent>();
	
	UMaterialInterface* _currentMaterial = _ownerMesh->GetMaterial(0);
	UStaticMeshComponent* _targetMesh = _targetToCheck->
		GetComponentByClass<UStaticMeshComponent>();
	UMaterialInterface* _targetMaterial = _targetMesh->GetMaterial(0);
	if (_targetMaterial != _currentMaterial)
	{
		
		return false;
	}
	else
	{

		mat = _currentMaterial;
		targetMat = _targetMaterial;
		//projectileManager->AddMaterial(UMaterialInstanceDynamic::Create(_currentMaterial, _ownerMesh));
		return true;
	}
}

void UProjectileTriggerComponent::HandleReveal()
{

	if (revealHiddenCompo)
	{
		revealHiddenCompo->GetOnConditionToRevealIsMet().Broadcast();
	}
}
