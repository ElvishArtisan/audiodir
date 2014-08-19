// audiodir.cpp
//
// A CGI script for listing audio metadata.
//
//   (C) Copyright 1996-2005 Fred Gleason <fredg@blackhawke.net>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <glob.h>

#include "conflib.h"
#include "cgilib.h"
#include "audiodir.h"
#include "filelist.h"


int PathCat(char *dest,const char *src,int maxlen)
{
  int l;
  int offset=0;

  l=strlen(dest);
  if(dest[l-1]!='/') {
    offset=1;
  }
  if((l+strlen(src)+offset)>=maxlen) {
    return -1;
  }
  if(offset==1) {
    strcat(dest,"/");
  }
  strcat(dest,src);
  return 0;
}


int main(int argc,char *argv[])
{
  int i,j;
  struct stat stat;
  struct dirent *dirent;
  char filename[PATH_LEN];
  char local_path[PATH_LEN];
  char download_path[PATH_LEN];
  char stream_path[PATH_LEN];
  char pathname[PATH_LEN];
  int l;
  int year;
  int tag_valid;
  char temp[256];
  char key[256];
  char filter[256];
  char format[256];
  char sortby[3];
  glob_t pglob;

  if(getenv("QUERY_STRING")==NULL) {
    Error("Missing CGI argument");
  }
  if(GetPostString(getenv("QUERY_STRING"),"Key",key,255)<0) {
    Error("Missing key value!");
  }
  if(GetPostString(getenv("QUERY_STRING"),"Filter",filter,255)<0) {
    Error("Missing filter value!");
  }
  if(GetPostString(getenv("QUERY_STRING"),"Format",format,255)<0) {
    Error("Missing format value!");
  }
  if(GetPostString(getenv("QUERY_STRING"),"SortBy",sortby,2)<0) {
    strcpy(sortby,"f");
  }
  l=strlen(format);

  /*
   * Get Paths
   */
  if(GetPrivateProfileString(AUDIODIR_CONF_FILE,key,"LocalPath",local_path,"",
			     PATH_LEN)<0) {
    Error("Invalid key value!");
  }
  GetPrivateProfileString(AUDIODIR_CONF_FILE,key,"DownloadPath",
			  download_path,"",PATH_LEN);
  GetPrivateProfileString(AUDIODIR_CONF_FILE,key,"StreamPath",
			  stream_path,"",PATH_LEN);

  /*
   * Find Matching Files
   */
  strcpy(pathname,local_path);
  PathCat(pathname,filter,255);
  glob(pathname,0,NULL,&pglob);

  //
  // Load the Filelist
  //
  FileList filelist(download_path,stream_path);
  for(i=0;i<pglob.gl_pathc;i++) {
    filelist.addFile(pglob.gl_pathv[i]);
  }

  //
  // Sort the Filelist
  //
  switch(sortby[0]) {
      case 't':
	filelist.sortBy(FileList::Title);
	break;

      case 'a':
	filelist.sortBy(FileList::Artist);
	break;

      case 'l':
	filelist.sortBy(FileList::Album);
	break;

      case 'z':
	filelist.sortBy(FileList::Year);
	break;

      case 'u':
	filelist.sortBy(FileList::Comment);
	break;

      case 'g':
	filelist.sortBy(FileList::Genre);
	break;

      case 'n':
	filelist.sortBy(FileList::Track);
	break;

      case 'D':
	filelist.sortBy(FileList::DownloadUrl);
	break;

      case 's':
	filelist.sortBy(FileList::StreamUrl);
	break;

      case 'K':
      case 'L':
	filelist.sortBy(FileList::Filesize);
	break;

      case 'd':
      case 'm':
      case 'y':
      case 'Y':
      case 'H':
      case 'M':
      case 'S':
	filelist.sortBy(FileList::ModifiedTime);
	break;
  }

  /*
   * Make Substitutions and Render Output
   */
  printf("Content-type: text/html\n\n");
  for(i=0;i<filelist.size();i++) {
    for(j=0;j<l;j++) {
      if(format[j]=='@') {
	if(format[++j]!=0) {
	  switch(format[j]) {
	      case 't':    // Title
		if(strlen(filelist.title(i))>0) {
		  printf("%s",filelist.title(i));
		}
		else {
		  printf("[no title tag]");
		}
		break;
		
	      case 'a':    // Artist
		if(strlen(filelist.artist(i))>0) {
		  printf("%s",filelist.artist(i));
		}
		else {
		  printf("[no artist tag]");
		}
		break;
		
	      case 'l':    // Album
		if(strlen(filelist.album(i))>0) {
		  printf("%s",filelist.album(i));
		}
		else {
		  printf("[no album tag]");
		}
		break;
		
	      case 'z':    // Year
		if(filelist.year(i)>0) {
		  printf("%04u",filelist.year(i));
		}
		else {
		  printf("[no year tag]");
		}
		break;
		
	      case 'u':    // Comment
		if(strlen(filelist.comment(i))>0) {
		  printf("%s",filelist.comment(i));
		}
		else {
		  printf("[no comment tag]");
		}
		break;
		
	      case 'g':    // Genre
		if(strlen(filelist.genre(i))>0) {
		  printf("%s",filelist.genre(i));
		}
		else {
		  printf("[no genre tag]");
		}
		break;
		
	      case 'n':    // Track
		if(filelist.track(i)>0) {
		  printf("%u",filelist.track(i));
		}
		else {
		  printf("[no track tag]");
		}
		break;
		
	      case 'D':    // Download URL
		printf("%s",filelist.downloadUrl(i));
		break;
		
	      case 's':    // Stream URL
		printf("%s",filelist.streamUrl(i));
		break;
		
	      case 'f':    // Filename
		printf("%s",filelist.filename(i));
		break;
		
	      case 'K':    // File Size with Unit
		if(filelist.filesize(i)<1000000) {
		  printf("%4.0f kB",(double)filelist.filesize(i)/1000.0);
		}
		else {
		  printf("%4.0f MB",(double)filelist.filesize(i)/1000000.0);
		}
		break;
		
	      case 'L':    // File Size
		printf("%4.0f",(double)filelist.filesize(i)/1000000.0);
		break;
		
	      case 'd':    // Last Modified Day
		printf("%02d",filelist.modifiedDay(i));
		break;
		
	      case 'm':    // Last Modified Month
		printf("%02d",filelist.modifiedMonth(i)+1);
		break;
		
	      case 'y':    // Last Modified Year (Two Digit)
		year=filelist.modifiedYear(i);
		year-=(year/100*100);
		printf("%02d",year);
		break;
		
	      case 'Y':    // Last Modified Year (Four Digit)
		printf("%04d",filelist.modifiedYear(i));
		break;
		
	      case 'H':    // Last Modified Hour
		printf("%02d",filelist.modifiedHour(i));
		break;
		
	      case 'M':    // Last Modified Minute
		printf("%02d",filelist.modifiedMinute(i));
		break;
		
	      case 'S':    // Last Modified Second
		printf("%02d",filelist.modifiedSecond(i));
		break;
		
	      case '@':    // Literal @
		putchar('@');
		break;
	  }
	}
      }
      else {
	putchar(format[j]);
      }
    }
    printf("\n");
  }
  globfree(&pglob);
  return 0;
}
