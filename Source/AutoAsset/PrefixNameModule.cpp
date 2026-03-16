// Fill out your copyright notice in the Description page of Project Settings.


#include "PrefixNameModule.h"

#include "Editor.h"
#include "Subsystems/ImportSubsystem.h"
#include "AssetToolsModule.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Misc/PackageName.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundWave.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Animation/AnimSequence.h"
#include "Misc/ConfigCacheIni.h"
#include "CoreGlobals.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

PrefixNameModule::PrefixNameModule()
{
}

PrefixNameModule::~PrefixNameModule()
{
}

void PrefixNameModule::StartupModule()
{
    if (!GEditor)
        return;

    UImportSubsystem* ImportSubsystem =
        GEditor->GetEditorSubsystem<UImportSubsystem>();

    // FBX / 에셋 임포트 후 이벤트 바인딩
    m_AssetPostImportHandle = ImportSubsystem->OnAssetPostImport.AddRaw(
        this, &PrefixNameModule::HandlePostImported);

    // EUBP 자동 실행 구문 (UEditorUtilitySubsystem 활용)
    UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
    if (EditorUtilitySubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AutoAsset] Starting BP Loading in StartupModule."));

        // 유틸리티 에셋 경로 리스트
        TArray<FString> StartupBPPaths = {
            TEXT("/AutoAsset/Editor/EUBP_AssetImportAction.EUBP_AssetImportAction"),
            TEXT("/AutoAsset/Editor/EUBP_StaticmeshActorAction.EUBP_StaticmeshActorAction")
        };

        for (const FString& PathStr : StartupBPPaths)
        {
            FSoftObjectPath SoftPath(PathStr);
            UObject* Asset = SoftPath.TryLoad();
            if (Asset)
            {
                if (EditorUtilitySubsystem->TryRun(Asset))
                {
                    UE_LOG(LogTemp, Warning, TEXT("[AutoAsset] Successfully executed Editor Utility: %s"), *PathStr);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("[AutoAsset] Failed to execute Editor Utility (TryRun failed): %s"), *PathStr);
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[AutoAsset] Failed to load Editor Utility asset: %s. Is the plugin content available?"), *PathStr);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[AutoAsset] EditorUtilitySubsystem is invalid. Cannot run BPs."));
    }
}

void PrefixNameModule::ShutdownModule()
{
    if (GEditor)
    {
        if (UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>())
        {
            if (m_AssetPostImportHandle.IsValid())
            {
                ImportSubsystem->OnAssetPostImport.Remove(m_AssetPostImportHandle);
                m_AssetPostImportHandle.Reset();
            }
        }
    }
}



void PrefixNameModule::HandlePostImported(UFactory*, UObject* AssetCreated)
{
    // 1) SkeletalMesh 임포트
    if (USkeletalMesh* SKM = Cast<USkeletalMesh>(AssetCreated))
    {
        // ─ 메시 이름
        RenameWithAsset(SKM, TEXT("SK_"), nullptr);

        // ─ 연결된 Skeleton 이름
        RenameWithAsset(SKM->GetSkeleton(),TEXT("SKEL_"), TEXT("_Skeleton"));

        // ─ 연결된 PhysicsAsset 이름
        RenameWithAsset(SKM->GetPhysicsAsset(),TEXT("PHYS_"), TEXT("_PhysicsAsset"));

        return;// 다른 에셋엔 손대지 않음
    }

    // 2) StaticMesh 임포트
    if (UStaticMesh* SM = Cast<UStaticMesh>(AssetCreated))
    {
        RenameWithAsset(SM, TEXT("SM_"), nullptr);
        return;
    }

    // 3) Texture 임포트
    if (UTexture* Tex = Cast<UTexture>(AssetCreated))
    {
        RenameWithAsset(Tex, TEXT("T_"),nullptr);
        return;
    }

    // 4) SoundWave 임포트
    if (USoundWave* Sound = Cast<USoundWave>(AssetCreated))
    {
        RenameWithAsset(Sound, TEXT("SW_"), nullptr);
        return;
    }

    // 5) Material 임포트
    if (UMaterial* Material = Cast<UMaterial>(AssetCreated))
    {
        RenameWithAsset(Material, TEXT("M_"), nullptr);
        return;
    }

    // 6) Material Instance 임포트 (메시 임포트 시 생성되는 머티리얼 인스턴스 등)
    if (UMaterialInstanceConstant* MaterialInst = Cast<UMaterialInstanceConstant>(AssetCreated))
    {
        RenameWithAsset(MaterialInst, TEXT("MI_"), nullptr);
        return;
    }

    // 7) Animation Sequence 임포트
    if (UAnimSequence* AnimSeq = Cast<UAnimSequence>(AssetCreated))
    {
        RenameWithAsset(AnimSeq, TEXT("AS_"), nullptr);
        return;
    }
}
FString PrefixNameModule::StripSuffix(const FString& Name, const TCHAR* Suffix)
{
    return (Suffix && Name.EndsWith(Suffix))
    ? Name.LeftChop(FCString::Strlen(Suffix))
    : Name;
}

FString PrefixNameModule::AddPrefix(const FString& Name, const TCHAR* Prefix)
{
    return (Prefix && Name.StartsWith(Prefix))
   ? Name
   : FString(Prefix)+Name;
}

bool PrefixNameModule::RenameWithAsset(UObject* Asset, const TCHAR* Prefix, const TCHAR* OptionalSuffix)
{
    if (!Asset) return false;
    FString BaseName = Asset->GetName();
    // 이미 접두사가 있으면 패스
    FString PrefixName = AddPrefix(BaseName,Prefix);
    
    FString SuffixName = StripSuffix(PrefixName, OptionalSuffix);
    if ( BaseName == SuffixName &&  BaseName == PrefixName) return false;
    
    /*
     * 에셋의 경로에서 경로만 가져옵니다.
     * 예시:
     *      /Game/Texture/SK_Asset  -- GetName()
     * ->   /Game/Texture/          -- GetLongPackagePath()
     */
    const FString PkgPath = FPackageName::GetLongPackagePath(
        Asset->GetOutermost()->GetName());
    /*
     * 에셋의 경로와 새로이 이름을 붙일 경로를 만듭니다.
     * RenameAssets가 어레이를 받으므로 배열로 만듭니다.
     */
    TArray<FAssetRenameData> Batch;
    Batch.Emplace(Asset, PkgPath, SuffixName);
    /*
     * 실제 에셋의 이름을 변경할 수 있도록 에셋 이름을 변경하기 위한 도구를 로드합니다.
     */
    FAssetToolsModule& AT =
        FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    return AT.Get().RenameAssets(Batch);
}

IMPLEMENT_MODULE(PrefixNameModule, AutoAsset)