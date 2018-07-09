#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void cut_string_char(char *src, char *dest, int first_index, char second_index)
{
  int j = 0;
  for (int i = first_index; src[i] != second_index; i++)
  {
    if (src[i] != ' ' && src[i] != '\r')
    {
      dest[j] = src[i];
      j++;
    }
  }
  dest[j] = '\0';
}


int main(void) {
  FILE *fp, *ftree;
  int i = 0;
  char *line = NULL, buffer[200];
  size_t len = 0;
  ssize_t read;
  char *index;


  ftree = fopen("test.txt", "w+");
  fp = fopen("index.html", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    if ((index = strstr(line,"href=\"")) != NULL) {
      if (strstr(line,"http") == NULL) {
        i = index - line + 6;
        cut_string_char(line,buffer,i,'\"');
        fprintf(ftree, "%s\n",buffer );
      }else if (strstr(line,"simpletest.org") != NULL){
        index = strstr(line,"http");
        i = index - line;
        cut_string_char(line,buffer,i,'\"');
        fprintf(ftree, "%s\n",buffer );
      }
    }

    if ((index = strstr(line,"src=\"")) != NULL) {
      if (strstr(line,"http") == NULL) {
        i = index - line + 5;
        cut_string_char(line,buffer,i,'\"');
        fprintf(ftree, "%s\n",buffer );
      }else if (strstr(line,"simpletest.org") != NULL){
        index = strstr(line,"http");
        i = index - line;
        cut_string_char(line,buffer,i,'\"');
        fprintf(ftree, "%s\n",buffer );
      }
    }
  }

  free(line);
  fclose(ftree);
  fclose(fp);
  exit(EXIT_SUCCESS);
}
