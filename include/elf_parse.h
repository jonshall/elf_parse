/**
 * @file   elf_parse.h
 * @author Jon S Hall
 * @brief  parse 32-bit elf executables
 * @date   April 2022
 * @ref    Learning Linux Binary Analysis by Ryan "Elfmaster" O'Neil
 */

// defines success and failure
#ifndef _ELF_PARSE_H
#define _ELF_PARSE_H

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define GOOD_EXIT 0
#define BAD_EXIT  -1
#define ELF_MAGIC 0x7f
#define ELF_STR   "ELF"

#endif