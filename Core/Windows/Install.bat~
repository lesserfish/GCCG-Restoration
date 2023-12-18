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

what c "Enter a letter > " egx

if not exist c:\what.exe copy what.exe c:\
if "%what%" == "E" goto ENGLISH
if "%what%" == "G" goto GERMAN
if "%what%" == "X" goto EXIT
if "%what%" == "" goto EXIT

REM -------------------------
REM English Menu
REM -------------------------
:ENGLISH
cls
echo              ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
echo              º           Installation of GCCG-Modules             º
echo              ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶
echo              º    Press the appropriate letter to install the     º
echo              º    game of your choice.                            º
echo              º                                                    º
echo              º     A. METW without Card Images                    º
echo              º     B. METW with Card Images                       º
echo              º     C. MTG without Card Images                     º
echo              º     D. MTG with Card Images                        º
echo              º     E. LOTR without Card Images                 º
echo              º     F. LOTR with Card Images                    º
echo              º     G. Source                                      º
echo              º                                                    º
echo              º     U. Update everything                           º
echo              º     L. Back to language menu                       º
echo              º     X. Press X to exit to DOS                      º
echo              ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
echo.

what c "Enter a letter > " abcdefgulx

if not exist c:\what.exe copy what.exe c:\
if "%what%" == "A" goto METW
if "%what%" == "B" goto METW_CARDS
if "%what%" == "C" goto MTG
if "%what%" == "D" goto MTG_CARDS
if "%what%" == "E" goto LOTR
if "%what%" == "F" goto LOTR_CARDS
if "%what%" == "G" goto SOURCE
if "%what%" == "U" goto UPDATE
if "%what%" == "L" goto LANGUAGES
if "%what%" == "X" goto EXIT
if "%what%" == "" goto EXIT

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
echo              ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
echo              º           Installation der GCCG-Module             º
echo              ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶
echo              º    Einfach den Buchstaben des gewuenschten Moduls  º
echo              º    druecken, um die jeweilige Aktion zu starten.   º
echo              º                                                    º
echo              º     A. METW in deutsch ohne Karten                 º
echo              º     B. METW in deutsch mit Karten                  º
echo              º                                                    º
echo              º     E. englische Module zu installieren            º
echo              º                                                    º
echo              º     U. Neueste Version installieren                º
echo              º     S. Zurueck zur Sprachauswahl                   º
echo              º     X. Zurueck zu DOS                              º
echo              ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
echo.

what c "Bitte den Buchstaben tippen > " abeusx

if not exist c:\what.exe copy what.exe c:\
if "%what%" == "A" goto DMETW
if "%what%" == "B" goto DMETW_CARDS
if "%what%" == "E" goto ENGLISH
if "%what%" == "U" goto UPDATE
if "%what%" == "S" goto LANGUAGES
if "%what%" == "X" goto EXIT
if "%what%" == "" goto EXIT



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

