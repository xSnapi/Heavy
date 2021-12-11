cd Sandbox
mkdir bin
xcopy "..\libs\SFML\lib\PDB" bin
cd..
start vendor\premake5.exe vs2022