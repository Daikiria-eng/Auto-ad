# Script: aduser

script para desbloquear y asignar contraseñas genéricas en el directorio activo.
No se usaron librerías al respecto, sólo es el manejo de salidas de comandos en C.
El uso de APIs o demás se consdierará en el tiempo.

## Compilación (básica)

```bash
gcc aduser.c -o aduser.exe
```

## Modo de uso

```batch
.\aduser -i <usuario> [[-u] [-c]] [-p]
```

### Parámetros

```batch
-i <usuario>
```
&emsp;&emsp;&emsp; Especificar el usuario
```batch
-u
```
&emsp;&emsp;&emsp; Desbloquear usuario
```batch
-c
```
&emsp;&emsp;&emsp; cambiar contraseña a la que sea especificada en el comando de la línea 66

## Culminación

El programa dejará en el portapapeles la documentación del caso según la hora.