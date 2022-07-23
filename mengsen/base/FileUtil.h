/*
 * @Date: 2020-08-14 14:48:26
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:24:06
 * @FilePath: /mengsen/mengsen/base/FileUtil.h
 */

#ifndef __MENGSEN_FILEUTIL_H__
#define __MENGSEN_FILEUTIL_H__

#include <sys/types.h>  // for off_t

#include "mengsen/base/NonCopyable.h"
#include "mengsen/base/StringPiece.h"

namespace mengsen {

namespace FileUtil {

// read small file < 64KB
class ReadSmallFile : NonCopyable {
 public:
  ReadSmallFile(StringArg filename);
  ~ReadSmallFile();

  // return errno
  template <typename String>
  int readToString(int maxSize, String* content, int64_t* fileSize,
                   int64_t* modifyTime, int64_t* createTime);

  /// Read at maxium kBufferSize into buf_
  // return errno
  int readToBuffer(int* size);

  const char* buffer() const { return buf_; }

  static const int kBufferSize = 64 * 1024;

 private:
  int fd_;
  int err_;
  char buf_[kBufferSize];
};

// read the file content, returns errno if error happens.
template <typename String>
int readFile(StringArg filename, int maxSize, String* content,
             int64_t* fileSize = NULL, int64_t* modifyTime = NULL,
             int64_t* createTime = NULL) {
  ReadSmallFile file(filename);
  return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

// not thread safe
class AppendFile : NonCopyable {
 public:
  explicit AppendFile(StringArg filename);

  ~AppendFile();

  void append(const char* logline, size_t len);

  void flush();

  off_t writtenBytes() const { return writtenBytes_; }

 private:
  size_t write(const char* logline, size_t len);

  FILE* fp_;
  char buffer_[64 * 1024];
  off_t writtenBytes_;
};

}  // namespace FileUtil

}  // namespace mengsen

#endif  // __MENGSEN_FILEUTIL_H__