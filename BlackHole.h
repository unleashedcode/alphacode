// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"


class UParticleSystem;
class USoundCue;
class UStaticMeshComponent;
class USphereComponent;


UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

	float CollisionSphereRadius;
	float DestroySphereRadius;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* BlackHoleMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* EventHorizonSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SingularitySphere;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestroyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundFX")
	USoundCue* DestroySoundFX;

	void OverlapSingularityDestroyFX();
	

	bool bBlackHoleSucks;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")	
	UFUNCTION()
	void PlayDestroyFX();

	UFUNCTION()
	void PlayDestroySoundFX();

};
