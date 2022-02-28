# practica-sistemas
Repositorio para la practica de la asignatura sistemas operativos. Funciones implementadas en mytar_routines.c

## Practica 1:
Acabada pero no correcta del todo. Estamos esperando la corrección del profesor para que nos confirme

## Practica 2: 
En esta práctica hemos creado la función my_unlink y my_read. Se encuentran en el archivo fuseLib.c y ejecutan la lógica cuando se elimina y se lee un archivo.
Once you have practica2 directory cloned in your computer, go to the PATH FicherosP2/FUSE_myFS

```bash
cd FicherosP2/FUSE_myFS
```
Changes are made in fuseLib.c

To compile save the file and type the following command

```bash
make
```

If there is no "mount-point" diretory, create it

```bash
mkdir mount-point
```

To detemine which command you need to use 

```bash
./fs-fuse
```

In my case

```bash
./fs-fuse -t 2097152 -a virtual-disk -f '-d -s mount-point'
```

MANTAIN THIS TERMINAL OPEN

OPEN A NEW TERMINAL

To check if your functions are correctly made

```bash
cd mount-point
touch test
ls -l
rm test
cd ..
```

And execute the disk auditor

```bash
./my-fsck virtual-disk 
```

## Practica 3: 
Sin empezar
