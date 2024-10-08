// Fill out your copyright notice in the Description page of Project Settings.

//Game Related Classes
#include "Player/DRPlayerChar.h"
#include "Rocket/DRRocketPawn.h"
#include "Ultimate/DRUltComponent.h"
#include "../DeathRocketGameModeBase.h"

#include <string>
//Components
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
//Inputs
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ADRPlayerChar::ADRPlayerChar()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	mCameraBoom->SetupAttachment(RootComponent);
	mCameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	mCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	mFollowCamera->SetupAttachment(mCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	mFollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mUltRef = CreateDefaultSubobject<UDRUltComponent>(TEXT("Ult"));

}
void ADRPlayerChar::BeginPlay()
{

	// Call the base class  
	Super::BeginPlay();
	
	
	FTimerHandle _;
		mWorldTimerManagerRef->SetTimerForNextTick(this, &ADRPlayerChar::SetupADRController);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(mDefaultMappingContext, 0);
		}
	}

	mMesh = GetMesh();

	mDynMatBotBody = UMaterialInstanceDynamic::Create(mMesh->GetMaterial(0), this);
	mDynMatUpBody = UMaterialInstanceDynamic::Create(mMesh->GetMaterial(1), this);
	mDynMatBackpack = UMaterialInstanceDynamic::Create(mMesh->GetMaterial(2), this);
	mDynMatGun = UMaterialInstanceDynamic::Create(mMesh->GetMaterial(3), this);

	mMesh->SetMaterial(0, mDynMatBotBody);
	mMesh->SetMaterial(1, mDynMatUpBody);
	mMesh->SetMaterial(2, mDynMatBackpack);
	mMesh->SetMaterial(3, mDynMatGun);

	ownName = mMesh->GetOwner()->GetName();

}
void ADRPlayerChar::SetupADRController()
{
	mPlayerController = Cast<ADR_PlayerController>(GetController());
}


//////////////////////////////////////////////////////////////////////////
// Input

void ADRPlayerChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &ADRPlayerChar::Move);

		//Looking
		EnhancedInputComponent->BindAction(mLookAction, ETriggerEvent::Triggered, this, &ADRPlayerChar::Look);

		// UsingUlt
		EnhancedInputComponent->BindAction(mUltAction, ETriggerEvent::Triggered, this, &ADRPlayerChar::UseUlt);

		EnhancedInputComponent->BindAction(mShootAction, ETriggerEvent::Completed, this, &ADRPlayerChar::Shoot);

	}
}
void ADRPlayerChar::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADRPlayerChar::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADRPlayerChar::Shoot(const FInputActionValue& Value)
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(mMuzzleName);
	if (!mActiveRocket)
		mActiveRocket = mBaseRocket;
	float RocketLength = mActiveRocket.GetDefaultObject()->GetLength();
	FVector ZOffset = mCameraBoom->GetForwardVector() * RocketLength+1.f;
	float YOffset = 2.f * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	ShootRocket(MuzzleLocation + ZOffset, mCameraBoom->GetTargetRotation());

}

void ADRPlayerChar::UseUlt(const FInputActionValue& Value)
{
	//Previously in begin play, but possession happens simultaneously
	if (mPlayerController->PlayerUltCharge >= 100)
		mUltRef->CallUltimate();
}

void ADRPlayerChar::Landed(const FHitResult& Hit)
{
	ACharacter::Landed(Hit);
	if (!mCurrentAmmo)
		Reload();
}
void ADRPlayerChar::SetInvisibilityOn(float Opacity)
{
	mbInvisible = true;
	GetMesh()->SetOverlayMaterial(nullptr);
	mDynMatBotBody->SetScalarParameterValue("Opacity", Opacity);//0.05f
	mDynMatUpBody->SetScalarParameterValue("Opacity", Opacity);
	mDynMatBackpack->SetScalarParameterValue("Opacity", Opacity);//0.05f
	mDynMatGun->SetScalarParameterValue("Opacity", Opacity);
	mMesh->bOnlyOwnerSee = true;
}
void ADRPlayerChar::SetInvisibilityOff()
{
	mbInvisible = false;

	ADeathRocketGameModeBase* ptrGamemode = Cast<ADeathRocketGameModeBase>(GetWorld()->GetAuthGameMode());
	if (ptrGamemode)
		GetMesh()->SetOverlayMaterial(ptrGamemode->GetOverlayMaterialColor(GetTeam()));

	mDynMatBotBody->SetScalarParameterValue("Opacity", 1.f);
	mDynMatUpBody->SetScalarParameterValue("Opacity", 1.f);
	mDynMatBackpack->SetScalarParameterValue("Opacity", 1.f);
	mDynMatGun->SetScalarParameterValue("Opacity", 1.f);

	mMesh->bOnlyOwnerSee = false;
}

void ADRPlayerChar::SetPawnUlt(TSubclassOf<UDRUltComponent> ultStaticClass)
{
	if (!ultStaticClass)
		return;

	// Check if there is an old Ult component
	if (mUltRef)
	{
		mUltRef->ConditionalBeginDestroy();
		mUltRef = nullptr;
	}

	// Create a new Ult component of the specified subclass
	mUltRef = NewObject<UDRUltComponent>(this, ultStaticClass.Get());
	if (mUltRef)
	{
		mUltRef->RegisterComponent();
		mUltRef->Owner = this;
	}
}




