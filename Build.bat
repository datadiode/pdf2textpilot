@echo off

echo Building PDF2Text Pilot...
C:\WINDOWS\Microsoft.NET\Framework\v3.5\MSBuild.exe PDF2Text/PDF2Text.sln /nologo /t:Rebuild /p:Platform="Win32" /p:Configuration="Release"

echo off
"twopilots_signtool.exe" "/password:iwY72hfc4P5y4Nyi4YmD" "/file:"PDF2Text\Release\PDF2Text.exe""
"twopilots_signtool.exe" "/password:iwY72hfc4P5y4Nyi4YmD" "/file:"dlls\PDFCreatorPilot.dll""

echo Building Help...
cd Help
call make_PDF2.bat
call make_PDFLib.bat
cd ..
echo Ok.

echo Building converters executables
C:\WINDOWS\Microsoft.NET\Framework\v3.5\MSBuild.exe Installation/Installation.sln /nologo /t:Rebuild /p:Platform="x86" /p:Configuration="Release"

echo off
"twopilots_signtool.exe" "/password:iwY72hfc4P5y4Nyi4YmD" "/file:"Installation\bin\Release\Installation.msi""

copy Installation\bin\Release\Installation.msi PDF2Text.msi

echo ... done.
