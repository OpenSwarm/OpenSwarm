:: local directory
cd %~dp0

:: create all symbolic links
cd "os\events"
mklink /D e-puck "..\platform\e-puck"

cd "..\io"
mklink /D e-puck "..\platform\e-puck"

cd "..\processes"
mklink /D e-puck "..\platform\e-puck"

cd "..\platform\e-puck"
mklink /D library "..\..\..\extern\platform\e-puck\library"


cd "..\..\..\"
pause
