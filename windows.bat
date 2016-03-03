:: local directory
cd %~dp0

:: create all symbolic links
cd "os\events"
mklink /D e-puck "..\platform\e-puck"
mklink /D kilobot "..\platform\kilobot"

cd "..\io"
mklink /D e-puck "..\platform\e-puck"
mklink /D kilobot "..\platform\kilobot"

cd "..\processes"
mklink /D e-puck "..\platform\e-puck"
mklink /D kilobot "..\platform\kilobot"

cd "..\platform\e-puck"
mklink /D library "..\..\..\extern\platform\e-puck\library"
cd "..\platform\kilobot"
mklink /D library "..\..\..\extern\platform\kilobot\kilolib"

cd "..\..\..\.git\hooks"
mklink post-merge "..\..\.git-hooks\post-merge"
mklink pre-commit "..\..\.git-hooks\pre-commit"

cd "..\..\"
pause
