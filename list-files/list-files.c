#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void EnumerateDirs(char* path);
int AreEqual(char* str1, char* str2);

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("%s\n", argv[1]);
        EnumerateDirs(argv[1]);
    }
    else
    {
        printf("Usage: dirlist.exe <path>\n");        
    }
    return 0;
}

void EnumerateDirs(char *path)
{
    DIR *pDir;
    struct dirent *dir;
    pDir = opendir(path);
    if (pDir == NULL)
    {
        return;
    }
    
    while ((dir =readdir(pDir)) != NULL)
    {
        if (AreEqual(dir->d_name, ".") ||
            AreEqual(dir->d_name, ".."))
            {
                continue;
            }
            
        if (dir->d_type != DT_DIR)
        {
            printf("%s/%s \n", path, dir->d_name);
        }
        else
        {
            printf("%s/%s\n",path, dir->d_name);
            char *nextDirPath = (char*)malloc(strlen(path) + 1 + strlen(dir->d_name) + 1);
            sprintf(nextDirPath, "%s/%s", path, dir->d_name);
            EnumerateDirs(nextDirPath);
            free(nextDirPath);
        }
    }
    
    closedir(pDir);  
}

int AreEqual(char* str1, char* str2)
{
    return strcmp(str1, str2) == 0;
}