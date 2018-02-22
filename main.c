#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct {
    char** array;
    size_t size;
    size_t used;
    
} Array;

void initArray(Array* a, size_t initialSize) {
  a->array = (char**)malloc(initialSize * sizeof(*a->array));
  a->used = 0;
  a->size = initialSize;
  //int i;
  //for (i = 0; i < a->size; i++)
    //a->array[i] = (char*)malloc(256 * sizeof(char));
}

void insertArray(Array* a, char* element) {
  //printf("%s\n", element);
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size = a->size + 1;
    a->array = realloc(a->array, a->size * sizeof(*a->array));
  }
  a->array[a->used] = malloc(254*sizeof(char)); 
  sprintf(a->array[a->used++], "%s", element);
  
}

void freeArray(Array* a) {
  int i;
  for (i = 0; i < a->size; i++)
  {
    free(a->array[i]);
    //a->array[i] = NULL;
  }
    
  free(a->array);
  //a->array = NULL;
  a->used = 0;
  a->size = 0;
}

char* getFileTime(char* filepath)
{
    struct stat tempTime;
    stat(filepath, &tempTime);
    return ctime(&tempTime.st_mtime);
}

char** readFile(char* filepath)
{
    //printf("Read file\n");
    FILE* temp = fopen(filepath, "r");
    Array file;
    initArray(&file, 2);
    int i = 0;
    if(temp != NULL)
    {
        while(!feof(temp))
        {
            char* word;
            fscanf(temp, "%s", word);
            strcat(word, "\0");
            //printf("%s\n", word);
            insertArray(&file, word);
            //printf("size: %lu\n", sizeof(file.array));
            i++;
            //word = "";
        }
    }
    else
    {
        printf("File \"%s\" was empty!\n", filepath);
        return NULL;
    }
    //for(i=0; i<file.size;i++)
    //    printf("Callback: %s\n", file.array[i]);
        
    char** send = file.array;
    //freeArray(&file);
    //printf("Returning File!\n");
    return file.array;
}

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf("Enter two files to compare!\n");
        return -1;
    }
    
    char* oldFileName = argv[1];
    char* newFileName = argv[2];
    
    //printf("Collect old file...\n");
    char** oldFile = malloc(2048 * sizeof(char));
    oldFile = readFile(oldFileName);
    //printf("Old File[0]: %s\n", oldFile[0]);
    
    //printf("Collect new file...\n");
    char** newFile = malloc(2048 * sizeof(char));
    newFile = readFile(newFileName);
    
    char* oldFileTime = getFileTime(oldFileName);
    printf("Last modified time for %s: %s\n",oldFileName, oldFileTime);
    
    char* newFileTime = getFileTime(newFileName);
    printf("Last modified time for %s: %s\n",newFileName, newFileTime);
    
    return 0;
}