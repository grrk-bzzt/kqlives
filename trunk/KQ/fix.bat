@echo off

rem  ==========================================================================
rem   Borrowed from Allegro package ;)  But with some modifications too.
rem   ReyBrujo, 2002
rem  ==========================================================================

rem
rem  Can you see it?  *grin*  TAB will hold the '\t' character, which at first
rem  glance you cannot see it, but believe me, it is there. Be careful if you
rem  edit this file: if your editor replaces '\t' for spaces, the generated
rem  makefile won't work!
rem
set TAB=	
if [%TAB%] == [] goto ooes_error

if [%1] == [mingw]  goto mingw
if [%1] == [djgpp]  goto djgpp
if [%1] == [msvc]   goto msvc
if [%1] == [none]   goto none
goto help

:mingw
echo Configuring the package for Windows/MingW32
set  PK_MAKEFILE=makefile.mgw
goto fix

:djgpp
echo Configuring the package for DOS/Djgpp
set  PK_MAKEFILE=makefile.dj
goto fix

:msvc
echo Configuring the package for Windows/MSVC
set  PK_MAKEFILE=makefile.vc
goto fix

:none
echo Setting original makefile
echo # default makefile generated by fix.bat > makefile
echo all: >> makefile
echo %TAB%@echo You need to setup your compiler with fix.bat >> makefile
goto end

:help
echo.
echo Usage: fix platform
echo.
echo Where platform is one of djgpp, mingw, msvc or none to go back again
echo to the original makefile.
echo.
goto end

:ooes_error
echo Not enough memory to set the variables. Check FAQ.TXT
goto end

:fix
if [%PK_MAKEFILE%] == [] goto ooes_error

echo # generated by fix.bat> makefile
echo include mkfiles/makefile.cfg>> makefile
echo include mkfiles/%PK_MAKEFILE%>> makefile
echo include mkfiles/makefile.all>> makefile
echo -include $(OBJDIR)/makefile.dep>> makefile
goto done

:done
echo Done!

:end
set  PK_MAKEFILE=
set  TAB=
