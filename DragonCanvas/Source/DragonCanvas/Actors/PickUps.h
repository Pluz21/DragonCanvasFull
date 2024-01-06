//Copyright © 2023 Pluz21(TVL).All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUps.generated.h"

UCLASS()
class DRAGONCANVAS_API APickUps : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> meshCompo;
	
public:	
	// Sets default values for this actor's properties
	APickUps();
	UPROPERTY(EditAnywhere)
	bool isSnapped = false;

	UPROPERTY(EditAnywhere)
	bool canRotate = true;
	UPROPERTY(EditAnywhere)
	float rotateSpeed = 50;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsSnapped() { isSnapped = true; }
	void Rotate();
};
