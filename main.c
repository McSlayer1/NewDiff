#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct {
    char* array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = (char *)malloc(initialSize * sizeof(char));
  a->used = 0;
  a->size = initialSize;
}


void insertArray(Array* a, char* element) {
  //printf("%s\n", element);
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (char *)realloc(a->array, a->size * sizeof(char));
  }
  a->array[a->used] = (char*)malloc(2*sizeof(element));
  a->array[a->used] = *element;
  a->used++;
}


void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

char* getFileTime(char* filepath)
{
    struct stat tempTime;
    stat(filepath, &tempTime);
    return ctime(&tempTime.st_mtime);
}

char** readFile(char* filepath)
{
    printf("Read file\n");
    FILE* temp = fopen(filepath, "r");
    Array file;
    initArray(&file, 1);
    int i = 0;
    char* word;
    if(temp != NULL)
    {
        while(!feof(temp))
        {
            fscanf(temp, "%s", word);
            printf("%s\n", word);
            insertArray(&file, word);
            i++;
        }
    }
    else
    {
        printf("File \"%s\" was empty!\n", filepath);
        return NULL;
    }
    i = file.size;
    printf("%d\n", i);
    int x = 0;
    for(x = 0; x < i; x++)
    {
        printf("%s\n", &file.array[x]);
    }
    char** returnFile = &file.array;
    //freeArray(&file);
    return returnFile;
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
    
    char** oldFile = readFile(oldFileName);
    char** newFile = readFile(newFileName);
    
    
    char* oldFileTime = getFileTime(oldFileName);
    printf("Last modified time for %s: %s\n",oldFileName, oldFileTime);
    
    char* newFileTime = getFileTime(newFileName);
    printf("Last modified time for %s: %s\n",newFileName, newFileTime);
    
    printf("%lu\n", sizeof(oldFile));
    printf("%lu\n", sizeof(oldFile[0]));
    int len = sizeof(oldFile) / sizeof(oldFile[0]);
    printf("%d\n", len);
    int i;
    for(i = 0; i < len; i++)
    {
        printf("%s\n", oldFile[i]);
    }
    
    return 0;
}