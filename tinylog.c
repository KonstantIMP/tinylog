// tinylog
// Copyright © 2023  KonstantIMP
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <tinylog.h>

#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#ifndef  __TINYLOG_TIME_FORMAT__
  #define __TINYLOG_TIME_FORMAT__ "%d.%m.%Y %H:%M:%S"
#endif //__TINYLOG_TIME_FORMAT__


#ifndef  __TINYLOG_TAG_LENGTH__
  #define __TINYLOG_TAG_LENGTH__ 16
#endif //__TINYLOG_TAG_LENGTH__


static FILE * __TINYLOG_STDLOG;
static bool __TINYLOG_CLOSE_ON_DESTORY;


void __tiny_log_print_timestamp(void) {
  time_t current_time; struct tm * timestamp;
    
  char buffer[128]; memset(buffer, 0, 128);

  current_time = time(NULL);
  timestamp = localtime(&current_time);

  strftime(buffer, 128, __TINYLOG_TIME_FORMAT__, timestamp);
  fprintf(__TINYLOG_STDLOG, "\033[1m[%s]\033[0m ", buffer);
}


int __tiny_log_calculate_str_hashsum(
    const char * str,
    const size_t len
) {
  int result = 0;

  for (size_t i = 0; i < len; i++) {
    result += (int)str[i];
  }

  return 31 + (result % 6); 
}


void __tiny_log_print_tag(
    const char * tag
) {
  size_t tag_len = strlen(tag); char buffer[__TINYLOG_TAG_LENGTH__ + 1];

  memset(buffer, ' ', __TINYLOG_TAG_LENGTH__);
  buffer[__TINYLOG_TAG_LENGTH__] = '\0';

  if (tag_len > __TINYLOG_TAG_LENGTH__) {
    memcpy(buffer, tag, __TINYLOG_TAG_LENGTH__ - 3);
    buffer[__TINYLOG_TAG_LENGTH__ - 1] = 
        buffer[__TINYLOG_TAG_LENGTH__ - 2] =
        buffer[__TINYLOG_TAG_LENGTH__ - 3] = '.';
  }
  else memcpy(buffer, tag, tag_len);

  fprintf(
      __TINYLOG_STDLOG, 
      "\033[%dm%s\033[0m ",
      __tiny_log_calculate_str_hashsum(tag, tag_len),
      buffer
  );
}


void __tiny_log_print_level(
    const tiny_log_level_t level
) {
  int fg, bg; char l;
  
  if (level ==   levelTrace) { l = 'T'; fg = 37; bg = 44; }
  if (level ==    levelInfo) { l = 'I'; fg = 37; bg = 45; }
  if (level == levelWarning) { l = 'W'; fg = 37; bg = 43; }
  if (level ==   levelDebug) { l = 'D'; fg = 37; bg = 42; }
  if (level ==   levelError) { l = 'E'; fg = 37; bg = 41; }

  fprintf(__TINYLOG_STDLOG, "\033[%d;%dm %c \033[0m ", fg, bg, l);
}


bool __tiny_log_is_white(
    const char ch
) {
  return ch ==' ' || ch == '\n' || ch == '\r' ||
            ch == '\t' || ch == '\v' || ch == '\0';
}


size_t __tiny_log_trim(
    char * line,
    size_t length
) {
  for (size_t i = length - 1; i >= 0; i--) {
    if (__tiny_log_is_white(line[i])) line[i] = '\0';
    else { length = i + 1; break; }
  }

  size_t white_counter = 0; bool white_ended = false;
  for (size_t i = 0; i < length; i++) {
    if (!white_ended) {
      if (__tiny_log_is_white(line[i])) white_counter++;
      else {
        if (white_counter == 0) return length;
        white_ended = true; i--;
      }
    }
    else {
      if (white_counter == 0) return length;
      line[i - white_counter] = line[i];
      line[i] = '\0';
    }
  }

  return length - white_counter;
}


void tiny_log_init(void) {
  __TINYLOG_CLOSE_ON_DESTORY = false;
  __TINYLOG_STDLOG = stdout;
}


void tiny_log_set_out(
    FILE * out_fd,
    const bool close_on_destroy
) {
  __TINYLOG_CLOSE_ON_DESTORY = close_on_destroy;
  __TINYLOG_STDLOG = out_fd;
}


FILE * tiny_log_get_out(void) {
  return __TINYLOG_STDLOG;
}
    

void tiny_log(
    const char * tag,
    const tiny_log_level_t level,
    const char * format, ...
) {
  __tiny_log_print_timestamp();
  __tiny_log_print_tag(tag);
  __tiny_log_print_level(level);

  size_t format_len = strlen(format);
  
  char * trimmed = (char *)malloc(format_len + 1);
  memcpy(trimmed, format, format_len);
  trimmed[format_len] = '\0';

  __tiny_log_trim(trimmed, format_len);

  va_list args; va_start(args, format);
  vfprintf(__TINYLOG_STDLOG, trimmed, args);
  va_end(args); free(trimmed);
  fprintf(__TINYLOG_STDLOG, "\n");
}


void tiny_log_destroy(void) {
  if (__TINYLOG_CLOSE_ON_DESTORY) fclose(__TINYLOG_STDLOG);
  __TINYLOG_STDLOG = NULL;
}


