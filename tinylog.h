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
#ifndef  __TINYLOG_H__
#define  __TINYLOG_H__


#include <stdbool.h>
#include <stdio.h>


/**
 * @brief List of available logging levels
 */
typedef enum {
  levelTrace   = 0,
  levelInfo    = 1,
  levelDebug   = 2,
  levelWarning = 3,
  levelError   = 5
} tiny_log_level_t;


/**
 * @brief inits some objects for correct working
 */
void tiny_log_init(void);


/**
 * @brief Sets file for messages writing
 * By default setted as __TINYDNS_STDLOG
 *
 * @param out_fd FILE* File for writing 
 * @param close_on_destroy bool Should the descriptor
 * be closed on destroy state
 */
void tiny_log_set_out(
  FILE * out_fd,
  const bool close_on_destroy
);


/**
 * @brief Returns pointer to the file for logs writing
 */
FILE * tiny_log_get_out(void);


/**
 * @brief send message (makes log record)
 *
 * @param tag char* Tag for the current message
 * @param level tiny_log_level_t level of the message 
 * @param format char* Format of the message for display
 */
void tiny_log(
  const char * tag,
  const tiny_log_level_t level,
  const char * format,
  ...
);


/**
 * @brief frees memory which was allocated on init 
 */
void tiny_log_destroy(void);


#endif //__TINYLOG_H__

