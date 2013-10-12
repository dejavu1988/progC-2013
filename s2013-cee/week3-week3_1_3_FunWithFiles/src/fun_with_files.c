#include <stdio.h>
#include <string.h>
#include "fun_with_files.h"

/* Exercise 1 */
int append_string(char *file_name) {
    FILE *fp;
    int ch;

    if ( (fp = fopen(file_name, "a+")) == NULL) {
	return -1;
    }
    while ( (ch = getchar()) != EOF)
	fputc(ch, fp);
    fclose(fp);
  return 0;
}

/* Exercise 2 */
int remove_empty_lines(char *file_name) {
    FILE *fp, *fp2;
    char str[50];
    int i;

    if ( (fp = fopen(file_name, "r+")) == NULL) {
	return 1;
    }
    if ( (fp2 = fopen("tmp.txt", "w+")) == NULL) {
	return -1;
    }
    while( fgets (str, 50, fp)!=NULL ) {
        if(!(str[0]=='\n') && !(str[0]=='\r' && str[1]=='\n')){
            fputs(str, fp2);
        }
    }
    fclose(fp);
    fclose(fp2);
    rename("tmp.txt",file_name);
  return 0;
}

/* Exercise 3 */
int count_characters_in_file(char *filename) {
    FILE *fp;
    int count = 0;
    char c;
    if ( (fp = fopen(filename, "r")) == NULL) {
	return -1;
    }
    while((c = fgetc(fp)) != EOF){
      count++;
    }
    fclose(fp);
  return count;
}
