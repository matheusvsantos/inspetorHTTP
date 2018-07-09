#include <sys/types.h>
#include <sys/param.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void read_sub(char *subdir, FILE *fp){
    DIR *dirp = opendir(subdir);
    if (!dirp)
        return;

    struct dirent *dentry;
    while ((dentry = readdir(dirp)) != 0)
    {
        if (strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)
            continue;

        fprintf(fp, "%s/", subdir);
        printf("%s/", subdir);

        char path[4096];
        int len = strlen(subdir) + 1 + strlen(dentry->d_name) + 1;
        if (len >= sizeof(path))
        {
            printf("path is too long...\n");
            break;
        }
        sprintf(path, "%s/%s", subdir, dentry->d_name);

        struct stat st;
        stat(path, &st);
        if (S_ISDIR(st.st_mode))
        {
            fprintf(fp, "%s\n", dentry->d_name);
            printf("%s", dentry->d_name);
            read_sub(path,fp);
        }
        else
        {
            fprintf(fp, "%s\n", dentry->d_name);
            printf("%s\n", dentry->d_name);
        }
    }
    closedir(dirp);
}

int main() {
  FILE *fp = fopen("tree.txt", "w+");
  read_sub("simpletest.org", fp);
  fclose(fp);
  return 0;
}
