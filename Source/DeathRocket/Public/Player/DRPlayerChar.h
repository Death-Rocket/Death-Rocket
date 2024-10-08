// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/DRCharacter.h"
#include "Player/DRPlayerController.h"
#include "Ultimate/DRUltComponent.h"

#include "DRPlayerChar.generated.h"

/**
 * 
 */
UCLASS()
class DEATHROCKET_API ADRPlayerChar : public ADRCharacter
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* mCameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* mFollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* mDefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mJumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mMoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mLookAction;

	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mUltAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mShootAction;

public:
	ADRPlayerChar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FString ownName;

	/** Called to use the ultimate */
	UFUNCTION(BlueprintCallable)
	void UseUlt(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SetInvisibilityOn(float Opacity);
	UFUNCTION(BlueprintCallable)
	void SetInvisibilityOff();

	UFUNCTION(BlueprintCallable)
	void SetPawnUlt(TSubclassOf<UDRUltComponent> ultStaticClass);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return mCameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return mFollowCamera; }
	/** Returns PlayerController**/
	FORCEINLINE class ADR_PlayerController* GetPlayerController() { return mPlayerController; }

protected:
	/** Called when Character lands */
	void Landed(const FHitResult& Hit) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Shoot(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shoot, meta = (AllowPrivateAccess = "true", ToolTip = "Here for the socket name, change with weapon/Mesh"))
	FName mMuzzleName =  "Muzzle_01";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	UDRUltComponent* mUltRef;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	void SetupADRController();


private:

	ADR_PlayerController* mPlayerController;

	USkeletalMeshComponent* mMesh;

	UMaterialInstanceDynamic* mDynMatUpBody;
	UMaterialInstanceDynamic* mDynMatBotBody;
	UMaterialInstanceDynamic* mDynMatBackpack;
	UMaterialInstanceDynamic* mDynMatGun;

};
