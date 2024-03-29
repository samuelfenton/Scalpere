// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_rootObj = CreateDefaultSubobject<USceneComponent>(TEXT("Player Character"));
	SetRootComponent(m_rootObj);

	m_armSkeletalMesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("Arm Mesh"));
	m_armSkeletalMesh->SetupAttachment(m_rootObj);

	m_camera = CreateDefaultSubobject< UCameraComponent>(TEXT("Player Camera"));
	m_camera->FieldOfView = 90.0f;
	m_camera->SetupAttachment(m_rootObj);
	m_camera->SetRelativeLocation(FVector(0.0f, 0.0f, 170.0f));

	//IM HERE
	//m_HMD = CreateDefaultSubobject<UHeadMountedDisplayFunctionLibrary>(TEXT("HMD"));

	m_leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	m_leftController->SetupAttachment(m_rootObj);
	m_leftController->SetTrackingSource(EControllerHand::Left);
	m_leftController->SetDisplayModelSource(TEXT("OpenXR"));
	m_leftController->SetShowDeviceModel(true);

	m_rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	m_rightController->SetupAttachment(m_rootObj);
	m_rightController->SetTrackingSource(EControllerHand::Right);
	m_rightController->SetDisplayModelSource(TEXT("OpenXR"));
	m_rightController->SetShowDeviceModel(true);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_leftController->bDisplayDeviceModel = true;
	m_rightController->bDisplayDeviceModel = true;

	UAnimInstance* animInstance = m_armSkeletalMesh->GetAnimInstance();

	if (animInstance != nullptr)
	{
		UAnimInstance_IKArms* asIKArms = Cast<UAnimInstance_IKArms>(animInstance);

		if (asIKArms != nullptr)
		{
			asIKArms->m_leftController = m_leftController;
			asIKArms->m_rightController = m_rightController;
			UE_LOG(LogTemp, Warning, TEXT("we set them all right"));

		}
		else

		{
			UE_LOG(LogTemp, Warning, TEXT("Cant cast"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant find instance"));

	}
	UE_LOG(LogTemp, Warning, TEXT("Ohhh"));

}

// Called every frame
void APlayerCharacter::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);

	//Auto rotate back	
	FRotator cameraRot = m_camera->GetRelativeRotation();
	FRotator meshRot = m_armSkeletalMesh->GetRelativeRotation();;

	meshRot.Yaw = cameraRot.Yaw;

	m_armSkeletalMesh->SetRelativeRotation(meshRot);

	FVector cameraLoc = m_camera->GetRelativeLocation();
	FVector meshLoc = m_armSkeletalMesh->GetRelativeLocation();
	meshLoc.X = cameraLoc.X;
	meshLoc.Y = cameraLoc.Y;
	meshLoc.Z = cameraLoc.Z - m_armHeightFromHead;

	m_armSkeletalMesh->SetRelativeLocation(meshLoc);

	UAnimInstance* animInstance = m_armSkeletalMesh->GetAnimInstance();
	if (animInstance != nullptr)
	{
		UAnimInstance_IKArms* asIKArms = Cast<UAnimInstance_IKArms>(animInstance);

		if (asIKArms != nullptr)
		{
			if(asIKArms->m_leftController == nullptr)
				UE_LOG(LogTemp, Warning, TEXT("nully"));


			asIKArms->m_leftController = m_leftController;
			asIKArms->m_rightController = m_rightController;
			UE_LOG(LogTemp, Warning, TEXT("we set them all right"));

		}
		else

		{
			UE_LOG(LogTemp, Warning, TEXT("Cant cast"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant find instance"));

	}
	UE_LOG(LogTemp, Warning, TEXT("Ohhh"));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* p_playerInputComponent)
{
	Super::SetupPlayerInputComponent(p_playerInputComponent);

	//Register translation
	p_playerInputComponent->BindAxis("MovementAxisLeft_X", this, &APlayerCharacter::Constant_TranslateHorizontal);
	p_playerInputComponent->BindAxis("MovementAxisLeft_Y", this, &APlayerCharacter::Constant_TranslateForward);

	//Register rotation
	p_playerInputComponent->BindAxis("MovementAxisRight_X", this, &APlayerCharacter::Constant_RotateHorizontal);
	//p_playerInputComponent->BindAxis("MovementAxisRight_Y", this, &APlayerCharacter::Constant_RotateVertical);
}


#pragma region Input management

/// <summary>
/// Moving left/right, input doesn't take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Constant_TranslateHorizontal(float p_input)
{
	FVector rightDir = m_camera->GetRightVector();
	rightDir.Z = 0.0f;
	rightDir.Normalize();

	FVector newLoc = m_rootObj->GetComponentLocation();
	newLoc += rightDir * p_input * FApp::GetDeltaTime() * m_locomotionHorizontalVelocity;

	m_rootObj->SetWorldLocation(newLoc);
}

/// <summary>
/// Moving forward/back, input doesn't take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Constant_TranslateForward(float p_input)
{
	FVector forwardDir = m_camera->GetForwardVector();
	forwardDir.Z = 0.0f;
	forwardDir.Normalize();

	FVector newLoc = m_rootObj->GetComponentLocation();
	newLoc += forwardDir * p_input * FApp::GetDeltaTime() * m_locomotionForwardVelocity;
	m_rootObj->SetWorldLocation(newLoc);
}

/// <summary>
/// Rotating left/right, input doesn't take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Constant_RotateHorizontal(float p_input)
{
	FRotator newRot = m_rootObj->GetComponentRotation();
	newRot.Yaw += p_input * FApp::GetDeltaTime() * m_locomotionHorizontalVelocity;
	m_rootObj->SetWorldRotation(newRot);
}

/// <summary>
/// Rotating up/down, input doesn't take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Constant_RotateVertical(float p_input)
{
	FRotator newRot = m_rootObj->GetComponentRotation();
	newRot.Pitch += p_input * FApp::GetDeltaTime() * m_rotationVerticalVelocity;
	m_rootObj->SetWorldRotation(newRot);
}

/// <summary>
/// Moving left/right, input should already take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Delta_TranslateHorizontal(float p_input)
{
	FVector rightDir = m_camera->GetRightVector();
	rightDir.Z = 0.0f;
	rightDir.Normalize();

	FVector newLoc = m_rootObj->GetComponentLocation();
	newLoc += rightDir * p_input * m_locomotionHorizontalVelocity;
	m_rootObj->SetWorldLocation(newLoc);
}

/// <summary>
/// Moving forward/back, input should already take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Delta_TranslateForward(float p_input)
{
	FVector forwardDir = m_camera->GetForwardVector();
	forwardDir.Z = 0.0f;
	forwardDir.Normalize();

	FVector newLoc = m_rootObj->GetComponentLocation();
	newLoc += forwardDir * p_input * m_locomotionForwardVelocity;
	m_rootObj->SetWorldLocation(newLoc);
}

/// <summary>
/// Rotating left/right, input should already take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Delta_RotateHorizontal(float p_input)
{
	FRotator newRot = m_rootObj->GetComponentRotation();
	newRot.Yaw += p_input * m_locomotionHorizontalVelocity;
	m_rootObj->SetWorldRotation(newRot);
}

/// <summary>
/// Rotating up/down, input should already take delta time into account
/// </summary>
/// <param name="p_input">Current input</param>
void APlayerCharacter::Delta_RotateVertical(float p_input)
{
	FRotator newRot = m_rootObj->GetComponentRotation();
	newRot.Pitch += p_input * m_rotationVerticalVelocity;
	m_rootObj->SetWorldRotation(newRot);
}

#pragma endregion

