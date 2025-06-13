# TC04-Simple-File-System-OS

### Requisitos

- GCC
- Make

### Compilación

- Ejecuta `make` para compilar.
- Ejecuta `make asan` para compilar con AddressSanitizer.
- Ejecuta `make debug` para compilar con símbolos de depuración.
- Ejecuta `make release` para compilar con optimizaciones.

### Ejecución del programa

- Por defecto, las pruebas están en `tests/1.txt`. Usa `make run` para ejecutar la prueba predeterminada.
- Para usar otro archivo de prueba, ejecuta:

  ```bash
  make run ARGS="archivo.txt"
  ```

- O modifica la variable `ARGS` en el archivo `Makefile`.

### Estructura del proyecto

```
src/         # código fuente
  fs.c       # implementación del sistema de archivos
  fs.h       # interfaz del sistema de archivos
  parser.c   # implementación del parser
  parser.h   # interfaz del parser
  error.h    # códigos de error
  main.c     # punto de entrada del programa
build/       # archivos objeto
bin/         # binarios compilados
tests/       # archivos de prueba
```
