@echo off
echo.
echo.
echo.Installation von Middle-earth: Collectible Card Game in DEUTSCH
echo.inklusive der Karten
echo.
pause
perl gccg_package install core client fonts windows32 metw metw-cards* metw-de
del Metw