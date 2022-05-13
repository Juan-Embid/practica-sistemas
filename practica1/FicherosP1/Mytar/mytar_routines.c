#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE *origin, FILE *destination, int nBytes) //FILE --> puntero a estructura tipo FILE donde se encuentra el file. Es una instancia de apertura de ese fichero. FILE nos pasa el fichero abierto
{
 int i = 0, c, p;
 c = getc(origin);

 while (i < nBytes && (c != EOF)) {
  p = putc(c, destination);
  if (p == EOF)
   return -1;

  c = getc(origin);
  i++;
 }

 return i;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file) {
	int size = 0, c = getc(file);
	char * buffer;
	while (c != (int)'\0' && c != EOF) {
		c = getc(file);
		size++;
	}
	buffer = malloc(size*sizeof(char));
	fseek(file, -size, SEEK_CUR);
	fread(buffer, sizeof(char), size, file);
	return buffer;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles) {
    int i = 0, fSize;
    stHeaderEntry * array = NULL;

    fread(nFiles, sizeof(int), 1, tarFile);
    array = malloc(sizeof(stHeaderEntry) * (*nFiles));
    while (i < (*nFiles)) {
        array[i].name = loadstr(tarFile);
        fread(&fSize, sizeof(int), 1, tarFile);
        array[i].size = fSize;
        i++;
    }
	return array;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[]) {
    int offData = 0, bytesCopied; //offdata lo que va a ocupar en el fichero
    stHeaderEntry * buffer;
    FILE *inputFile;

    FILE *outputFile = fopen(tarName, "w");
    if(outputFile == NULL) {
        printf("Error opening the output file");
        return EXIT_FAILURE;
    }

    buffer = malloc(sizeof(stHeaderEntry) * nFiles);
    offData = sizeof(int) + nFiles * sizeof(unsigned int);
    for (int i = 0; i < nFiles; i++) {
        offData += strlen(fileNames[i]) + i;
        buffer[i].name = malloc(strlen(fileNames[i]) + 1);
        strcpy(buffer[i].name,fileNames[i]);
    }
    fseek(outputFile, offData, SEEK_SET);
    for (int i = 0; i < nFiles; i++) {
       inputFile = fopen(fileNames[i], "r");

       if(inputFile == NULL) {
            printf("Error opening the input file");
            return EXIT_FAILURE;
        }

       bytesCopied = copynFile(inputFile, outputFile, INT_MAX);
       fclose(inputFile);
       buffer[i].size = bytesCopied;
    }
    fseek(outputFile, 0, SEEK_SET);
    fwrite(&nFiles, sizeof(int), 1, outputFile);

    for(int i = 0; i < nFiles; i++) {
        fwrite(buffer[i].name, strlen(buffer[i].name) + 1, 1, outputFile);
        fwrite(&buffer[i].size, sizeof(unsigned int), 1, outputFile);
    }

    for(int i = 0; i < nFiles; i++)
        free(buffer[i].name);
    free(buffer);

    fclose(outputFile);

    printf("Mtar file created succesfully \n");
    return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[]) {
    FILE * extractedTar = fopen(tarName, "r");
    int nFiles;
    stHeaderEntry * tarHeader = readHeader(extractedTar, &nFiles);
    FILE * new;

    if (extractedTar == NULL) {
        printf("Error opening .tar file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nFiles; i++) {
        new = fopen(tarHeader[i].name, "w");

        if (new == NULL) {
            printf("Error creating file %s", tarHeader[i].name);
            return EXIT_FAILURE;
        }
        copynFile(extractedTar, new, tarHeader[i].size); //asignar a entero
        fclose(new);
    }

    for (int i = 0; i < nFiles; i++) 
        free(tarHeader[i].name);

    free(tarHeader);
    fclose(extractedTar);
    //se libera iterando el array y borrando los espacios de array que teniamos reservado para los nombres con malloc
    //despues de iterar, borramos el array
	return EXIT_SUCCESS;
}

int
listTar(char tarName[]) {
    FILE * extractedTar = fopen(tarName, "r");
    int nFiles;
    stHeaderEntry * tarHeader = readHeader(extractedTar, &nFiles);

      if (extractedTar == NULL) {
        printf("Error opening .tar file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nFiles; i++)
        printf("%s \n",tarHeader[i].name);

    return EXIT_SUCCESS;
}