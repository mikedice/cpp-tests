#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void StartEnumerateDirs(char* dir);
void EnumerateDirs(DIR *pDir, char* path);
int AreEqual(char* str1, char* str2);

int main(void)
{
    StartEnumerateDirs("/home/pi/code/c++");
    return 0;
}

void StartEnumerateDirs(char *path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        EnumerateDirs(d, path);
        closedir(d);
    }   
}

void EnumerateDirs(DIR *pDir, char *path)
{
    struct dirent *dir;
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
            DIR* next = opendir(nextDirPath);
            EnumerateDirs(next, nextDirPath);
            free(nextDirPath);
            closedir(next);
        }
    }

    
}

int AreEqual(char* str1, char* str2)
{
    return strcmp(str1, str2) == 0;
}