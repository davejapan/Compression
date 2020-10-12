#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/sendfile.h>

//thread parameters structure
struct thread_data{
  unsigned char thread_id;
  char *in;
  size_t size;
  FILE *out_fp;
};

//encoder function definition
void* decoder(void *);
//decoder function definition
void* encoder(void *);
