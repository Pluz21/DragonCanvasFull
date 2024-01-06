//Copyright © 2023 Pluz21(TVL).All rights reserved.

#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include <DrawDebugHelpers.h>
#include <EnhancedInputSubsystems.h>


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	physicsHandle = GetPhysicsHandleComponent();
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Hold();
}



bool UGrabber::FindTargetInReach(FHitResult& _outHitResult)   //maybe transform into bool
{
	FVector _ownerLocation = GetOwner()->GetActorLocation();
	//DrawDebugSphere(GetWorld(), _ownerLocation, sphereRadius, 12, FColor::Blue);
	//FVector _spawnPointRef;
	FVector _cameraNormalDirection =  GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
	//FRotator CameraRotation;
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(_spawnPointRef, CameraRotation);

	//FVector _cameraForwardVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::X);

	FVector _targetLocation = _ownerLocation + _cameraNormalDirection * maxGrabDistance;

	//DrawDebugSphere(GetWorld(), _targetLocation,
	//	sphereRadius, 12, FColor::Red, true, -1, 0, 3);	// ...
	FQuat _ownerQuat = GetOwner()->GetActorQuat();
	FRotator _ownerRotation = GetOwner()->GetActorRotation();
	FCollisionShape _sphere = FCollisionShape::MakeSphere(grabRadius);
	DrawDebugSphere(GetWorld(), _targetLocation,
		_sphere.GetSphereRadius(), 12, FColor::Magenta, false, 1, 0, 3);
	/*
	
	FRotator CameraRotation;
	FVector _cameraForwardVector;
	FVector _spawnPointRef; // only used to get the rotation from GetPlayerViewPoint since it OUTs FVector and FRotator;

	FVector _spawnPointVector = _dragonRef->baseGunRef->materialChangerMesh->GetComponentLocation();
	_dragonRef->playerController->GetPlayerViewPoint(_spawnPointRef, CameraRotation);
	_cameraForwardVector = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::X);

	FVector _lookAtLocation = _spawnPointVector + (_cameraForwardVector * 10000);

	//DrawDebugSphere(GetWorld(), _spawnPointVector, 10, 12, FColor::Orange,true);
	//DrawDebugLine(GetWorld(), _spawnPointVector, _lookAtLocation, FColor::Red, true);

	FVector _projectileSpawnLocationFVector = _dragonRef->projectileSpawnPoint->GetForwardVector();
	FVector _projectileDirection = (_lookAtLocation - _spawnPointVector).GetSafeNormal();
	meshCompo->AddImpulse(_projectileDirection * impulseSpeed, NAME_None, true);
	*/

	FCollisionQueryParams _customParams;
	_customParams.AddIgnoredActor(GetOwner());
	

	bool _hit =  GetWorld()->SweepSingleByChannel(_outHitResult, _ownerLocation, // might need to change for more accurate grabbing linetrace
		_targetLocation,
		FQuat::Identity, ECC_GameTraceChannel2,
		_sphere, _customParams);
	if (!_hit)return false;
	DrawDebugPoint(GetWorld(), _outHitResult.ImpactPoint, 20, FColor::Cyan, false, 5, 0);
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *_outHitResult.GetActor()->GetName());

	targetLocation = _targetLocation;
	return _hit;
	
		
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandleComponent()
{
	UPhysicsHandleComponent* _physicsHandleComponent = GetOwner()->
		GetComponentByClass<UPhysicsHandleComponent>();
	return _physicsHandleComponent;
}

void UGrabber::Grab()
{

	FHitResult _hitResult;
	
	bool _hasHit = FindTargetInReach(_hitResult);
	hitResult = _hitResult;
	if (!_hasHit)
	{
		Release();

		return;
	}

	UPrimitiveComponent* _hitComponent = _hitResult.GetComponent();
	hitComponent = _hitComponent;
	_hitComponent->WakeAllRigidBodies();
	_hitComponent->SetSimulatePhysics(true);   // might need to be actor

	_hitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	physicsHandle->GrabComponentAtLocationWithRotation(
		_hitComponent, NAME_None,		// might need to be actor
		_hitResult.ImpactPoint,
		_hitComponent->GetComponentRotation());
	SetIsGrabbing();
}

void UGrabber::Hold()
{
	if (!isGrabbing)return;
	if(physicsHandle && physicsHandle->GetGrabbedComponent())
	{

			FVector _ownerLocation = GetOwner()->GetActorLocation();
			FVector _cameraNormalDirection = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
			FVector _targetLocation = _ownerLocation + _cameraNormalDirection * holdDistance;
		
			physicsHandle->SetTargetLocationAndRotation(_targetLocation,
				GetOwner()->GetActorRotation());
			DrawDebugSphere(GetWorld(), _targetLocation, 20, 10, FColor::Blue);
	}
		
}

void UGrabber::Release()
{

	if (physicsHandle->GetGrabbedComponent() && isGrabbing == true)
	{
		physicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* _heldActor = physicsHandle->GetGrabbedComponent()->GetOwner();

		physicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Warning, TEXT("Removing tag from %s"), *_heldActor->GetName()); 
		SetIsGrabbing();
	}

}

void UGrabber::SetIsGrabbing()
{
	isGrabbing = !isGrabbing;
}








