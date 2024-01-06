//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "ColorActivator.h"
#include "Dragon.h"
#include "PickUps.h"

#include "ProjectileManager.h"
#include "DragonCanvas/World/CustomGameMode.h"

#include "DragonCanvas/Components/MaterialCheckerComponent.h"

#include "DragonCanvas/Components/ProjectileTriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerProjectile.h"

// THIS CLASS CHANGES THE MATERIAL OF THE PROJECTILE ON OVERLAP WITH A PICK-UP class

AColorActivator::AColorActivator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>("root");
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("baseMesh");
	secondMesh = CreateDefaultSubobject<UStaticMeshComponent>("secondMesh");
	triggerCompo = CreateDefaultSubobject<UProjectileTriggerComponent>("trigger");
	materialChecker = CreateDefaultSubobject<UMaterialCheckerComponent>("matChecker");
	baseMesh->SetupAttachment(root);
	secondMesh->SetupAttachment(baseMesh);
	AddOwnedComponent(triggerCompo);
}

// Called when the game starts or when spawned
void AColorActivator::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AColorActivator::ManageOverlap);
	Init();
	
}

// Called every frame
void AColorActivator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
void AColorActivator::Init()
{
	//Might need to call managers as in Dragon class 
	InitGameMode();
	ADragon* _dragonRef = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));
	if (!_dragonRef || !_dragonRef->GetProjectileToSpawn())return;
	TSubclassOf<APlayerProjectile> _projectileRef = _dragonRef->GetProjectileToSpawn();
	dragonProjectileRef = _projectileRef;
	if (!secondMesh || !baseMesh)return;
	UMaterialInterface* _secondMeshMat = secondMesh->GetMaterial(0);
	if (!_secondMeshMat)return;
	baseMesh->SetMaterial(0, _secondMeshMat);
	onMaterialReceived.Broadcast(_secondMeshMat);
	matInterfaceToApply = secondMesh->GetMaterial(0);

}

void AColorActivator::InitGameMode()
{
	gameMode = GetWorld()->GetAuthGameMode<ACustomGameMode>(); // Grab game mode
	if (!gameMode)return;
	projectileManager = gameMode->GetProjectileManager();
	if (!projectileManager)return;
}




void AColorActivator::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	if (!_overlap || !_overlapped) return;

	if (_overlap->IsA(APickUps::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with %s"), *_overlap->GetName());
		if(materialChecker->ActorMaterialCheck(_overlap))
			{
				GiveColor();
				if (!triggerCompo)return;
				triggerCompo->SnapTarget(_overlap);
			}
	}
}

void AColorActivator::GiveColor() // Function to be re-used for the projectile selection
{
	if (!dragonProjectileRef)return;
	APlayerProjectile* _projectileRef = dragonProjectileRef.GetDefaultObject();
	if (!_projectileRef)return;
	 UStaticMeshComponent* _projectileMesh = _projectileRef->
					GetComponentByClass<UStaticMeshComponent>();
	 if (!_projectileMesh)return;
	 UMaterial* _projectileMaterial = _projectileMesh->GetMaterial(0)->GetMaterial();
	 if (!_projectileMaterial)return;
	 _projectileMesh->SetMaterial(0, matInterfaceToApply);
	 //_projectileMesh->SetMaterial(0,matToApply); 
	 
	 projectileManager->AddMaterial(matInterfaceToApply);

}






