; Inno Setup Script

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{98A39462-A3DA-4FCE-AB0D-B4CC3C988C23}
AppName=LispIDE
AppVerName=LispIDE
AppPublisher=DaanSystems
AppPublisherURL=http://www.daansystems.com/
AppSupportURL=http://www.daansystems.com/
AppUpdatesURL=http://www.daansystems.com/
DefaultDirName={pf}\LispIDE
DefaultGroupName=LispIDE
LicenseFile=..\License.txt
InfoBeforeFile=..\ReadMe.txt
OutputDir=.
OutputBaseFilename=LispIDE_Setup
Compression=lzma
SolidCompression=yes
WizardImageFile=installer.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Files]
Source: "../Release/LispIDE.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "../ReadMe.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme
Source: "../doc/CLHS.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "../doc/CLtL2.chm"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\LispIDE"; Filename: "{app}\LispIDE.exe"; WorkingDir: "{app}"
Name: "{group}\ReadMe.txt"; Filename: "{app}\ReadMe.txt"
Name: "{group}\Common Lisp HyperSpec"; Filename: "{app}\CLHS.chm"
Name: "{group}\Common Lisp The Language, 2nd Edition"; Filename: "{app}\CLtL2.chm"
Name: "{group}\{cm:UninstallProgram,LispIDE}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\LispIDE"; Filename: "{app}\LispIDE.exe"; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\LispIDE"; Filename: "{app}\LispIDE.exe"; WorkingDir: "{app}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\LispIDE.exe"; Description: "{cm:LaunchProgram,LispIDE}"; Flags: nowait postinstall skipifsilent

