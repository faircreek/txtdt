/*
 * Copyright (C) 2002-2015 Olof <jinxolina@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <limits.h>
#include "bswap.h"

/**
 * Stuctures
 */

/**
 * IT_TXT structure
 */
typedef struct {

  /* This is normaly just one big array
  of chars (and uint8_t) separated by
  '0x09' this is implementation and access
  unfriendly so we remade it */

   /* Size variable in the real one */

  /* The "string" with '0x09' exchanged to
   '\0' */
  uint8_t *it_txt;

  /* The size of the "string" including '\0' */
  int string_size;

  /* The string number used for verification in
  the implementation */
  int string_number;

  /* The offset of the string relative to TXTDT
   used during implmentation */
  int string_offset;

} it_txt_t;
/**
 * END - IT_TXT structure
 */

/**
 * IT_TXT_SRP structure
 */
typedef struct {

  /* Size 4 bytes */
  /* 0000 - IT_TXT_IDCD (i.e. type of entry) this
  can either be a Application_IDCD or a Application_IDCD
  see parser for info */
  uint8_t it_txt_idcd;

  /*0001 - Reserved/Unknown */
  /*0002 - Start byte of string beloning to the entry
  relative to TXTDT (IT_TXT_SA)
  DONT USE THIS ACCESS POINTER IT'S JUST INFO*/
  uint16_t it_txt_sa;

  /* This structure is also arcane so we will
  make a better interface :). The case is that
  strings can be refered to multiple times hence
  one entry != one string */

  /* Entry number to the IT_TXT array
  0 == false (not an string in the array)
  NOTE: Since we use 0 as false use
  it_txt_enty_nr -1 to access the right
  IT_TXT in the array*/
  int it_txt_entry_nr;

} it_txt_srp_t;
/**
 * END - IT_TXT_SRP structure
 */

/**
 * TXTDT structure
 */
typedef struct {

  /* Size of header is 4 bytes */

  /* 0000 Number of IT_TXT_SRP entries (TXTDTI)
  Max ??? is 65535 entries - Remeber we can
  have entries for e.g. specific cell so
  it's pretty low level the main usage
  for the user is probably just the
  entries from the it_txt_srp_sa_t array*/
  uint16_t nr_of_it_txt_srps;

  /* 0002 - Reserved/Unknown */

  /* Access pointers */
  it_txt_srp_t *it_txt_srp;
  it_txt_t  *it_txt;
  
  int  nr_of_it_txt;

} txtdt_t;
/**
 * END - TXTDT structure
 */

/**
 * IT_TXT_SRP_SA access structure not a part of the table!!
 * NOT USED!!
 */
typedef struct {

  /* Entry number to the IT_TXT_SRP array
  0 == false (not an entry in the array)
  NOTE: Since we use 0 as false use
  it_txt_srp_enty_nr -1 to access the right
  IT_TXT_SRP in the array*/

  int it_txt_srp_entry_nr;

  /* NOTE: There is supposed to be nr_of_srpts
  (number of titles) + 1 (for the volume) entries
  in this array but that is not the case in real
  live DVD-Video authors usually skip to name e.g
  the FBI Warning title :) */

}it_txt_srp_sa_t;
/**
 * END - IT_TXT_SRP_SA access structure not a part of the table!!
 */

/**
 * TXTDT_LU
 */
typedef struct {

  /* Size 204 bytes*/

  /* 0000 - Endbyte of TXTDT_LU relative to TXTDT_LU i.e. TXTDT_LUI
  This one must be uint32_t we can have 64k of
  text data i.e.00 01 00 00h plus all IT_TXT_SRP
  entries which is more or less an undefined amount*/
  uint32_t end_byte;

  /* 0004 - Start byte of Volume IT_TXT_SRP entry relative to TXTDT_LU
  this one is more or less  static since the first IT_TXT_SRP has to be
  the volume descriptor (IT_TXT_SRT_SA_VLM)*/
  // see it_txt_srp_sa_t -  uint16_t start_byte_vlm;

  /* 0006 - Start byte or each title IT_TXT_SRP max 99 since we can
  only have 99 titles on a DVD-Video (IT_TXT_SRP_SA_TT)*/
  // see it_txt_srp_sa_t - uint16_t start_byte_tt[99];

  /* Access pointers */
  txtdt_t *txtdt;

  /* Since the txtdt.. structure sucks big time we will
  implement it in a less arcane way  hence we have this
  access struture instead of raw byte offsers*/
  //it_txt_srp_sa_t it_txt_srp_sa[100];
  uint16_t  it_txt_srp_sa[100];

  /* Each entry in the array is a number to the
  IT_TXT_SRP_SA array
  0 == false (not an entry in the array)
  NOTE: Since we use 0 as false use
  "it_txt_srp_enty_nr" -1 to access the right
  IT_TXT_SRP in the array*/

  /* NOTE: There is supposed to be nr_of_srpts
  (number of titles) + 1 (for the volume) entries
  in this array but that is not the case in real
  live DVD-Video authors usually skip to name e.g
  the FBI Warning title :) */


} txtdt_lu_t;
/**
 * END - TXTDT_LU
 */

/**
 * TXTDT_LU_SRP
 */
typedef struct {

  /* Size each row in table 8 bytes */

  /* 0000 - Text language code */
  //uint16_t txtdt_lcd;
  char txtdt_lcd[3];

  /* 0002 - Reserved/Unknown */
  /* 0003 - Character set of IT_TXT
  00h == Unicode ??
  01h == ISO 646 ASCII
  10h == JIS Roman & JIS Kanji 1990
  11h == ISO 8859-1
  12h == Shift JIS Kanji, including Romand & JIS Katakan*/
  uint8_t char_set;

  /* 0004 - Start address of TXTDT_LU relative to TXTDT_MG */
  uint32_t start_byte;

  /* Access pointers to each TXT_LU
  in form of an array*/
  txtdt_lu_t  *txtdt_lu;

} txtdt_lu_srp_t;
/**
 * END - TXTDT_LU_SRP
 */

/**
 * TXTDT_MG header i.e. TXTDT_MG_SA
 */
typedef struct {

  /* Size header 20 bytes */

  /* 0000 - Company or Disk identifier string
  NOTE: Size in real life 12 bytes!*/
  char txtdt_id[13];

  /* 000c - Reserved/Unknown */

  /* 000e - Number of TXTDT_LU (Language) units */
  uint16_t nr_of_txtdt_lus;

  /* 0010 - Endbyte of TXTDT_MG relative to TXTDT_MG*/
  uint32_t end_byte;

  /* Access pointer to each
  TXT_LU_SRP in form of an array*/
  txtdt_lu_srp_t  *txtdt_lu_srp;

} txtdt_mg_t;
/**
 * END - TXTDT_MG header i.e. TXTDT_MG_SA
 */

/**
 * END - Stuctures
 */

/**
 * Prototypes
 */

/**
 * Returns a txt_mg_t on success and NULL on error
 */
txtdt_mg_t * Read_TXTDT_MG(int file_handle);

/**
 * Free all allocated memory of the TXT_MG stucture
 */
void Free_TXTDT_MG(txtdt_mg_t * txtdt_mg);


