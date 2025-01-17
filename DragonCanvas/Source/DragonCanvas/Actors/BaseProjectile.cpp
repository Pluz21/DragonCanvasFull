// Copyright © 2023 Pluz21 (TVL). All rights reserved.


#include "DragonCanvas/Actors/BaseProjectile.h"
#include "DragonCanvas/Actors/ProjectileManager.h"
#include "DragonCanvas/World/CustomGameMode.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("mymesh");
	sphereCollider = CreateDefaultSubobject<USphereComponent>("sphereCollider");
	
	meshCompo->SetupAttachment(RootComponent);
	sphereCollider->SetupAttachment(meshCompo);
	//SetRootComponent(meshCompo);

}
// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::Init()
{
	SetSimulatePhysics();

	gameMode = GetWorld()->GetAuthGameMode<ACustomGameMode>();

	if (!gameMode)return;
	projectileManager = gameMode->GetProjectileManager();
}

void ABaseProjectile::SetSimulatePhysics()
{
	meshCompo->SetSimulatePhysics(true);
}

void ABaseProjectile::SelfDestruct()
{
	projectileManager->RemoveItem(this);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTION"));

}
