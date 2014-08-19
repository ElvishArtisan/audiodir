// filelist.h
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

#ifndef FILELIST_H
#define FILELIST_H

#include <time.h>

#include <vector>
#include <taglib/fileref.h>
#include <taglib/tag.h>

class FileList
{
 public:
  enum Attribute {Title=0,Artist=1,Album=2,Year=3,Comment=4,Genre=5,
		  Track=6,DownloadUrl=7,StreamUrl=8,Filename=9,
		  Filesize=10,ModifiedTime=11};
  FileList(char *download_url,char *stream_url);
  int size() const;
  const char *title(int n) const;
  const char *artist(int n) const;
  const char *album(int n) const;
  unsigned year(int n) const;
  const char *comment(int n) const;
  const char *genre(int n) const;
  unsigned track(int n) const;
  const char *downloadUrl(int n) const;
  const char *streamUrl(int n) const;
  const char *filename(int n) const;
  unsigned filesize(int n) const;
  int modifiedDay(int n) const;
  int modifiedMonth(int n) const;
  int modifiedYear(int n) const;
  int modifiedHour(int n) const;
  int modifiedMinute(int n) const;
  int modifiedSecond(int n) const;
  void sortBy(Attribute attb);
  void addFile(char *path);

 private:
  void BubbleSort(std::vector<unsigned> *v);
  void BubbleSort(std::vector<time_t> *v);
  void BubbleSort(std::vector<TagLib::String> *v);
  std::vector<TagLib::String> list_title;
  std::vector<TagLib::String> list_artist;
  std::vector<TagLib::String> list_album;
  std::vector<TagLib::String> list_comment;
  std::vector<TagLib::String> list_genre;
  std::vector<TagLib::String> list_download;
  std::vector<TagLib::String> list_stream;
  std::vector<TagLib::String> list_filename;
  std::vector<unsigned> list_year;
  std::vector<unsigned> list_track;
  std::vector<unsigned> list_filesize;
  std::vector<time_t> list_time;
  std::vector<unsigned> list_sort;
  char *list_download_url;
  char *list_stream_url;
};


#endif  // FILELIST_H
