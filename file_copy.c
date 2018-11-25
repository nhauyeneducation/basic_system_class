#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define LOG printf

#ifdef DEBUG_ENABLE
#define DEBUG printf
#else
#define DEBUG
#endif

#define BUFSIZE 1000

enum error_s
{
    SUCCESS = 0,
    ERR_OPEN = -100, 
    ERR_READ,
};

int main(int argc, char *argv[])
{
   int source_fd, dest_fd, openflags;
   int numread; 
   mode_t fileperm; 
   char rbuf[BUFSIZE] = {0};

   source_fd = open(argv[1], O_RDONLY);
   if(source_fd < 0)
   {
      LOG("open error %d %s\n", errno, (char *)strerror(errno)); 
      return ERR_OPEN;       
   }    

   fileperm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
   dest_fd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, fileperm);
   if(dest_fd < 0)
   {
      LOG("open 2 error %d %s\n", errno, (char *)strerror(errno));
      return ERR_OPEN;
   }  

   while((numread = read(source_fd, rbuf, BUFSIZE)) > 0)
   {
       DEBUG("numread = %d\n", numread);  
       write(dest_fd, rbuf, numread);  
   }
   if(numread < 0)
   {
       return ERR_READ; 
   }
   
   close(source_fd);
   close(dest_fd);     
   return SUCCESS;
}
