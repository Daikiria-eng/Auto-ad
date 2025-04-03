@echo off

echo [*] Eliminando archivos temporales
del /s /q \Windows\Temp\*
del /s /q \Windows\Prefetch\*
for /d %%i in (\Users\*) do (
    del /s /q %%i\AppData\Local\Temp\*
)
ipconfig -flushdns

echo [*] Limpiando imagenes
dism /online /cleanup-image /checkhealth
dism /online /cleanup-image /scanhealth
dism /online /cleanup-image /restorehealth
sfc /scannow
