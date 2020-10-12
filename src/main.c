#include "rle.h"

int main(int argc, char *argv[])
{
  if(argc< 4)
  {
    printf("Format : run_length_encoding <encode/decode> <in_filename> <out_filename> [Optional:<Number of threads>]\n");
    return -1;
  }

  if(strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0)
  {
    printf("Format : run_length_encoding <encode/decode> <in_filename> <out_filename> [Optional:<Number of threads>]\n");
    return -2;
  }

  FILE *in_fp = NULL;
  if((in_fp= fopen(argv[2], "r")) == NULL)
  {
    return -3;
  }

  unsigned int no_of_threads=0;
  if(argc == 5)
  {
    sscanf(argv[4], "%d", &no_of_threads);
  }
  else
  {
    no_of_threads = 1;
  }

  long lSize;
  char *buffer;
  size_t result;

  // obtain file size:
  fseek (in_fp , 0 , SEEK_END);
  lSize = ftell (in_fp);
  rewind(in_fp);

  // allocate memory to contain the whole file:
  if((buffer = (char*) malloc (sizeof(char)*lSize)) == NULL)
  {
    return -4;
  }

  // copy the file into the buffer:
  result = fread (buffer, 1, lSize, in_fp);
  if(result != lSize)
  {
    return -5;
  }

  //Thread and data structure initiation
  pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t)*no_of_threads);
  struct thread_data *p_thread_data = (struct thread_data*) malloc(sizeof(struct thread_data)*no_of_threads);

  int rc;
  unsigned char i;
  char out_file_name[THREAD_OUTPUT_FILE_NAME_SIZE] = {};

  //Iterate over threads
  for(i=0; i<no_of_threads; i++)
  {
    //initialize thread function argument structure
    p_thread_data[i].thread_id = i;
    p_thread_data[i].in = buffer;

    if(strcmp(argv[1], "encode") == 0)
    {
      if(i != 0)
      {
        p_thread_data[i].in += p_thread_data[i-1].size;
      }

      p_thread_data[i].size = lSize/no_of_threads;
      if(i < lSize%no_of_threads)
      {
        p_thread_data[i].size++;
      }

      rc = pthread_create(&threads[i], NULL, encoder, (void *)(&p_thread_data[i]));
      if (rc)
      {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
      }
    }
    else
    {
      if(i != 0)
      {
        p_thread_data[i].in += p_thread_data[i-1].size;
      }

      p_thread_data[i].size = ((lSize>>1)/no_of_threads);
      if(i < (lSize>>1)%no_of_threads)
      {
        p_thread_data[i].size++;
      }
      p_thread_data[i].size <<= 1;

      rc = pthread_create(&threads[i], NULL, decoder, (void *)(&p_thread_data[i]));
      if (rc)
      {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
      }
    }
  }

  //Wait for threads to complete
  for(i=0; i<no_of_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  //Input file and buffer cleanup
  free(buffer);
  fclose(in_fp);

  //Merging files produced by threads
  int output_file, input_file;
  if((output_file= open(argv[3], O_WRONLY | O_CREAT, 0644)) < 0)
  {
    return -5;
  }

  //Merging files produced by threads
  for(i=0; i<no_of_threads; i++)
  {
    sprintf(out_file_name, "%d", p_thread_data[i].thread_id);
    if((input_file= open(out_file_name, O_RDONLY)) < 0)
    {
      return -9;
    }

    struct stat st;
    stat(out_file_name, &st);
    lSize = st.st_size;

    //Using sendfile instead of cat/fgetc for performance
    if(sendfile( output_file, input_file, 0, lSize) != lSize)
    {
        return -10;
    }
    //Deleting files produced by threads
    remove(out_file_name);
  }

  //Closing opened files
  close(output_file);
  close(input_file);

  return 0;
}
