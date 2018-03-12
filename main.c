#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

// GLOBAL VARIABLES
int oldFileCount = 0;
int newFileCount = 0;
char* oldFileName;
char* newFileName;

typedef struct 
{
    char** array;
    size_t size;
    size_t used;
    
} Array;

void initArray(Array* a, size_t initialSize) 
{
  a->array = (char**)malloc(initialSize * sizeof(*a->array));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array* a, char* element) 
{
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
  for (i = 0; i < a->used; i++)
  {
    free(a->array[i]);
    //a->array[i] = NULL;
  }
    
  free(a->array);
  //a->array = NULL;
  a->used = 0;
  a->size = 0;
}

int arrayContains(Array* a, char* element)
{
    //bool found = false;
    int x = 0;
    
    for(x = 0; x < a->used; x++)
    {
        if(strcmp(a->array[x], element) == 0)
        {
            return 1;
        }
    }
    
    return 0;
}

char* getFileTime(char* filepath)
{
    struct stat tempTime;
    stat(filepath, &tempTime);
    return ctime(&tempTime.st_mtime);
}

char** readFile(char* filepath)
{
    int count = 0;
    FILE* temp = fopen(filepath, "r");
    Array file;
    initArray(&file, 2);
    int i = 0;
    if(temp != NULL)
    {
        while(!feof(temp))
        {
            char* word;
            size_t len = 0;
            //fscanf(temp, "%s", word);
            getline(&word, &len, temp);
            //strcat(word, "\0");
            //printf("%s\n", word);
            insertArray(&file, word);
            count++;
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
        
    if(strcmp(filepath, oldFileName) == 0)
    {
        oldFileCount = count;
    }
    else if(strcmp(filepath, newFileName) == 0)
    {
        newFileCount = count;
    }
    char** send = file.array;
    //freeArray(&file);
    //printf("Returning File!\n");
    return file.array;
}

void split_str(Array* temp, char* string)
{
    char* token = strtok(string, " ");
    
    while(token != NULL)
    {
        //printf("%s\n", token);
        insertArray(temp, token);
        token = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[])
{
    system("clear");
    if(argc <= 1)
    {
        printf("Enter two files to compare!\n");
        return -1;
    }
    
    oldFileName = argv[1];
    newFileName = argv[2];
    
    char** oldFile = malloc(2048 * sizeof(char));
    oldFile = readFile(oldFileName);

    char** newFile = malloc(2048 * sizeof(char));
    newFile = readFile(newFileName);
    
    char* oldFileTime = getFileTime(oldFileName);
    printf("Last modified time for %s: %s\n",oldFileName, oldFileTime);
    
    char* newFileTime = getFileTime(newFileName);
    printf("Last modified time for %s: %s\n",newFileName, newFileTime);
    
    // Make a common list of elements between both files
    Array commonLines;
    initArray(&commonLines, 2);
    int i = 0;
    int j = 0;
    //int countCommon = 0;
    for(i = 0; i < oldFileCount; i++)
    {
        //printf("Old word: %s\n", oldFile[i]);
        for(j = 0; j < newFileCount; j++)
        {
            //printf("New word: %s\n", newFile[j]);
            if(strcmp(oldFile[i], newFile[j]) == 0)
            {
                //printf("Found match!: %s = %s\n", oldFile[i], newFile[j]);
                insertArray(&commonLines, oldFile[i]);
                //countCommon++;
            }
            
            if(strcmp(newFile[j], "") == 0 || strcmp(newFile[j], "\0") == 0)
                break;
            
        }
        
        if(strcmp(oldFile[i], "") == 0 || strcmp(oldFile[i], "\0") == 0)
                break;
    }
    
    // Check what was removed between the files
    Array removedLines;
    initArray(&removedLines, 2);
    //int countRemoved = 0;
    //printf("Removed:\n");
    for(i = 0; i < oldFileCount; i++)
    {
        if(arrayContains(&commonLines, oldFile[i]) == 0)
        {
            //printf("%s", oldFile[i]);
            insertArray(&removedLines, oldFile[i]);
        }
    }
    
    // Check what was added between the files
    Array addedLines;
    initArray(&addedLines, 2);
    //printf("\n\nAdded:\n");
    for(i = 0; i < newFileCount; i++)
    {
        if(arrayContains(&commonLines, newFile[i]) == 0)
        {
            //printf("%s", newFile[i]);
            insertArray(&addedLines, newFile[i]);
        }
    }
    //printf("\n");
    
    // Do the nasty work of checking between the added and removed items
    // to see what was "changed" between each file
    Array changedLines;
    initArray(&changedLines, 2);
    int score_exact = 0;
    int score_succession = 0;
    int t;
    char* oldWord = (char*)malloc(sizeof(char)*255);
    char* newWord = (char*)malloc(sizeof(char)*255);
    for(i = 0; i < removedLines.used; i++)
    {
        Array oldWords;
        initArray(&oldWords, 2);
        //printf("removedLines.array[%d]: %s\n", i, removedLines.array[i]);
        split_str(&oldWords, removedLines.array[i]);
        for(j = 0; j < addedLines.used; j++)
        {
            Array newWords;
            initArray(&newWords, 2);
            
            printf("addedLines.array[%d]: %s\n", j, addedLines.array[j]);
            //printf("Address of: %p\n", addedLines.array[j]);
            split_str(&newWords, addedLines.array[j]);
            
            char* address = addedLines.array[j];
            
            //printf("OLD WORDS:\n");
            int z;
            for(z = 0; z < oldWords.used; z++)
            {
                //printf("%s\n", oldWords.array[z]);
            }
            
            //printf("NEW WORDS:\n");
            for(z = 0; z < newWords.used; z++)
            {
                //printf("%s\n", newWords.array[z]);
            }
            
            if(strcmp(oldWords.array[0], newWords.array[0]) == 0)
            {
                t = 0;
                while(1)
                {
                    //printf("t: %d\n", t);
                    if(t < oldWords.used)
                        strcpy(oldWord, oldWords.array[t]);
                    else
                        strcpy(oldWord, "");
                    //printf("Old: %s\n", oldWord);
                        
                    if(t < newWords.used)    
                        strcpy(newWord, newWords.array[t]);
                    else
                        strcpy(newWord, "");
                    //printf("New: %s\n", newWord);
                    
                    if(strcmp(oldWord, newWord) == 0)
                    {
                        //printf("Old: %s\n", oldWord);
                        //printf("New: %s\n", newWord);
                        score_exact++;
                        score_succession++;
                    }
                    else
                    {
                        score_exact--;
                        score_succession = 0;
                    }
                    
                    if(strcmp(oldWord, "") == 0 && strcmp(newWord, "") == 0)
                        break;
                        
                    t++;
                    //printf("next\n");
                }
                
                if((score_exact + score_succession) > 0)
                {
                    printf("A change!!!!\n");
                    insertArray(&changedLines, removedLines.array[i]);
                    insertArray(&changedLines, addedLines.array[j]);
                }
                score_exact = 0;
                score_succession = 0;
            }
            
            //printf("Freeing newWords\n");
            freeArray(&newWords);
        }
        //printf("Freeing oldWords\n");
        freeArray(&oldWords);
        //break;
    }
    
    // filter out the changed items from the added and removed items
    
    // Write out what was changed
    //printf("Changed from %s ---> %s\n--------------------------\n", oldFileName, newFileName);
    for(i = 0; i < changedLines.used; i+=2)
    {
        printf("%s ---> %s\n", changedLines.array[i], changedLines.array[i+1]);
    }
    
    // Write out what was removed
    
    // Write out what was added
    
    // *Optional* write out to a file that is .csv format
    // and formatted to be compatible with MS Excel (or any other spreadsheet software)
    
    // Done exit
    return 0;
}