/*************************************************************************\
 *                  Copyright (C) Michael Kerrisk, 2014.                   *
 *                                                                         *
 * This program is free software. You may use, modify, and redistribute it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 or (at your option) any      *
 * later version. This program is distributed without any warranty.  See   *
 * the file COPYING.gpl-v3 for details.                                    *
* \*************************************************************************/
/*
*
* Name:Sayd Mateen
* Modified this program to work with the stats command.
*\


#define _BSD_SOURCE     /* Get major() and minor() from <sys/types.h> */
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "file_perms.h"
#include "tlpi_hdr.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>

static void
displayStatInfo(const struct stat *sb)
{   
    char tmArr[200];
    time_t t;
    struct tm *tmp;  
    struct passwd *pd = getpwuid(sb->st_uid);
    struct group *gd = getgrgid(sb->st_gid);
    printf("  Size: %-16lld", (long long) sb->st_size);
    printf("Blocks: %-11lld", (long long) sb->st_blocks); 
    printf("IO Block: %-7ld", (long) sb->st_blksize);
    switch (sb->st_mode & S_IFMT) {
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFLNK:  printf("symbolic (soft) link\n");    break;
    case S_IFIFO:  printf("FIFO or pipe\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown file type?\n");      break;
    }
    char combine[50];
    sprintf(combine, "%lxh/%ldd",(long)sb->st_dev, (long)sb->st_dev);
    printf("Device: %-16s", combine);
    printf("Inode: %-12ld", (long) sb->st_ino);
    printf("Links: %ld\n", (long) sb->st_nlink);
    printf("Access: (0%lo/-%s)  ",
            (unsigned long) sb->st_mode & 000777, filePermStr(sb->st_mode, 0));
    printf("Uid: (%5u/%8s)   ", pd->pw_uid, pd->pw_name);
    printf("Gid: (%5u/%8s)\n", gd->gr_gid, gd->gr_name);
    tmp = localtime(&sb-> st_atime);
    strftime( tmArr, sizeof(tmArr), "%F %T %z"  , tmp);
    printf("Access: %s\n", tmArr);
    tmp = localtime(&sb-> st_mtime);
    strftime( tmArr, sizeof(tmArr), "%F %T %z"  , tmp);
    printf("Modify: %s\n", tmArr);
    tmp = localtime(&sb-> st_ctime);
    strftime( tmArr, sizeof(tmArr), "%F %T %z"  , tmp);
    printf("Change: %s\n",tmArr);
}

int
main(int argc, char *argv[])
{
    struct stat sb;
    Boolean statLink;           /* True if "-l" specified (i.e., use lstat) */
    int fname;                  /* Location of filename argument in argv[] */

    statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
                                /* Simple parsing for "-l" */
    fname = statLink ? 2 : 1;

    if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [-l] file\n"
                "        -l = use lstat() instead of stat()\n", argv[0]);

    if (statLink) {
        if (lstat(argv[fname], &sb) == -1)
            errExit("lstat");
    } else {
        if (stat(argv[fname], &sb) == -1)
            errExit("stat");
    }
    printf("  File: `%s'\n", argv[fname]);
    displayStatInfo(&sb);

    exit(EXIT_SUCCESS);
}
