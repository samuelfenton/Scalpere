// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "AnimInstance_IKArms.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class SCALPERE_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float p_deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* p_playerInputComponent) override;

	void Constant_TranslateHorizontal(float p_input);
	void Constant_TranslateForward(float p_input);

	void Constant_RotateHorizontal(float p_input);
	void Constant_RotateVertical(float p_input);

	/// <summary>
	/// These should already have delta time involved 
	/// </summary>
	/// <param name="p_input">Input</param>
	void Delta_TranslateHorizontal(float p_input);
	void Delta_TranslateForward(float p_input);

	void Delta_RotateHorizontal(float p_input);
	void Delta_RotateVertical(float p_input);

	//Locomotion
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float m_locomotionHorizontalVelocity = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float m_locomotionForwardVelocity = 100.0f;

	//Rotation
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float m_rotationHorizontalVelocity = 60.0f;
	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float m_rotationVerticalVelocity = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float m_armHeightFromHead = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnywhere)
	//UHeadMountedDisplayFunctionLibrary* m_HMD;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* m_rightController;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* m_leftController;

	//Root Object
	UPROPERTY(EditAnywhere)
	USceneComponent* m_rootObj;

	//Player camera
	UPROPERTY(EditAnywhere)
	UCameraComponent* m_camera;

	//Static mesh
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* m_armSkeletalMesh;
};
