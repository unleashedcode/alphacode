// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values constructor
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphereRadius = 85.f;
	DestroySphereRadius = 10.f;
	bBlackHoleSucks = false;


	BlackHoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackHoleMesh"));
	BlackHoleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlackHoleMesh->SetVisibility(true);
	RootComponent = BlackHoleMesh;

	//setup a sphere component which will trigger if radialforce is applied. Actors must trigger to get pulled in.
	EventHorizonSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	EventHorizonSphere->SetSphereRadius(CollisionSphereRadius);
	EventHorizonSphere->SetupAttachment(BlackHoleMesh);

	//setup a spherecomponent to destroy any actors which collide with it
	SingularitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphere"));
	SingularitySphere->SetSphereRadius(DestroySphereRadius);	
	SingularitySphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SingularitySphere->SetupAttachment(BlackHoleMesh);
	//DestroySphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapDestroySphere);

}


/**Objective here is to have a mesh component with two spheres as declared above. One sphere is the trigger for an object getting to close to the black hole
* which will suck it in. The second smaller sphere will destroy the object when it overlaps and also play an animation to signify a destroyed
* object. 
**/



// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	//create an array of primitiveComponents which overlap the collision sphere
	TArray<UPrimitiveComponent*> OverlappingComps;
	TArray<UPrimitiveComponent*> OverlappedSingularityComponents;
	
	//get the overlapping actors in the scene
	EventHorizonSphere->GetOverlappingComponents(OverlappingComps);
	SingularitySphere->GetOverlappingComponents(OverlappedSingularityComponents);

	
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimitiveComponent = OverlappingComps[i];
		
		if (PrimitiveComponent && PrimitiveComponent->IsSimulatingPhysics()) //making sure that first a PrimitiveComponent exist, then checking if it has simulated physics ticked
		{
			const float RadialForceStrength = -18000;

			const float RadialForceRadius = EventHorizonSphere->GetScaledSphereRadius(); 
			
			PrimitiveComponent->AddRadialForce(GetActorLocation(), RadialForceRadius, RadialForceStrength, ERadialImpulseFalloff::RIF_Linear, true);				
			
		}			
	}


	for (int32 si = 1; si < OverlappedSingularityComponents.Num(); si++)
	{
		UPrimitiveComponent* SingularityPrimitiveComponent = OverlappedSingularityComponents[si];
	
		if (SingularityPrimitiveComponent)
		{
			SingularityPrimitiveComponent->DestroyComponent();
			OverlapSingularityDestroyFX();
		}
	}

}


void ABlackHole::OverlapSingularityDestroyFX()
{
	PlayDestroyFX();
	PlayDestroySoundFX();
}

void ABlackHole::PlayDestroyFX()
{

	UGameplayStatics::SpawnEmitterAtLocation(this, DestroyFX, GetActorLocation());
	//next play a sound
}

void ABlackHole::PlayDestroySoundFX()
{
	//UGameplayStatics::PlaySound2D(this, DestroySoundFX); 
}
