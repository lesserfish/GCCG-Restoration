@echo off

:LANGUAGES
REM -------------------------
REM Choose your language
REM -------------------------
cls
echo.
echo.GCCG
echo.----
echo.
echo.
echo "E" to continue in English
echo "G" um in Deutsch fortzufahren
echo.
echo To quit,   press the letter "X"
echo Zum Abbrechen bitte "X" druecken
echo.


choice /c egx /M "Enter a letter "

if "%errorlevel%" == "1" goto ENGLISH
if "%errorlevel%" == "2" goto GERMAN
if "%errorlevel%" == "3" goto EXIT
goto EXIT

REM -------------------------
REM English Menu
REM -------------------------
:ENGLISH
cls
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo              ¿           Installation of GCCG-Modules             ¿
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo              ¿    Press the appropriate letter to install the     ¿
echo              ¿    game of your choice.                            ¿
echo              ¿                                                    ¿
echo              ¿     A. METW without Card Images                    ¿
echo              ¿     B. METW with Card Images                       ¿
echo              ¿     C. MTG without Card Images                     ¿
echo              ¿     D. MTG with Card Images                        ¿
echo              ¿     E. LOTR without Card Images                 ¿
echo              ¿     F. LOTR with Card Images                    ¿
echo              ¿     G. Source                                      ¿
echo              ¿                                                    ¿
echo              ¿     U. Update everything                           ¿
echo              ¿     L. Back to language menu                       ¿
echo              ¿     X. Press X to exit to DOS                      ¿
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo.

choice /c abcdefgulx /M "Enter a letter "

if "%errorlevel%" == "1" goto METW
if "%errorlevel%" == "2" goto METW_CARDS
if "%errorlevel%" == "3" goto MTG
if "%errorlevel%" == "4" goto MTG_CARDS
if "%errorlevel%" == "5" goto LOTR
if "%errorlevel%" == "6" goto LOTR_CARDS
if "%errorlevel%" == "7" goto SOURCE
if "%errorlevel%" == "8" goto UPDATE
if "%errorlevel%" == "9" goto LANGUAGES
if "%errorlevel%" == "10" goto EXIT
goto EXIT

REM ------------------------------------------
REM Installation of METW in english
REM ------------------------------------------
:METW
CLS
echo.
echo. Preparing METW...
echo.
echo.
perl gccg_package install core client fonts windows32 metw
echo.
echo Installation completed
echo.
echo.To start GCCG/METW type: metw.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of METW with Cards in english
REM ------------------------------------------
:METW_CARDS
CLS
echo.
echo. Preparing METW with Card Images...
echo.
echo.
perl gccg_package install core client fonts windows32 metw metw-cards*
echo.
echo Installation completed
echo.
echo.To start GCCG/METW type: metw.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of MTG in english
REM ------------------------------------------
:MTG
CLS
echo.
echo. Preparing MTG...
echo.
echo.
perl gccg_package install core client fonts windows32 mtg
echo.
echo Installation completed
echo.
echo.To start GCCG/MTG type: mtg.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of MTG with Cards in english
REM ------------------------------------------
:MTG_CARDS
CLS
echo.
echo. Preparing MTG with Card Images...
echo.
echo.
perl gccg_package install core client fonts windows32 mtg mtg-cards*
echo.
echo Installation completed
echo.
echo.To start GCCG/MTG type: mtg.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of LOTR in english
REM ------------------------------------------
:LOTR
CLS
echo.
echo. Preparing LOTR...
echo.
echo.
perl gccg_package install core client fonts windows32 lotr
echo.
echo Installation completed
echo.
echo.To start GCCG/LOTR type: lotr.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of LOTR with Cards in english
REM ------------------------------------------
:LOTR_CARDS
CLS
echo.
echo. Preparing LOTR with Card Images...
echo.
echo.
perl gccg_package install core client fonts windows32 lotr lotr-cards*
echo.
echo Installation completed
echo.
echo.To start GCCG/LOTR type: lotr.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Install the Sourcecode
REM ------------------------------------------
:SOURCE
CLS
echo.
echo. Preparing the Installation of the Source...
echo.
echo.
perl gccg_package install source
echo.
echo Installation of the Source is completed
echo.
echo.
pause
goto EXIT

REM ------------------------------------------
REM Update Everything
REM ------------------------------------------
:UPDATE
CLS
echo.
echo. Preparing the Installation of the Source...
echo.
echo.
perl gccg_package update
echo.
echo Everything is up to date
echo.
echo.
pause
goto EXIT



REM --------------------------
REM Deutsche Version ---------
REM --------------------------

REM -------------------------
REM German Menu
REM -------------------------
:GERMAN
cls
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo              ¿           Installation der GCCG-Module             ¿
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo              ¿    Einfach den Buchstaben des gewuenschten Moduls  ¿
echo              ¿    druecken, um die jeweilige Aktion zu starten.   ¿
echo              ¿                                                    ¿
echo              ¿     A. METW in deutsch ohne Karten                 ¿
echo              ¿     B. METW in deutsch mit Karten                  ¿
echo              ¿                                                    ¿
echo              ¿     E. englische Module zu installieren            ¿
echo              ¿                                                    ¿
echo              ¿     U. Neueste Version installieren                ¿
echo              ¿     S. Zurueck zur Sprachauswahl                   ¿
echo              ¿     X. Zurueck zu DOS                              ¿
echo              ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
echo.

choice /c abeusx /M "Bitte den Buchstaben tippen > "

if not exist c:\what.exe copy what.exe c:\
if "%errorlevel%" == "1" goto DMETW
if "%errorlevel%" == "2" goto DMETW_CARDS
if "%errorlevel%" == "3" goto ENGLISH
if "%errorlevel%" == "4" goto UPDATE
if "%errorlevel%" == "5" goto LANGUAGES
if "%errorlevel%" == "6" goto EXIT
goto EXIT



REM ------------------------------------------
REM Installation of METW in german
REM ------------------------------------------
:DMETW
CLS
echo.
echo. METW wird installiert...
echo.
echo.
perl gccg_package install core client fonts windows32 metw metw-de
echo.
echo Installation vollstaendig
echo.
echo.Start von GCCG/METW mit: metw.bat [RETURN]
echo.
pause
goto EXIT

REM ------------------------------------------
REM Installation of METW with Cards in german
REM ------------------------------------------
:DMETW_CARDS
CLS
echo.
echo. METW inklusive Karten wird installiert...
echo.
echo.
perl gccg_package install core client fonts windows32 metw metw-cards* metw-de
echo.
echo Installation vollstaendig
echo.
echo.Start von GCCG/METW mit: metw.bat [RETURN]
echo.
pause
goto EXIT




:EXIT
echo.

