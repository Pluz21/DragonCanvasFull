//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "ConeLineTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include <DrawDebugHelpers.h>


// Sets default values
UConeLineTrace::UConeLineTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("ConeLineTrace Constructor!"));
	ownerRef = GetOwner();

}

// Called when the game starts or when spawned
void UConeLineTrace::BeginPlay()
{
	Super::BeginPlay();
	//ownerRef = GetOwner();

	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner initialized: %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find owner"));
	}
}

// Called every frame
void UConeLineTrace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UConeLineTrace::ConeTrace()
{
	//if (!IsValid(GetOwner())) 
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to find owner"));
	//	return;
	//}
	//FVector _startLocation = GetOwner()->GetActorLocation();
	//FRotator _ownerRotation = GetOwner()->GetActorRotation();
	//FVector _forwardVector = GetOwner()->GetActorForwardVector();

	//UE_LOG(LogTemp, Warning, TEXT("Owner = %s"), GetOwner());


	////UE_LOG(LogTemp, Error, TEXT("startLocation %s "),*_startLocation.ToString());

	//FVector _endLocation = _startLocation + (_forwardVector * distance);
	//UE_LOG(LogTemp, Error, TEXT("endlocation %s "),*_endLocation.ToString());
	//DrawDebugSphere(GetWorld(), _endLocation,
	//	200, 25, FColor::Black, false, -1, 0, 3);
	//lineTraceEnd = _endLocation;
	////UE_LOG(LogTemp, Error, TEXT("linetraceend %s "),*lineTraceEnd.ToString());
	//FCollisionQueryParams _ignoreSelfParam;     
	//_ignoreSelfParam.AddIgnoredActor(GetOwner());

	//TArray<FHitResult> _allHits;
	//FQuat _quat = FQuat(GetOwner()->GetActorRotation());

	//bool _hit = GetWorld()->SweepMultiByChannel(_allHits, _startLocation,
	//	_endLocation, _quat, 
	//	_coneTraceChannel, FCollisionShape::MakeSphere(coneTraceRadius),
	//	_ignoreSelfParam);
	//if (_hit)
	//{
	//	canSelfDestruct = true;
	//	for (int i = 0; i < _allHits.Num(); i++)

	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("The FireBreath hit: %s"), *_allHits[i].GetActor()->GetName());
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Nothing hit by FireBreath")); 
	//	canSelfDestruct = false;


	//}

	//DrawDebugSphere(GetWorld(), _startLocation,
	//	coneTraceRadius + distance, 25, FColor::Orange, false, -1, 0, 3);
}


