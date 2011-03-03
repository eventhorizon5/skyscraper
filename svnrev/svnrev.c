/*  SvnRev
 *
 *  This utility retrieves the highest number that follows the "$Id: $" keyword
 *  or a combination of the $Rev: $ and $Date: $ keywords. The Subversion
 *  version control system expands these keywords and keeps them up to date.
 *  For an example of the tag, see the end of this comment.
 *
 *  Details on the usage and the operation of this utility is available on-line
 *  at http://www.compuphase.com.
 *
 *
 *  License
 *
 *  Copyright (c) 2005-2006, ITB CompuPhase (www.compuphase.com).
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 *
 * Version: $Id: svnrev.c 3629 2006-08-09 08:23:35Z thiadmer $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svnrev.h"

static void about(void)
{
  printf("svnrev 1.3." SVN_REVSTR "\n\n");
  printf("Usage: svnrev [-ofilename] [-i] <input> [input [...]]\n\n"
         "The -o option sets the filename for the output file with the build number.\n"
         "When no filename follows \"-o\", the result is written to stdout.\n\n"
         "The -i option should be used when the file list is only a partial set of all\n"
         "files. With this option, svnrev also scans the output file (default=\"svnrev.h\")\n"
         "that was generated on a previous run.\n");
  exit(1);
}

static void processfile(const char *name, int failsilent,
                        int *max_build, int *accum_build,
                        int *max_year, int *max_month, int *max_day)

{
  char str[512];
  char *p1;
  FILE *fp;
  int build, maj_build;
  int year, month, day;

  fp = fopen(name, "r");
  if (fp == NULL) {
    if (!failsilent)
      fprintf(stderr, "Failed to open input file '%s'\n", name);
    return;
  } /* if */
  build = 0;
  maj_build = 0;      /* RCS / CVS */
  year = month = day = 0;

  while (fgets(str, sizeof str, fp) != NULL) {
    if ((p1 = strstr(str, "$Id:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Id: %*s %d %d-%d-%d", &build, &year, &month, &day) < 4
          && sscanf(p1, "$Id: %*s %d %d/%d/%d", &build, &year, &month, &day) < 4)
        if (sscanf(p1, "$Id: %*s %d.%d %d-%d-%d", &maj_build, &build, &year, &month, &day) < 5)
          sscanf(p1, "$Id: %*s %d.%d %d/%d/%d", &maj_build, &build, &year, &month, &day);
    } else if ((p1 = strstr(str, "$Rev:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Rev: %d.%d", &maj_build, &build) < 2) {
        sscanf(p1, "$Rev: %d", &build);
        maj_build = 0;
      } /* if */
    } else if ((p1 = strstr(str, "$Revision:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Revision: %d.%d", &maj_build, &build) < 2) {
        sscanf(p1, "$Revision: %d", &build);
        maj_build = 0;
      } /* if */
    } else if ((p1 = strstr(str, "$Date:")) != NULL && strchr(p1+1, '$') != NULL) {
      if (sscanf(p1, "$Date: %d-%d-%d", &year, &month, &day) < 3)
        sscanf(p1, "$Date: %d/%d/%d", &year, &month, &day);
    } /* if */

    if (maj_build)
      *accum_build += build;            /* RCS / CVS */
    else if (build > *max_build)
      *max_build = build;               /* Subversion */
    if (year > *max_year
        || (year == *max_year && month > *max_month)
        || (year == *max_year && month == *max_month && day > *max_day))
    {
        *max_year = year;
        *max_month = month;
        *max_day = day;
    } /* if */
    if (build > 0 && year > 0)
      break;      /* both found, no need to search further */

  } /* while */
  fclose(fp);
}

int main(int argc, char *argv[])
{
  char *outname = "svnrev.h";
  FILE *fp;
  int index;
  int process_self = 0;
  int max_build, accum_build;
  int max_year, max_month, max_day;

  if (argc <= 1)
    about();

  /* phase 1: scan through all files and get the highest build number */

  max_build = 0;
  accum_build = 0;      /* for RCS / CVS */
  max_year = max_month = max_day = 0;
  for (index = 1; index < argc; index++) {
    /* check for options */
    if (argv[index][0] == '-'
#if defined __WIN32__ || defined _Win32 || defined _WIN32
     || argv[index][0] == '/'
#endif
    )
    {
      switch (argv[index][1]) {
      case 'o':
        outname = &argv[index][2];
        continue;
      case 'i':
        process_self = 1;
        continue;
      default:
        fprintf(stderr, "Invalid option '%s'\n", argv[index]);
        about();
      } /* switch */
    } /* if */

    processfile(argv[index], 0, &max_build, &accum_build, &max_year, &max_month, &max_day);
  } /* for */

  /* also run over the existing header file, if any */
  if (process_self && *outname != '\0')
    processfile(outname, 1, &max_build, &accum_build, &max_year, &max_month, &max_day);

  if (accum_build > max_build)
    max_build = accum_build;

  /* phase 2: write a file with this highest build number */
  if (*outname == '\0') {
    fp = stdout;
  } else if ((fp = fopen(outname, "w")) == NULL) {
    fprintf(stderr, "Failed to create output file '%s'\n", outname);
    return 2;
  } /* if */
  if (*outname != '\0') {
    /* don't print the comments to stdout */
    fprintf(fp, "/* This file was generated by the \"svnrev\" utility.\n"
                " * You should not modify it manually, as it may be re-generated.\n"
                " *\n"
                " * $Revision: %d$\n"
                " * $Date: %04d-%02d-%02d$\n"
                " */\n\n", max_build, max_year, max_month, max_day);
  } /* if */

  fprintf(fp, "#ifndef SVNREH_H\n");
  fprintf(fp, "#define SVNREV_H\n\n");
  fprintf(fp, "#define SVN_REV\t\t%d\n", max_build);
  fprintf(fp, "#define SVN_REVSTR\t\"%d\"\n", max_build);
  fprintf(fp, "#define SVN_REVDATE\t\"%04d-%02d-%02d\"\n", max_year, max_month, max_day);
  fprintf(fp, "#define SVN_REVSTAMP\t%04d%02d%02dL\n", max_year, max_month, max_day);
  fprintf(fp, "\n#endif /* SVNREV_H */\n");
  if (*outname != '\0')
    fclose(fp);

  return 0;
}
