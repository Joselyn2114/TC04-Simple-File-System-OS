# TC04-Simple-File-System-OS

## Requisitos

- GCC (MinGW-w64 o similar)
- Make (opcional)
- PowerShell (para pruebas en Windows) configurado en UTF-8
  ```powershell
  chcp 65001  # Cambia la página de códigos a UTF-8 para mostrar acentos y caracteres especiales correctamente
  ```

## Compilación

### Con Make

Desde la carpeta raíz del proyecto:
```bash
make            # Compila binario "myfs.exe"
make asan       # Compila con AddressSanitizer
make debug      # Compila con símbolos de depuración
make release    # Compila con optimizaciones
```

### Sin Make (GCC directo)

```bash
gcc -std=c99 -Wall -Wextra src/main.c src/fs.c src/parser.c -I src -o myfs.exe
```

> **Nota:** En ambos métodos el ejecutable resultante se llama `myfs.exe`.

## Ejecución

Todos los casos de prueba están en el directorio `tests/`.

### Ejecución individual

En Windows PowerShell (UTF-8):
```powershell
chcp 65001
.\myfs.exe tests/1.txt    # Flujo básico de prueba
``` 

O bien, usando Make:
```bash
make run TEST=tests/1.txt
```

- Si no defines `TEST`, `make run` ejecuta `tests/1.txt` por defecto.

## Casos de prueba incluidos

| Archivo                          | Descripción                                              |
|----------------------------------|----------------------------------------------------------|
| `tests/1.txt`                    | Flujo básico: CREATE, WRITE, READ, LIST, DELETE          |
| `tests/duplicate_create.txt`     | Intento de CREATE duplicado                              |
| `tests/write_out_of_bounds.txt`  | WRITE que excede el tamaño del archivo                   |
| `tests/unknown_command.txt`      | Comando desconocido                                      |
| `tests/no_args.txt`              | Ejecución sin argumentos (error de uso)                  |

### Ejecutar todas las pruebas

#### Con PowerShell
```powershell
chcp 65001
Get-ChildItem tests\*.txt | ForEach-Object { .\myfs.exe $_.FullName }
```

#### Con Bash (en WSL o Git Bash)
```bash
for test in tests/*.txt; do
  ./myfs.exe "$test"
done
```

#### Con Make
```bash
for test in tests/*.txt; do   make run TEST=$test; done
```

#### Para cada uno individualmente
```powershell
.\myfs.exe tests/1.txt
```

```powershell
.\myfs.exe tests/duplicate_create.txt
```

```powershell
.\myfs.exe tests/no_args.txt
```

```powershell
.\myfs.exe tests/unknown_command.txt
```

```powershell
.\myfs.exe tests/write_out_of_bounds.txt
```

## Estructura del proyecto

```
src/         # Código fuente
  fs.c       # Implementación del sistema de archivos
  fs.h       # Interfaz del sistema de archivos
  parser.c   # Implementación del parser de comandos
  parser.h   # Interfaz del parser
  error.h    # Códigos de error
  main.c     # Punto de entrada (main.c)
build/       # Archivos objeto (*.o)
bin/         # Binarios compilados (myfs.exe)
tests/       # Scripts de prueba (1.txt, duplicate_create.txt, ...)
Makefile     # Reglas de compilación y ejecución
README.md    # Este archivo de instrucciones
```
