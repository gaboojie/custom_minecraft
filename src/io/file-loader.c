#include <stdlib.h>
#include <stdio.h>
#include "file-loader.h"

char* load_file(char* path) {
  FILE *fp;
  long size;
  char *buffer;
  fp = fopen(path, "rb");
  if (!fp) fputs("ERROR: Error opening shader file", stderr),perror(path),exit(1);

  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  ///* allocate memory for entire content */
  buffer = calloc(1, size+1);
  if (!buffer) fclose(fp),fputs("ERROR: Error allocating buffer to open shader file", stderr),exit(1);

  ///* copy the file into the buffer */
  if(1 != fread(buffer, size, 1 , fp)) fclose(fp),free(buffer),fputs("ERROR: Trying to read shader file data failed!",stderr),exit(1);

  // Remember to free(buffer)
  fclose(fp);
  return buffer;
}
