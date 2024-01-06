// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSpawner.h"

// Sets default values
AFireSpawner::AFireSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFireSpawner::BeginPlay()
{
	Super::BeginPlay();
	//Init();
	
}

// Called every frame
void AFireSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

