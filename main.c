#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

// GLOBAL VARIABLES
int oldFileCount = 0;
int newFileCount = 0;
int sensitivity = 2;
char* oldFileName;
char* newFileName;

// An array struct to hold an array
// and the arrtibutes about the array
typedef struct 
{
    char** array;
    size_t size;
    size_t used;
    
} Array;

// initializes an array passed to it, with a given size
void initArray(Array* a, size_t initialSize) 
{
  a->array = (char**)malloc(initialSize * sizeof(*a->array));
  a->used = 0;
  a->size = initialSize;
}

// instert an element into an array
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

// frees a given array from memory
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

// search an array if it contains a certain element
// returns 1 if the array contains the element
// returns 0 if the array DOES NOT contain the element
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

// removes an element from an array,
// using the elements index
void arrayRemove(Array* a, int index)
{
    int i;
    for(i = index; i < a->used; i++)
    {
        a->array[i] = realloc(a->array[i], sizeof(char) * 254);
        a->array[i] = a->array[i+1];
    }
    char** tmp = realloc(a->array, (a->used - 1) * sizeof(*a->array) );
    a->used = a->used - 1;
    a->array = tmp;
}

// get the last modified time a file
char* getFileTime(char* filepath)
{
    struct stat tempTime;
    stat(filepath, &tempTime);
    return ctime(&tempTime.st_mtime);
}

// read the file and store the lines into
// a dynamically allocated array
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
            insertArray(&file, word);
            count++;
            i++;
            //word = "";
        }
    }
    else
    {
        printf("File \"%s\" was empty!\n", filepath);
        return NULL;
    }
        
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
    return file.array;
}

// splits a string (sentence in this context)
// into an array of words
void split_str(Array* temp, char* string)
{
    char* word = (char*)malloc(strlen(string)+1);
    strcpy(word, string);
    char* token = strtok(word, " ");
    
    while(token != NULL)
    {
        //printf("%s\n", token);
        insertArray(temp, token);
        token = strtok(NULL, " ");
    }
    
    free(word);
}

// MAIN APPLICATION
int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf("Enter two files to compare!\n");
        return -1;
    }
    register char* messageOut = malloc(sizeof(char)*254);
    strcpy(messageOut, "");
    register char* temp; // = malloc(sizeof(char) * 254);
    
    oldFileName = argv[1];
    newFileName = argv[2];
    
    char** oldFile = malloc(2048 * sizeof(char));
    oldFile = readFile(oldFileName);

    char** newFile = malloc(2048 * sizeof(char));
    newFile = readFile(newFileName);
    
    char* oldFileTime = getFileTime(oldFileName);
    printf("Last modified time for %s: %s\n",oldFileName, oldFileTime);
    temp = malloc(sizeof(char) * 254);
    sprintf(temp, "Last modified time for %s: %s\n",oldFileName, oldFileTime);
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    
    char* newFileTime = getFileTime(newFileName);
    printf("Last modified time for %s: %s\n",newFileName, newFileTime);
    temp = realloc(temp, sizeof(char) * 254);
    sprintf(temp, "Last modified time for %s: %s\n",newFileName, newFileTime);
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    free(temp);
    temp = "";
    
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
    
    // Do the nasty work of checking between the added and removed items
    // to see what was "changed" between each file
    Array changedLines;
    initArray(&changedLines, 2);
    int score_exact = 0;
    int score_succession = 0;
    int t;
    register char* oldWord = (char*)malloc(sizeof(char)*255);
    register char* newWord = (char*)malloc(sizeof(char)*255);
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

            split_str(&newWords, addedLines.array[j]);
            
            char* address = addedLines.array[j];
            
            // if the the first words of each line match
            // cycle word by word to test exactness of
            // each line, if exact enought (sensitivity)
            // then the lines would resemble that of a change
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
                }
                
                if((score_exact + score_succession) > sensitivity)
                {
                    insertArray(&changedLines, removedLines.array[i]);
                    insertArray(&changedLines, addedLines.array[j]);
                }
                score_exact = 0;
                score_succession = 0;
            }
            // clean up the array for new words
            freeArray(&newWords);
        }
        // clean up the array for old words
        freeArray(&oldWords);
    }
    
    // Write out what was changed
    printf("Changed from %s ---> %s\n--------------------------\n", oldFileName, newFileName);
    temp = malloc(sizeof(char) * 254);
    sprintf(temp, "Changed from %s ---> %s\n--------------------------\n", oldFileName, newFileName);
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    for(i = 0; i < changedLines.used; i+=2)
    {
        printf("%s ---> %s\n", changedLines.array[i], changedLines.array[i+1]);
        temp = realloc(temp, sizeof(char) * 254);
        sprintf(temp, "%s ---> %s\n", changedLines.array[i], changedLines.array[i+1]);
        messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
        strcat(messageOut, temp);
    }
    printf("\n--------------------------\n\n");
    temp = realloc(temp, sizeof(char) * 254);
    sprintf(temp, "\n--------------------------\n\n");
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    free(temp);
    temp = "";

    
    // Write out what was removed
    printf("Removed from %s\n--------------------------\n", oldFileName);
    temp = malloc(sizeof(char) * 254);
    sprintf(temp, "Removed from %s\n--------------------------\n", oldFileName);
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    for(i = 0; i < removedLines.used; i++)
    {
        // do a quick check to make sure the item isnt
        // a changed item
        if(arrayContains(&changedLines, removedLines.array[i]) == 1)
            continue;
            
        printf("%s", removedLines.array[i]);
        temp = realloc(temp, sizeof(char) * 254);
        sprintf(temp,"%s", removedLines.array[i]);
        messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
        strcat(messageOut, temp);
    }
    printf("\n--------------------------\n\n");
    temp = realloc(temp, sizeof(char) * 254);
    sprintf(temp, "\n--------------------------\n\n");
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    free(temp);
    temp = "";
    
    // Write out what was added
    printf("Added from %s\n--------------------------\n", newFileName);
    temp = malloc(sizeof(char) * 254);
    sprintf(temp, "Added from %s\n--------------------------\n", newFileName);
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    for(i = 0; i < addedLines.used; i++)
    {
        // do a quick check to make sure the item isnt
        // a changed item
        if(arrayContains(&changedLines, addedLines.array[i]) == 1)
            continue;
            
        printf("%s", addedLines.array[i]);
        temp = realloc(temp, sizeof(char) * 254);
        sprintf(temp,"%s", addedLines.array[i]);
        messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
        strcat(messageOut, temp);
    }
    printf("\n--------------------------\n");
    temp = realloc(temp, sizeof(char) * 254);
    sprintf(temp, "\n--------------------------\n\n");
    messageOut = realloc(messageOut, strlen(temp) + strlen(messageOut) + 1);
    strcat(messageOut, temp);
    strcat(messageOut, "\0");
    free(temp);
    temp = "";
    
    //printf("Replay:\n%s\n", messageOut);
    
    // *Optional* write out to a file that is .csv format
    // and formatted to be compatible with MS Excel (or any other spreadsheet software)
    FILE* outFile;
    char* out_filename = malloc(sizeof(char) * 254);
    strcpy(out_filename, "");
    time_t ti = time(NULL);
    struct tm tm = *localtime(&ti);
    sprintf(out_filename, "diff_logs/diff_output_%d%d%d_%d%d%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    outFile = fopen(out_filename, "w");
    fputs(messageOut, outFile);
    fclose(outFile);
    free(messageOut);
    messageOut = "";
    
    printf("Done! Log saved under %s\n", out_filename);
    free(out_filename);
    out_filename = "";
    
    // free some other stuff from memory that
    // were allocated earlier
    freeArray(&removedLines);
    freeArray(&addedLines);
    freeArray(&changedLines);
    free(oldFile);
    free(newFile);
    
    // Done exit
    return 0;
}