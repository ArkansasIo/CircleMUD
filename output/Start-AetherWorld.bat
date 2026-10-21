@echo off
setlocal

rem Run from the project root so the server can find its lib directory.
cd /d "%~dp0.."
if errorlevel 1 (
  echo Could not change to the AetherWorld project folder.
  pause
  exit /b 1
)

if not exist "output\circle.exe" (
  echo AetherWorld server executable was not found at output\circle.exe.
  echo Build the server first, then run this launcher again.
  pause
  exit /b 1
)

echo Starting AetherWorld on port 4000...
echo Keep this window open while the server is running.
echo.
"output\circle.exe" 4000
set "server_exit_code=%ERRORLEVEL%"

echo.
echo AetherWorld stopped with exit code %server_exit_code%.
pause
exit /b %server_exit_code%
