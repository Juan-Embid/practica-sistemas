# practica-sistemas
Repositorio para la practica de la asignatura sistemas operativos. Funciones implementadas en mytar_routines.c

## Practica 1:
To test it out you just need to type:

```bash
make
```

Once typed the following instruccion will show you how to compress several files into one single ".mtar" file

```bash
./mytar -cf project1.mtar a.txt b.txt c.txt
```

The files can be simply extracted by

```bash
./mytar -xf project1.mtar
```

## Practica 2: 
En esta práctica hemos creado la función my_unlink y my_read. Se encuentran en el archivo fuseLib.c y ejecutan la lógica cuando se elimina y se lee un archivo.
Once you have practica2 directory cloned in your computer, go to the PATH FicherosP2/FUSE_myFS

```bash
cd FicherosP2/FUSE_myFS
```
Changes are made in fuseLib.c

To compile save the file and type the following command and remove "virtual-disk" in case you have it

```bash
rm virtual-disk
make
```

If there is no "mount-point" directory, create it

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

To check if your functions are correctly made we need to create a new file

```bash
cd mount-point
touch test
ls -l
```

Fill the file with information by opening it with a text editor

```bash
nano test
```

Verify the my_read function with the command "cat"

```bash
cat test
```

Now try the my_unlink function with the command "rm"

```bash
rm test
cd ..
```

And execute the disk auditor

```bash
./my-fsck virtual-disk 
```

## Practica 3: 
Para ejecutarlo lo único que hay que hacer el un make y ejecutar el archivo
