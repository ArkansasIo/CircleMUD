/* code to convert 2.20 shop files to 3.0 shop files - written by Jeff Fink */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "utils.h"
#include "shop.h"

void basic_mud_log(const char *x, ...)
{
  puts(x);
}

char *fread_string(FILE * fl, const char *error)
{
  char buf[MAX_STRING_LENGTH], tmp[512], *rslt;
  size_t length, end;
  int flag;

  *buf = '\0';

  do {
    if (!fgets(tmp, sizeof(tmp), fl)) {
      printf("fread_string: format error at or near %s\n", error);
      exit(1);
    }
    if (strlen(tmp) + strlen(buf) >= sizeof(buf)) {
      printf("SYSERR: fread_string: string too large (shopconv.c)");
      exit(1);
    } else
      strcat(buf, tmp);

    length = strlen(buf);
    end = length;
    if (end > 0 && buf[end - 1] == '\n')
      end--;
    while (end > 0 && isspace((unsigned char)buf[end - 1]))
      end--;
    flag = end > 0 && buf[end - 1] == '~';
    if (flag)
      buf[end - 1] = '\0';
  } while (!flag);

  /* do the allocate boogie  */

  if (strlen(buf) > 0) {
    CREATE(rslt, char, strlen(buf) + 1);
    strcpy(rslt, buf);
  } else
    rslt = NULL;
  return (rslt);
}


void do_list(FILE * shop_f, FILE * newshop_f, int max)
{
  int count, temp;
  char buf[MAX_STRING_LENGTH];

  for (count = 0; count < max; count++) {
    fscanf(shop_f, "%d", &temp);
    fgets(buf, MAX_STRING_LENGTH - 1, shop_f);
    if (temp > 0)
      fprintf(newshop_f, "%d%s", temp, buf);
  }

  fprintf(newshop_f, "-1\n");
}


void do_float(FILE * shop_f, FILE * newshop_f)
{
  float f;
  char str[20];

  fscanf(shop_f, "%f \n", &f);
  sprintf(str, "%f", f);
  while ((str[strlen(str) - 1] == '0') && (str[strlen(str) - 2] != '.'))
    str[strlen(str) - 1] = 0;
  fprintf(newshop_f, "%s \n", str);
}


void do_int(FILE * shop_f, FILE * newshop_f)
{
  int i;

  fscanf(shop_f, "%d \n", &i);
  fprintf(newshop_f, "%d \n", i);
}


void do_string(FILE * shop_f, FILE * newshop_f, char *msg)
{
  char *ptr;

  ptr = fread_string(shop_f, msg);
  fprintf(newshop_f, "%s~\n", ptr ? ptr : "");
  free(ptr);
}


int boot_the_shops(FILE * shop_f, FILE * newshop_f, char *filename)
{
  char *buf, buf2[150];
  int temp, count;

  sprintf(buf2, "beginning of shop file %s", filename);
  fprintf(newshop_f, "CircleMUD %s Shop File~\n", VERSION3_TAG);
  for (;;) {
    buf = fread_string(shop_f, buf2);
    if (*buf == '#') {		/* New shop */
      sscanf(buf, "#%d\n", &temp);
      sprintf(buf2, "shop #%d in shop file %s", temp, filename);
      fprintf(newshop_f, "#%d~\n", temp);
      free(buf);		/* Plug memory leak! */
      printf("   #%d\n", temp);

      do_list(shop_f, newshop_f, MAX_PROD);	/* Produced Items */

      do_float(shop_f, newshop_f);	/* Ratios */
      do_float(shop_f, newshop_f);

      do_list(shop_f, newshop_f, MAX_TRADE);	/* Bought Items */

      for (count = 0; count < 7; count++)	/* Keeper msgs */
	do_string(shop_f, newshop_f, buf2);

      for (count = 0; count < 5; count++)	/* Misc   */
	do_int(shop_f, newshop_f);
      fprintf(newshop_f, "-1\n");
      for (count = 0; count < 4; count++)	/* Open/Close     */
	do_int(shop_f, newshop_f);

    } else {
      if (*buf == '$') {	/* EOF */
	free(buf);		/* Plug memory leak! */
	fprintf(newshop_f, "$~\n");
	break;
      } else if (strstr(buf, VERSION3_TAG)) {
	printf("%s: New format detected, conversion aborted!\n", filename);
	free(buf);		/* Plug memory leak! */
	return (1);
      }
    }
  }
  return (0);
}

int main(int argc, char *argv[])
{
  FILE *sfp, *nsfp;
  char *fn;
  char *part, *backup;
  size_t path_length;
  int result, index;

  if (argc < 2) {
    printf("Usage: shopconv <file1> [file2] [file3] ...\n");
    exit(1);
  }
  for (index = 1; index < argc; index++) {
    fn = argv[index];
    path_length = strlen(fn);
    part = malloc(path_length + sizeof(".tmp"));
    backup = malloc(path_length + sizeof(".bak"));
    if (!part || !backup) {
      fprintf(stderr, "Unable to allocate path buffers for %s.\n", fn);
      free(part);
      free(backup);
      continue;
    }
    snprintf(part, path_length + sizeof(".tmp"), "%s.tmp", fn);
    snprintf(backup, path_length + sizeof(".bak"), "%s.bak", fn);
    if (rename(fn, part) != 0) {
      perror(fn);
      free(part);
      free(backup);
      continue;
    }
    sfp = fopen(part, "r");
    if (sfp == NULL) {
      perror(part);
      rename(part, fn);
    } else {
      if ((nsfp = fopen(fn, "w")) == NULL) {
	printf("Error writing to %s.\n", fn);
	perror(fn);
	fclose(sfp);
	rename(part, fn);
	free(part);
	free(backup);
	continue;
      }
      printf("%s:\n", fn);
      result = boot_the_shops(sfp, nsfp, fn);
      fclose(nsfp);
      fclose(sfp);
      if (result) {
	if (rename(part, fn) != 0)
	  perror(part);
      } else {
	remove(backup);
	if (rename(part, backup) != 0)
	  perror(part);
	else
	  printf("Done!\n");
      }
    }
    free(part);
    free(backup);
  }

  return (0);
}
