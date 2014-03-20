#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define MAX_SIZE 1024

void print_file(FILE* f,int unbuf)
{
  char buf[MAX_SIZE];
  int r;
  if (f == NULL)
   return;
  if (unbuf)
  {
	while((r = fread(buf,sizeof(char),1,f))>0)
	{
		write(1,buf,1);
	}	
  }
  else
  {
    while(!feof(f))
   {
     if (fgets(buf,MAX_SIZE,f) == NULL)
       return;
     fputs(buf,stdout);
   }
  }
return;
}

int main(int argc, char* argv[])
{
  int i,r;
  int unbuf = 0;
  if (argc==1)
  {
	print_file(stdin,0);
  }
  for(i = 0; (i < (argc-1)) && (!unbuf); i++)
  {
   if (!strcmp("-u", argv[i+1]))
   unbuf = 1;
  }
  if (argc - unbuf == 1)
{
	char buf[1];
	while ((r = read(0, buf ,1)) > 0)
	write(1,buf,1);			
}
  else
  {
    for(i = 0; i < (argc-1); i++)
    if (strcmp("-u", argv[i+1]))
    {
      FILE*  f = fopen(argv[i+1],"r");
      if (!f)
      {
        perror("Ошибка при открытии файла");
        exit(1);
      }
     print_file(f,unbuf);
     fclose(f);
     }
   }
return 0;
}


