CALL CMakeCommon.bat

ECHO *** Set MVC9 specifics and run CMake ***

SET MVC9_OUTPUTDIR="%OUTPUTDIR%\MVC9a"
CMake.exe -G"Visual Studio 9 2008" -H"%SRCROOTDIR%" -B%MVC9_OUTPUTDIR%
PAUSE