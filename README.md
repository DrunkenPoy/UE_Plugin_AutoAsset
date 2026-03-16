# AutoAsset Plugin

AutoAsset은 언리얼 엔진에서 에셋 임포트 시 자동으로 에셋의 네이밍 컨벤션을 맞춰주고, 에셋 관리를 더욱 편리하게 해주는 에디터 유틸리티를 제공하는 플러그인입니다.

## 주요 기능 (Features)

### 1. 임포트 시 자동 네이밍 (Auto Naming on Import)
에셋을 엔진으로 임포트할 때, 해당 에셋의 타입(Class)을 판별하여 알맞은 접두사(Prefix)를 자동으로 붙여주며, 기본적으로 생성되는 불필요한 접미사(Suffix)를 정리합니다.

지원하는 에셋과 네이밍 규칙:
* **Static Mesh**: `SM_` 접두사 자동 추가
* **Skeletal Mesh**: `SK_` 접두사 자동 추가
* **Skeleton**: `SKEL_` 접두사 추가 및 `_Skeleton` 접미사 제거
* **Physics Asset**: `PHYS_` 접두사 추가 및 `_PhysicsAsset` 접미사 제거
* **Texture**: `T_` 접두사 자동 추가
* **Sound Wave**: `SW_` 접두사 자동 추가
* **Material**: `M_` 접두사 자동 추가
* **Material Instance**: `MI_` 접두사 자동 추가
* **Animation Sequence**: `AS_` 접두사 자동 추가

### 2. 에셋 관리 에디터 유틸리티 (Editor Utility)
플러그인 내에 포함된 에디터 유틸리티 블루프린트를 통해 에셋 네이밍 및 관리를 더 쉽게 할 수 있습니다. 
특히 **스크립팅된 에셋 액션(Scripted Asset Action)** 및 **스크립팅된 액터 액션(Scripted Actor Action)** 기능을 통해 직관적인 워크플로우를 제공합니다.

* **사용 방법**: 콘텐츠 브라우저에서 에셋을 우클릭하거나, 뷰포트 및 아웃라이너에서 액터를 우클릭하여 나오는 메뉴를 통해 커스텀 액션을 수행할 수 있습니다.
* **에셋 관리 툴 경로**: `AutoAsset Content/Editor/EUBP_AssetImportAction`
* **주의사항**: 에디터 유틸리티 블루프린트를 직접 확인하거나 수정하려면 콘텐츠 브라우저 설정에서 **'플러그인 콘텐츠 표시(Show Plugin Content)'** 옵션을 켜야 합니다.

지원하는 기능:
* **부모 머티리얼 변경**: 머티리얼 에셋의 부모 머티리얼 설정을 변경합니다.
* **텍스처 임포트 변경**: 텍스처 에셋의 접미사에 따라 임포트 설정을 변경합니다.
* **이름 추가**: 접두사나 접미사를 추가합니다.
* **이름 대치하여 변경**: 에셋이름을 대치합니다. 부분적인 변경도 가능합니다. ex) SM_Electric_VFX -> SM_Fire_VFX
* **스태틱 메시 변경**: 스태틱 메시를 변경하고 메시 이름도 같이 변경합니다.

## 설치 및 적용 방법 (Installation)
1. GitHub 리포지토리의 **[Releases]** 탭에서 제공되는 최신 릴리즈 압축파일(`.zip`)을 다운로드합니다.
2. 다운로드한 압축파일을 해제합니다.
3. 적용하려는 언리얼 프로젝트 폴더 내에 `Plugins` 폴더를 생성합니다. (이미 존재한다면 생략)
4. 압축 해제한 `AutoAsset` 폴더를 프로젝트의 `Plugins` 폴더 위치로 복사해 넣습니다.
5. 언리얼 에디터를 실행하여 프로젝트를 열람할 때, 모듈 리빌드가 필요하다는 팝업이 뜨면 **[Yes]**를 눌러 플러그인을 컴파일합니다.
6. 에디터 진입 후, 상단 메뉴 `Edit > Plugins` 창에서 **Auto Asset Tool** 이 활성화(`Enabled`)되어 있는지 확인합니다.
     * _참고: 블루프린트 전용 프로젝트의 경우 플러그인 컴파일을 위해 프로젝트에 빈 C++ 클래스를 하나 이상 추가하여 C++ 프로젝트로 변환해야 할 수 있습니다._

### 직접 빌드가 필요한 경우 (Build from Source)
엔진 버전이 다르거나 직접 컴파일해서 사용해야 하는 경우, 깃허브 릴리즈에서 함께 제공되는 빌드 스크립트 압축파일(`build.bat` 및 `build_config.ini` 포함)을 이용하여 플러그인을 수동으로 빌드할 수 있습니다.

1. 릴리즈 탭에서 빌드 스크립트 압축파일을 다운로드하고 플러그인의 소스 코드와 같은 경로에 압축을 해제합니다.
2. `build_config.ini` 파일을 텍스트 편집기로 열어 자신의 환경에 맞게 변수를 수정합니다.
   * `UATPath`: 로컬 PC에 설치된 언리얼 엔진의 `RunUAT.bat` 절대 경로 (예: `C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat`)
   * `PluginPath`: 빌드 대상인 `AutoAsset.uplugin` 파일의 상대 경로 (예: `\Plugins\AutoAsset\AutoAsset.uplugin`)
   * `OutputPath`: 빌드된 플러그인이 생성될 출력 폴더 경로 (예: `\BuildPlugin\5.5\AutoAsset`)
3. `build.bat` 파일을 더블클릭하여 실행하면 내부적으로 UAT(Unreal Automation Tool)가 실행되어 플러그인 패키징을 시작합니다.
4. 빌드가 완료되면 `OutputPath`에 지정했던 경로로 들어가 생성된 빌드 결과물을 프로젝트의 `Plugins` 폴더에 복사하여 사용합니다.

## 개발 환경 (Environment)
* 언리얼 엔진 C++ 기반 커스텀 플러그인 (Unreal Engine 5 이상 권장)
* 모듈: `Editor` 환경 전용 플러그인 (`LoadingPhase: PostEngineInit`)
