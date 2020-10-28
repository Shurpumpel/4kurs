;=== Defines
!define REG    `${SET}\${APP}.reg`
!define DEFREG `${DEFDATA}\settings\${APP}.reg`
;===
!define DJVU   `${DATA}\DjVu resources`
;===
!define LANG   `HKEY_CURRENT_USER\Software\Andrew Zhezherun\WinDjView\Settings`
!define KEY    language
;===
!define EXE    `${APPDIR}\WinDjView.exe`
!define 32     `${APPDIR}\WinDjView32.exe`
!define 64     `${APPDIR}\WinDjView64.exe`
;===
!define PFM    `$0\PortableApps.com\PortableAppsPlatform.exe`
;===
Function GetLCID
	!macro _GetLCID _LNG _LCID
		Push ${_LNG}
		Call GetLCID
		Pop ${_LCID}
	!macroend
	!define GetLCID "!insertmacro _GetLCID"
	Exch $0
	StrCmp $0 dword:00000409 0 +3
	StrCpy $0 1033
		Goto EndLCID
	StrCmp $0 dword:00000419 0 +3
	StrCpy $0 1049
		Goto +2
	StrCpy $0 1033
	EndLCID:
	Exch $0
FunctionEnd
Function GetLNG
	!macro _GetLNG _LCID _LNG
		Push ${_LCID}
		Call GetLNG
		Pop ${_LNG}
	!macroend
	!define GetLNG "!insertmacro _GetLNG"
	Exch $0
	StrCmp $0 1033 0 +3
	StrCpy $0 199
		Goto EndLNG
	StrCmp $0 1049 0 +3
	StrCpy $0 1049
		Goto +2
	StrCpy $0 error
	EndLNG:
	Exch $0
FunctionEnd
;=== Custom.nsh
${SegmentFile}
${Segment.OnInit}
	Push $0
	${IsWOW64} $0
	StrCmp $0 0 0 +3
	${WriteSettings} 32 Architecture
		Goto +2
	${WriteSettings} 64 Architecture
	Pop $0
!macroend
;===
!macro OS
	Push $0
	${If} ${IsNT}
		${IfNot} ${AtLeastWinXP}
			StrCpy $0 `Windows XP or newer...`
			MessageBox MB_ICONSTOP|MB_TOPMOST `$(MINREQ)`
			Call Unload
			Quit
		${EndIf}
	${Else}
		StrCpy $0 `Windows XP or newer...`
		MessageBox MB_ICONSTOP|MB_TOPMOST `$(MINREQ)`
		Call Unload
		Quit
	${EndIf}
	Pop $0
!macroend
;===
!macro Lang
	Push $0
	${GetParent} `$EXEDIR` $0
	${If} ${FileExists} `${PFM}`
		ReadEnvStr $0 PortableApps.comLocaleID
		${GetLNG} $0 $0
		${If} $0 == error
			IfFileExists `${REG}` 0 +3
			ReadINIStr $0 `${REG}` `${LANG}` `"${KEY}"`
				Goto +2
			ReadINIStr $0 `${DEFREG}` `${LANG}` `"${KEY}"`
			${GetLCID} $0 $0
			${SetEnvironmentVariable} PortableApps.comLocaleID $0
		${EndIf}
	${Else}
		IfFileExists `${REG}` 0 +3
		ReadINIStr $0 `${REG}` `${LANG}` `"${KEY}"`
			Goto +2
		ReadINIStr $0 `${DEFREG}` `${LANG}` `"${KEY}"`
		${GetLCID} $0 $0
		${SetEnvironmentVariable} PortableApps.comLocaleID $0
	${EndIf}
	Pop $0
!macroend
;===
!macro LangInit
	Push $0
	ReadEnvStr $0 PortableApps.comLocaleID
	${GetLNG} $0 $0
	${SetEnvironmentVariable} PAL:LanguageCustom $0
	Pop $0
!macroend
;===
!macro Init
	IfFileExists `${DJVU}` +2
	CreateDirectory `${DJVU}`
	IfFileExists `${REG}` +2
	CopyFiles /SILENT `${DEFREG}` `${REG}`
!macroend
;===
!macro EXE
	${If} ${RunningX64}
		Rename `${64}` `${EXE}`
	${Else}
		Rename `${32}` `${EXE}`
	${EndIf}
!macroend
;===
${SegmentPostPrimary}
	${If} ${RunningX64}
		Rename `${EXE}` `${64}`
	${Else}
		Rename `${EXE}` `${32}`
	${EndIf}
!macroend