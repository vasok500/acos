#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
void List(char* path);
void ListDir(char* path);
void Print(char *path);

int aflag,lflag, rflag;
void GetKeys(int argc, char *argv[])
{
    int ch;
    while ((ch = getopt(argc,argv,"alR")) != -1)
    {
        switch (ch)
        {
            case 'a':
                     aflag = 1;
                     break;
            case 'l':
                     lflag = 1;
                     break;
            case 'R':
                     rflag= 1;
                     break;
            default:
                     fprintf(stderr, "incorect argument");
                     exit(1);

        }    
    }
}
void List(char* path)
{
    int k = 0;
    struct stat st;
    if (path == NULL)
    { 
        path = (char*)malloc(2);
        strcpy(path,".");
        k = 1;  
    }  
    if (lstat(path,&st))
    {
        fprintf(stderr,"Error: path or file  does not exist.\n");
        return;
    }
    if (S_ISDIR(st.st_mode))
    {  
        ListDir(path); 
        if (k==1)
        free(path);
    }  
    else if (S_ISREG(st.st_mode))
    {  
        Print(path); 
    }  
    else
    {  
        fprintf(stderr,"Not a regular file or directory.\n");
    return;
    } 
}
void ListDir(char* path)
{
    DIR *dp;
    struct dirent *dirp; 
    struct stat st;
    if ((dp = opendir(path)) == NULL)
    {
        fprintf(stderr,"can't open directory\n");
        return;
    }
    chdir(path);  
    while ((dirp = readdir(dp)) != NULL)
    {
        if (!(strcmp(dirp->d_name, ".")) || !(strcmp(dirp->d_name, "..")) || (dirp->d_name[0] == '.'))
        {
            if (aflag == 0)
            {
                continue;
            }   
        }
      Print(dirp->d_name); 
      if(S_ISDIR(dirp->d_type) & rflag) ListDir(dirp->d_name);
    } 
    if (closedir(dp))
    {
       fprintf(stderr,"can't close the directory \n");
       return;
    }
}
void Print(char *path)
{
    struct stat st;
    char *fileName = NULL;
    char time[64];
    struct passwd *p;
    if (lstat(path, &st))
    {
        fprintf(stderr,"file does not exist.\n");
        return;
    }
    if ((fileName = strrchr(path, '/')) != NULL)
    {  
        fileName++;  
    } 
    else
    {
        fileName = path;
    }
    if (lflag == 1)              
    {
        (S_ISDIR(st.st_mode)) ? printf("d"): printf("-");
        (st.st_mode & S_IRUSR) ? printf("r"): printf("-");
        (st.st_mode & S_IWUSR) ? printf("w"): printf("-");
        (st.st_mode & S_IXUSR) ? printf("x"): printf("-");
        (st.st_mode & S_IRGRP) ? printf("r"): printf("-");
        (st.st_mode & S_IWGRP) ? printf("w"): printf("-");
        (st.st_mode & S_IXGRP) ? printf("x"): printf("-");
        (st.st_mode & S_IROTH) ? printf("r"): printf("-");
        (st.st_mode & S_IWOTH) ? printf("w"): printf("-");
        (st.st_mode & S_IXOTH) ? printf("x"): printf("-");
        if ((p = getpwuid(st.st_uid))==NULL)
    fprintf(stderr,"Can't identify the user name");
        printf(" %s  ",p->pw_name);
        printf("%d  ",p->pw_gid);
        printf("%10ld ", st.st_size);
        strftime(time, sizeof(time), "%Y-%m-%d %H:%M", localtime(&st.st_mtime));
        printf("%s %s\n",time, fileName);
    }
    else
    {
        printf("%s\n", fileName);
    }
}
int main(int argc, char* argv[])
{
    if (argc==1)
    {
        List(NULL);
        return 0;
    }
    GetKeys(argc,argv);
    if (optind == argc)
    {
        List(NULL);
        return 0;
    }
    int i;
    for ( i = optind;i<argc;++i)
    {
        List(argv[i]);
    }
    return 0;
}
