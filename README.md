# Random Parameter Filtering

## Descripción
El siguiente proyecto es una actualización y encapsulamiento del proyecto de implementación de [RandomParameterFiltering (RPF) en PBRT-v2](http://civc.ucsb.edu/graphics/Papers/Sen2011_RPF/). El proyecto original desarrollado por Pradeep Sen y Soheil Darabi.

## Docker
Se provee una imagen de Docker para facilitar la compilación y ejecución del proyecto. Adicionalmente, un archivo make permite interactuar con la imagen de Docker de manera más sencilla.

Para construir la imagen de Docker, ejecutar el siguiente comando:
```bash
make docker
```

Para abrir la consola de la imagen de Docker, ejecutar el siguiente comando:
```bash
make shell
```

## Compilación

Dentro del entorno dockerizado debe de correr las scripts de scons para compilar el proyecto. 
```bash
cd pbrt-v2-rpf/src && scons
```

## Ejecución

Las scripts de compilación crean el archivo ejecutable en la ruta `pbrt-v2-rpf/build/linux-release/pbrt`. Para ejecutar el programa, se debe de correr el siguiente comando:

```bash
./pbrt-v2-rpf/build/linux-release/pbrt [--outfile <archivo de salida>]  <archivo de escena> 
```

Donde `<archivo de salida>` es el archivo donde se guardará la imagen renderizada y `<archivo de escena>` es el archivo de escena a renderizar.









