//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "ColorActivatorProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "DragonCanvas/Components/MaterialCheckerComponent.h"
#include "DragonCanvas/Components/RevealHiddenComponent.h"

#include "DragonCanvas/Actors/HiddenActors.h" // can be deleted since we use compo
#include "PlayerProjectile.h"

// THIS CLASS IS THE ONE THAT ALLOWS ACTORS TO CHANGE TO THE COLOR OF THE PROJECTILE
AColorActivatorProjectile::AColorActivatorProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	meshCompo = CreateDefaultSubobject<UStaticMeshComponent>("meshCompo");
	materialChecker = CreateDefaultSubobject<UMaterialCheckerComponent>("materiaChecker");
	onReceiveMaterialSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Sounds/Interaction_Sounds/RevealSoundSource.RevealSoundSource")).Object;
	meshCompo->SetupAttachment(RootComponent);
	AddOwnedComponent(materialChecker);

}

// Called when the game starts or when spawned
void AColorActivatorProjectile::BeginPlay()
{
	Super::BeginPlay();
	Init();
	//initialMat = meshCompo->CreateDynamicMaterialInstance(0);
}

// Called every frame
void AColorActivatorProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColorActivatorProjectile::Init()
{
	OnActorBeginOverlap.AddDynamic(this, &AColorActivatorProjectile::ManageOverlap);
	onMaterialReceived.AddDynamic(this, &AColorActivatorProjectile::PlaySound);
	onMaterialReceived.AddDynamic(this, &AColorActivatorProjectile::SetHasReceivedMaterial);
	
	//FRevealedEvent::FDelegate _lambdaDelegate	
	/*_lambdaDelegate.AddLambda([this]() {
		PlayRevealSound(onRevealSound); 
		});
	onReveal.Add(_lambdaDelegate);*/

	initialCollisionSetting = meshCompo->GetCollisionEnabled();
	meshCompo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	revealHiddenCompo = GetComponentByClass<URevealHiddenComponent>();
}

void AColorActivatorProjectile::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	//Super::ManageOverlap(_overlapped, _overlap);

	if (!_overlap || !_overlapped) return;
	if (!_overlap->IsA(APlayerProjectile::StaticClass()))return;
	if (hasReceivedMaterial)return;
	{
		if(materialChecker->ActorMaterialCheck(_overlap))
		{
			meshCompo->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			meshCompo->SetCollisionEnabled(initialCollisionSetting);
			ReceiveColor(_overlap);
			if (revealHiddenCompo)
			{
				revealHiddenCompo->GetOnConditionToRevealIsMet().Broadcast();
			}


		}
			
	}
}

void AColorActivatorProjectile::ReceiveColor(AActor* _targetToGetColorFrom)
{
	UStaticMeshComponent* _projectileMesh = _targetToGetColorFrom->GetComponentByClass < UStaticMeshComponent >();
	UMaterialInterface* _projectileMat = _projectileMesh->GetMaterial(0);
	meshCompo->SetMaterial(0, _projectileMat);
	onMaterialReceived.Broadcast();
}


void AColorActivatorProjectile::SetHasReceivedMaterial()
{
	hasReceivedMaterial = true;
}

void AColorActivatorProjectile::PlayRevealSound(USoundBase* _soundSource)
{
	if (!_soundSource)return;
	UGameplayStatics::PlaySound2D(GetWorld(), _soundSource);
	UE_LOG(LogTemp, Error, TEXT("REVEAL HIDDEN ACTORS SOUND CALLED"));

}

void AColorActivatorProjectile::PlaySound()
{
	PlayRevealSound(onReceiveMaterialSound);
	UE_LOG(LogTemp, Error, TEXT("REVEAL HIDDEN ACTORS SOUND CALLED"));

}




