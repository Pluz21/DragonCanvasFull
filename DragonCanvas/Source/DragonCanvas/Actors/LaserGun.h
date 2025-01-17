// Copyright © 2023 Pluz21 (TVL). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "LaserGun.generated.h"

UCLASS()
class DRAGONCANVAS_API ALaserGun : public AGun
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Rotate(UStaticMeshComponent* _meshToRotate) override;
	/*void GunSwap(AGun* _gunToUse) override;*/
};
