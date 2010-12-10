; Script originally generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "KQ Lives"
!define PRODUCT_VERSION "Beta"
!define PRODUCT_PUBLISHER "The KQ Team"
!define PRODUCT_WEB_SITE "http://kqlives.sf.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\kq.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"


!define LICENSE_FILE "license-file"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page (not needed)
; !insertmacro MUI_PAGE_LICENSE $LICENSE_FILE
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\$0.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
; OutFile "kq-win32bin-installer.exe" ; (commented out. will be set by make-release.sh)
InstallDir "$PROGRAMFILES\KQ"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  StrCpy $0 "KQ"
  SetOverwrite ifnewer

  SetOutPath "$INSTDIR\data"
  File data\*
  SetOutPath "$INSTDIR\maps"
  File maps\*
  SetOutPath "$INSTDIR\music"
  File music\*
  SetOutPath "$INSTDIR\scripts"
  File scripts\*

  SetOutPath "$INSTDIR"
  File KQ.exe
  File alleg42.dll
  File lua50.dll
  File README
  File COPYING
  File GPL


  CreateDirectory "$SMPROGRAMS\KQ Lives"
  CreateShortCut "$SMPROGRAMS\KQ Lives\$0.lnk" "$INSTDIR\$0.exe"
; CreateShortCut "$DESKTOP\$0.lnk" "$INSTDIR\$0.exe"
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\KQ Lives\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  StrCpy $0 "KQ"
  Delete "$INSTDIR\*"
  Delete "$INSTDIR\data\*"
  Delete "$INSTDIR\maps\*"
  Delete "$INSTDIR\music\*"
  Delete "$INSTDIR\scripts\*"

  Delete "$SMPROGRAMS\KQ Lives\Uninstall.lnk"
; Delete "$DESKTOP\$0.lnk"
  Delete "$SMPROGRAMS\KQ Lives\$0.lnk"

  RMDir "$SMPROGRAMS\KQ Lives"
  RMDir "$INSTDIR\data"
  RMDir "$INSTDIR\maps"
  RMDir "$INSTDIR\music"
  RMDir "$INSTDIR\scripts"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
