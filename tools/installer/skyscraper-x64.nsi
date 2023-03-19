; Script generated by the HM NIS Edit Script Wizard.

!include x64.nsh

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Skyscraper"
!define PRODUCT_VERSION "2.0 Alpha 11"
!define PRODUCT_PUBLISHER "Ryan Thoryk"
!define PRODUCT_WEB_SITE "https://www.skyscrapersim.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Skyscraper.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

; Location where Skyscraper files are stored
;example: !define LOCAL_FILES "${LOCAL_FILES}"
!define LOCAL_FILES "..\..\"
!define WINDOWS_DIR "C:\WINDOWS"
!define SYSTEM_DIR "C:\WINDOWS\System32"

SetCompressor lzma
VIAddVersionKey ProductName "Skyscraper"
VIAddVersionKey FileDescription "Skyscraper"
VIAddVersionKey CompanyName "The Skyscraper Project"
VIAddVersionKey LegalCopyright "Copyright 2003-2023 Ryan Thoryk"
VIAddVersionKey FileVersion "1.11.0.0"
VIAddVersionKey ProductVersion "1.11.0.0"
VIProductVersion 1.11.0.0

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${LOCAL_FILES}\skyscraper.ico"
!define MUI_UNICON "${LOCAL_FILES}\skyscraper.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${LOCAL_FILES}\tools\installer\install.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${LOCAL_FILES}\tools\installer\install.bmp"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "${LOCAL_FILES}\LICENSE"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Skyscraper"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
;!define MUI_FINISHPAGE_RUN "$INSTDIR\Skyscraper.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "skyscraper20alpha11-x64.exe"
InstallDir "$PROGRAMFILES64\Skyscraper"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Application" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "${LOCAL_FILES}\Skyscraper.exe"
  File "${LOCAL_FILES}\SBS.dll"
  File "${LOCAL_FILES}\Skyscraper.pdb"
  File "${LOCAL_FILES}\SBS.pdb"
  File "${LOCAL_FILES}\OgreBulletCollisions.dll"
  File "${LOCAL_FILES}\OgreBulletDynamics.dll"
  File "${LOCAL_FILES}\skyscraper.ini"
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Skyscraper.lnk" "$INSTDIR\Skyscraper.exe"
  CreateShortCut "$DESKTOP\Skyscraper.lnk" "$INSTDIR\Skyscraper.exe"
  File "${LOCAL_FILES}\readme.txt"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Release Notes.lnk" "write.exe" '"$INSTDIR\readme.txt"'
  File "${LOCAL_FILES}\designguide.html"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Building Design Guide.lnk" "$INSTDIR\designguide.html"
  File "${LOCAL_FILES}\changelog.txt"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Changelog.lnk" "write.exe" '"$INSTDIR\changelog.txt"'
  File "${LOCAL_FILES}\design.rtf"
  File "${LOCAL_FILES}\keyboard.ini"
  SetOutPath "$INSTDIR\buildings"
  File "${LOCAL_FILES}\buildings\*.bld"
  SetOutPath "$INSTDIR\screenshots"
  File "${LOCAL_FILES}\screenshots\*.*"
  SetOutPath "$INSTDIR\guide"
  File "${LOCAL_FILES}\guide\*.*"
  SetOutPath "$INSTDIR\data"
  File "${LOCAL_FILES}\data\*.*"
  SetOutPath "$INSTDIR\data\triton_signs"
  File "${LOCAL_FILES}\data\triton_signs\*.*"
  SetOutPath "$INSTDIR\data\fonts"
  File "${LOCAL_FILES}\data\fonts\*.*"
  SetOutPath "$INSTDIR\data\MT"
  File "${LOCAL_FILES}\data\MT\*.*"
  SetOutPath "$INSTDIR\data\scripts"
  File "${LOCAL_FILES}\data\scripts\*.*"
  SetOutPath "$INSTDIR\data\signs"
  File "${LOCAL_FILES}\data\signs\*.*"
  SetOutPath "$INSTDIR\data\materials"
  File "${LOCAL_FILES}\data\materials\*.*"
  SetOutPath "$INSTDIR\data\otis67"
  File "${LOCAL_FILES}\data\otis67\*.*"
  SetOutPath "$INSTDIR\data\Mitsubishi_ADA"
  File "${LOCAL_FILES}\data\Mitsubishi_ADA\*.*"
  SetOutPath "$INSTDIR\data\New_Otis_Elevator"
  File "${LOCAL_FILES}\data\New_Otis_Elevator\*.*"
  SetOutPath "$INSTDIR\data\Otis_ZEN"
  File "${LOCAL_FILES}\data\Otis_ZEN\*.*"
  SetOutPath "$INSTDIR\data\pool"
  File "${LOCAL_FILES}\data\pool\*.*"
  SetOutPath "$INSTDIR\data\Sigma_Palladium"
  File "${LOCAL_FILES}\data\Sigma_Palladium\*.*"
  SetOutPath "$INSTDIR\media\Main"
  File "${LOCAL_FILES}\media\Main\*.*"
  SetOutPath "$INSTDIR\media\packs"
  File "${LOCAL_FILES}\media\packs\*.*"
  SetOutPath "$INSTDIR\media\RTShaderLib\GLSL"
  File "${LOCAL_FILES}\media\RTShaderLib\GLSL\*.*"
  SetOutPath "$INSTDIR\media\Terrain"
  File "${LOCAL_FILES}\media\Terrain\*.*"
SectionEnd

Section /o "Source Code" SEC02
  SetOutPath "$INSTDIR"
  File "${LOCAL_FILES}\skyscraper.ico"
  File "${LOCAL_FILES}\LICENSE"
  File "${LOCAL_FILES}\Doxyfile"
  File "${LOCAL_FILES}\CMakeLists.txt"
  File "${LOCAL_FILES}\compiling.txt"
  File "${LOCAL_FILES}\*.patch"
  SetOutPath "$INSTDIR\ogrebullet"
  File /r "${LOCAL_FILES}\ogrebullet\*.*"
  SetOutPath "$INSTDIR\src"
  File /r "${LOCAL_FILES}\src\*.*"
  SetOutPath "$INSTDIR\codeblocks"
  File "${LOCAL_FILES}\codeblocks\*.*"
  SetOutPath "$INSTDIR\codeblocks\wxsmith"
  File "${LOCAL_FILES}\codeblocks\wxsmith\*.*"
  SetOutPath "$INSTDIR\msvc"
  File "${LOCAL_FILES}\msvc\*.*"
  SetOutPath "$INSTDIR\tools"
  File /r "${LOCAL_FILES}\tools\*.*"
SectionEnd

Section "Required libraries" SEC03
  SetOutPath "$INSTDIR"
  File "${LOCAL_FILES}\Caelum.dll"
  File "${LOCAL_FILES}\cg.dll"
  File "${LOCAL_FILES}\fmod.dll"
  File "${LOCAL_FILES}\Codec_FreeImage.dll"
  File "${LOCAL_FILES}\OgreMain.dll"
  File "${LOCAL_FILES}\OgreOverlay.dll"
  File "${LOCAL_FILES}\OgreRTShaderSystem.dll"
  File "${LOCAL_FILES}\OgreBites.dll"
  File "${LOCAL_FILES}\FreeImage.dll"
  File "${LOCAL_FILES}\Plugin_CgProgramManager.dll"
  File "${LOCAL_FILES}\Plugin_OctreeSceneManager.dll"
  File "${LOCAL_FILES}\RenderSystem_Direct3D9.dll"
  File "${LOCAL_FILES}\RenderSystem_Direct3D11.dll"
  File "${LOCAL_FILES}\RenderSystem_GL.dll"
  File "${LOCAL_FILES}\RenderSystem_GL3Plus.dll"
  File "${LOCAL_FILES}\RenderSystem_GLES2.dll"
  File "${LOCAL_FILES}\plugins.cfg"
  File "${LOCAL_FILES}\resources.cfg"
  File "${LOCAL_FILES}\ogre.cfg"
  File "${LOCAL_FILES}\wxbase32u_vc_custom.dll"
  File "${LOCAL_FILES}\wxmsw32u_core_vc_custom.dll"
  File "${LOCAL_FILES}\D3DX9_43.dll"
  File "${LOCAL_FILES}\D3DCompiler_47.dll"
  File "${LOCAL_FILES}\msvcp140.dll"
  File "${LOCAL_FILES}\vcruntime140.dll"
  File "${LOCAL_FILES}\vcruntime140_1.dll"
;  File "${LOCAL_FILES}\dbghelp.dll"
  File "${LOCAL_FILES}\vc_redist.x64.exe"
  File "${LOCAL_FILES}\*.pdb"
  File "${LOCAL_FILES}\zlib.dll"
  File "${LOCAL_FILES}\SDL2.dll"
  ExecWait '"$INSTDIR\vc_redist.x64.exe" /Q'
  Delete "${LOCAL_FILES}\vc_redist.x64.exe" 
  SetOutPath "$INSTDIR\data\caelum"
  File /r "${LOCAL_FILES}\data\caelum\*.*"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\Skyscraper.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Skyscraper.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}" "$ICONS_GROUP"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Application"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Source code"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "Required libraries"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ReadRegStr $ICONS_GROUP ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}"
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  RMdir /r "$INSTDIR"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Website.lnk"
  Delete "$DESKTOP\Skyscraper.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Skyscraper.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Release Notes.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Building Design Guide.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Changelog.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
