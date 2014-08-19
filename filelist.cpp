// filelist.cpp
//
// A file list class.
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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "conflib.h"
#include "filelist.h"

FileList::FileList(char *download_url,char *stream_url)
{
  list_download_url=download_url;
  list_stream_url=stream_url;
}


int FileList::size() const
{
  return list_filename.size();
}


const char *FileList::title(int n) const
{
  return list_title[list_sort[n]].toCString();
}


const char *FileList::artist(int n) const
{
  return list_artist[list_sort[n]].toCString();
}


const char *FileList::album(int n) const
{
  return list_album[list_sort[n]].toCString();
}


unsigned FileList::year(int n) const
{
  return list_year[list_sort[n]];
}


const char *FileList::comment(int n) const
{
  return list_comment[list_sort[n]].toCString();
}


const char *FileList::genre(int n) const
{
  return list_genre[list_sort[n]].toCString();
}


unsigned FileList::track(int n) const
{
  return list_track[list_sort[n]];
}


const char *FileList::downloadUrl(int n) const
{
  return list_download[list_sort[n]].toCString();
}


const char *FileList::streamUrl(int n) const
{
  return list_stream[list_sort[n]].toCString();
}


const char *FileList::filename(int n) const
{
  return list_filename[list_sort[n]].toCString();
}


unsigned FileList::filesize(int n) const
{
  return list_filesize[list_sort[n]];
}


int FileList::modifiedDay(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_mday;
}


int FileList::modifiedMonth(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_mon;
}


int FileList::modifiedYear(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_year+1900;
}


int FileList::modifiedHour(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_hour;
}


int FileList::modifiedMinute(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_min;
}


int FileList::modifiedSecond(int n) const
{
  return localtime(&list_time[list_sort[n]])->tm_sec;
}


void FileList::sortBy(Attribute attb)
{
  switch(attb) {
  case FileList::Title:
    BubbleSort(&list_title);
    break;

  case FileList::Artist:
    BubbleSort(&list_artist);
    break;

  case FileList::Album:
    BubbleSort(&list_album);
    break;

  case FileList::Comment:
    BubbleSort(&list_comment);
    break;

  case FileList::Filename:
    break;

  case FileList::Genre:
    BubbleSort(&list_genre);
    break;

  case FileList::DownloadUrl:
    BubbleSort(&list_download);
    break;

  case FileList::StreamUrl:
    BubbleSort(&list_stream);
    break;

  case FileList::Filesize:
    BubbleSort(&list_filesize);
    break;

  case FileList::Year:
    BubbleSort(&list_year);
    break;

  case FileList::Track:
    BubbleSort(&list_track);
    break;

  case FileList::ModifiedTime:
    BubbleSort(&list_time);
    break;
  }
}


void FileList::addFile(char *path)
{
  char filename[PATH_LEN];
  char urlname[PATH_LEN];
  int j;
  bool tag_valid;
  struct stat stat;

  memset(&stat,0,sizeof(struct stat));
  if((::stat(path,&stat)!=0)||(S_ISREG(stat.st_mode)!=1)) {
    return;
  }
  for(j=strlen(path)-1;j>=0;j--) {
    if(path[j]=='/') {
      strcpy(filename,path+j+1);
      j=-1;
    }
  }
  if(j==0) {
    strcpy(filename,path);
  }

  TagLib::FileRef *fileref=new TagLib::FileRef(path,false);
  if(!fileref->isNull()) {
    TagLib::Tag *filetag=fileref->tag();
    if(!filetag->isEmpty()) {
      list_title.push_back(filetag->title());
      list_artist.push_back(filetag->artist());
      list_album.push_back(filetag->album());
      list_genre.push_back(filetag->genre());
      list_comment.push_back(filetag->comment());
      list_year.push_back(filetag->year());
      list_track.push_back(filetag->track());
      tag_valid=true;
    }
    else {
      tag_valid=false;
    }
  }
  else {
    tag_valid=false;
  }
  if(!tag_valid) {
    list_title.push_back("");
    list_artist.push_back("");
    list_album.push_back("");
    list_genre.push_back("");
    list_comment.push_back("");
    list_year.push_back(0);
    list_track.push_back(0);
  }
  list_filename.push_back(filename);
  list_filesize.push_back(stat.st_size);
  list_time.push_back(stat.st_mtime);
  list_sort.push_back(list_sort.size());
  if((strlen(list_download_url)+strlen(filename)+2)<=PATH_LEN) {
    strcpy(urlname,list_download_url);
    if(urlname[strlen(urlname)-1]!='/') {
      strcat(urlname,"/");
    }
    strcat(urlname,filename);
    list_download.push_back(urlname);
  }
  if((strlen(list_stream_url)+strlen(filename)+2)<=PATH_LEN) {
    strcpy(urlname,list_stream_url);
    if(urlname[strlen(urlname)-1]!='/') {
      strcat(urlname,"/");
    }
    strcat(urlname,filename);
    list_stream.push_back(urlname);
  }
}


void FileList::BubbleSort(std::vector<unsigned> *v)
{
  bool changed=true;
  unsigned n;

  while(changed) {
    changed=false;
    for(unsigned i=1;i<list_sort.size();i++) {
      if(v->at(list_sort[i])<v->at(list_sort[i-1])) {
	n=list_sort[i];
	list_sort[i]=list_sort[i-1];
	list_sort[i-1]=n;
	changed=true;
      }
    }
  }
}


void FileList::BubbleSort(std::vector<time_t> *v)
{
  bool changed=true;
  unsigned n;

  while(changed) {
    changed=false;
    for(unsigned i=1;i<list_sort.size();i++) {
      if(v->at(list_sort[i])<v->at(list_sort[i-1])) {
	n=list_sort[i];
	list_sort[i]=list_sort[i-1];
	list_sort[i-1]=n;
	changed=true;
      }
    }
  }
}


void FileList::BubbleSort(std::vector<TagLib::String> *v)
{
  bool changed=true;
  unsigned n;

  while(changed) {
    changed=false;
    for(unsigned i=1;i<list_sort.size();i++) {
      if(v->at(list_sort[i])<v->at(list_sort[i-1])) {
	n=list_sort[i];
	list_sort[i]=list_sort[i-1];
	list_sort[i-1]=n;
	changed=true;
      }
    }
  }
}
