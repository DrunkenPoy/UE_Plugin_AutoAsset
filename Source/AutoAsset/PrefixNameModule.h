// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PrefixNameModule: public IModuleInterface
{
public:
	PrefixNameModule();
	~PrefixNameModule();

	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
void HandlePostImported(UFactory*, UObject* AssetCreated);
FString StripSuffix(const FString& Name, const TCHAR* Suffix);
FString AddPrefix(const FString& Name, const TCHAR* Prefix);
bool RenameWithAsset(UObject* Asset, const TCHAR* Prefix, const TCHAR* OptionalSuffix);
private:
/** ImportSubsystem 바인딩 핸들 */
FDelegateHandle m_AssetPostImportHandle;
};
