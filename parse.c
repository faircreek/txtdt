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

#include "txtdt.h"
#include "parse.h"
#include "idcd.h"

/* Errors */
int errors = 0;


static int internal_parse(txtdt_t *txtdt, int entry, int nr_of_tabs);

static void do_tabs(int nr_of_tabs) {
	int i;
	for(i=0; i < nr_of_tabs; i++) {
		printf("\t");
	}
}



int  Parse_TXTDT_MG(txtdt_mg_t * txtdt_mg) {

	/* Interations */
	int i, j;


	/* Structures */
	txtdt_t *txtdt;

	/* IT_TXT_IDCD */
	idcd_type_t  type;

	/* Temporary varibales */
	int nr_of_tabs;
	int char_code;
	int entry;

	printf("\nCompany / Disk ID: %s\n",txtdt_mg->txtdt_id);
	printf("Number of TXTDT_LU: %d\n",txtdt_mg->nr_of_txtdt_lus);
	printf("Endbyte of TXTDT_MG: %08x\n\n",txtdt_mg->txtdt_lu_srp);



	for(i=0; i < txtdt_mg->nr_of_txtdt_lus; i++) {

		printf("Language Code for TXTDT_LU number %d is: %s\n", i + 1, txtdt_mg->txtdt_lu_srp[i].txtdt_lcd);
		char_code = txtdt_mg->txtdt_lu_srp[i].char_set;
		switch(char_code) {
			case 0:
				printf("The Character set of the txtit is: Unicode\n");
				break;
			case 1:
				printf("The Character set of the txtit is: ISO 646 - ASCII\n");
				break;
			case 0x10:
				printf("The Character set of the txtit is: JIS Roman & JIS Kanji 1990\n");
				break;
			case 0x11:
				printf("The Character set of the txtit is: ISO 8859-1 - ISO Latin-1\n");
				break;
			case 0x12:
				printf("The Character set of the txtit is: Shift JIS Kanji, including Romand & JIS Katakan\n");
				break;
			default:
				printf("Please send a bug report this is a unknown character set code: %02x\n", char_code);
		}

		printf("Start byte of TXTDT_LU is: %08x\n\n",txtdt_mg->txtdt_lu_srp[i].start_byte);

		printf("TXTDT_LU\n");
		printf("Endbyte of TXTDT_LU is: %08x\n\n",txtdt_mg->txtdt_lu_srp[i].txtdt_lu->end_byte);

		printf("TXT Information from IT_TXT_SRP_SA table\n");

		nr_of_tabs = 0;

		for(j = 0 ; j < 100; j++) {
			type = IDCD_Reserved;
			entry = txtdt_mg->txtdt_lu_srp[i].txtdt_lu->it_txt_srp_sa[j];
#ifdef DEBUG
			printf("Entry number is %d\n", entry);
#endif
			if( entry <= txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt->nr_of_it_txt_srps && entry != 0 ) {
				nr_of_tabs = internal_parse(txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt, entry - 1, nr_of_tabs);
				type = txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt->it_txt_srp[entry -1].it_txt_idcd;
			}
			if( entry < txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt->nr_of_it_txt_srps && entry != 0 && (type ==IDCD_Volume || type == IDCD_Title)) {
				nr_of_tabs = internal_parse(txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt, entry, nr_of_tabs);
			}

		}

		printf("\nTXT Information from IT_TXT_SRP table\n");
		nr_of_tabs = 0;
		for(j=0; j < txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt->nr_of_it_txt_srps ; j++) {
			nr_of_tabs = internal_parse(txtdt_mg->txtdt_lu_srp[i].txtdt_lu->txtdt, j, nr_of_tabs);
		}

		printf("\n");
	}

	return(errors);

}

static int internal_parse(txtdt_t *txtdt, int entry, int nr_of_tabs) {

	/* Interation */
	int j;

	/* IT_TXT_IDCD */
	idcd_type_t  type;


#ifdef DEBUG
	for(j=0; j < txtdt->nr_of_it_txt_srps ; j++) {

		printf("Type is %02x\n",txtdt->it_txt_srp[j].it_txt_idcd);
		printf("String Number %d\n", txtdt->it_txt_srp[j].it_txt_entry_nr);
		if(txtdt->it_txt_srp[j].it_txt_entry_nr != 0) {
			printf("String is %s\n", txtdt->it_txt[txtdt->it_txt_srp[j].it_txt_entry_nr -1].it_txt);
		}
	}
#endif
/*	nr_of_tabs = 0;
	for(j=0; j < txtdt->nr_of_it_txt_srps ; j++) {
*/

	type = txtdt->it_txt_srp[entry].it_txt_idcd;

	switch (type) {
		case IDCD_Reserved:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Volume:
			nr_of_tabs = 0;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 1;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Volume:	%s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Volume:	\n");
			}
			break;

		case IDCD_Title:
			nr_of_tabs = 1;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 2;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title: \n");
			}
			break;

		case IDCD_Parental_ID:
			nr_of_tabs = 1;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 2;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Parental ID: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Parental ID:\n");
			}
			break;

		case IDCD_Part_Of_Title:
			nr_of_tabs = 2;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 3;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter: \n");
			}
			break;

		case IDCD_Cell:
			nr_of_tabs = 3;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 4;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Cell: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Cell: \n");
			}
			break;

		case IDCD_Reserved_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");

			break;

		case IDCD_Reserved_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_5:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_5: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_6:
			errors++;
			do_tabs(nr_of_tabs);

			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_6: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Provider_Uniq_1:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_1: \n");
			}
			break;

		case IDCD_Provider_Uniq_2:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_2: \n");
			}
			break;

		case IDCD_Provider_Uniq_3:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_3: \n");
			}
			break;

		case IDCD_Provider_Uniq_4:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_4: \n");
			}
			break;

		case IDCD_Audio_Stream:
			nr_of_tabs = 4;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 5;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Audio_Stream: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Audio_Stream: \n");
			}
			break;

		case IDCD_Subpicture_Stream:
			nr_of_tabs = 4;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 5;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Subpicture_Stream: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Subpicture_Stream: \n");
			}
			break;

		case IDCD_Angle:
			nr_of_tabs = 4;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 5;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Angle: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Angle: \n");
			}
			break;

		case IDCD_Reserved_7:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_7: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_8:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_8: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_8: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_9:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_9: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_9: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_A:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_A: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_B:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_B: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_C:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_C: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_C: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_D:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_D: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_D: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_E:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_E: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_E: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_F:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_F: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_F: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Provider_Uniq_5:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_5: \n");
			}
			break;

		case IDCD_Provider_Uniq_6:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_6: \n");
			}
			break;

		case IDCD_Provider_Uniq_7:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_7: \n");
			}
			break;

		case IDCD_Provider_Uniq_8:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_8: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_8: \n");
			}
			break;

		case IDCD_Audio_Channel:
			nr_of_tabs = 5;
			do_tabs(nr_of_tabs);
			nr_of_tabs = 6;
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Audio_Channel: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Audio_Channel: \n");
			}
			break;

		case IDCD_Reserved_11:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_11: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_11: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_12:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_12: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_12: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_13:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_13: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_13: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_14:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_14: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_14: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_15:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_15: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_15: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_16:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_16: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_16: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_17:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_17: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_17: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_18:
			errors++;
			do_tabs(nr_of_tabs);

			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_18: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_18: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_19:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_19: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_19: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_1A:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_1A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_1A: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Reserved_1B:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Reserved_1B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Reserved_1B: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Provider_Uniq_9:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_9: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_9: \n");
			}
			break;

		case IDCD_Provider_Uniq_A:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_A: \n");
			}
			break;

		case IDCD_Provider_Uniq_B:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_B: \n");
			}
			break;

		case IDCD_Provider_Uniq_C:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Provider_Uniq_C: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Provider_Uniq_C: \n");
			}
			break;

		case IDCD_General_Name:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Name: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Name: \n");
			}
			break;

		case IDCD_General_Comments:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Comments: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Comments: \n");
			}
			break;

		case IDCD_General_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_General_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_General_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);

			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_General_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_General_Unknown_5:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_5: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");

			break;
		case IDCD_General_Unknown_6:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Unknown_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Unknown_6: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Series:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Series: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Series: \n");
			}
			break;

		case IDCD_Title_Movie:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Movie: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Movie: \n");
			}
			break;

		case IDCD_Title_Video:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Video: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Video: \n");
			}
			break;

		case IDCD_Title_Album:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Album: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Album: \n");
			}
			break;

		case IDCD_Title_Song:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Song: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Song: \n");
			}
			break;

		case IDCD_Title_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Other:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Other: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Other: \n");
			}
			break;

		case IDCD_Title_Sub_Series:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Series: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Series: \n");
			}
			break;

		case IDCD_Title_Sub_Movie:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Movie: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Movie:\n");
			}
			break;

		case IDCD_Title_Sub_Video:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Video: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Video: \n");
			}
			break;

		case IDCD_Title_Sub_Album:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Album: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Album: \n");
			}
			break;

		case IDCD_Title_Sub_Song:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Song: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Song: \n");
			}
			break;

		case IDCD_Title_Sub_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Sub_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Sub_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Sub_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Sub_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Sub_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Sub_Other:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Chapter Other: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Chapter Other: \n");
			}
			break;

		case IDCD_Title_Orig_Series:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Series: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Series: \n");
			}
			break;

		case IDCD_Title_Orig_Movie:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Movie: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Movie: \n");
			}
			break;

		case IDCD_Title_Orig_Video:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Video: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Video: \n");
			}
			break;

		case IDCD_Title_Orig_Album:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Album: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Album: \n");
			}
			break;

		case IDCD_Title_Orig_Song:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Song: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Song: \n");
			}
			break;

		case IDCD_Title_Orig_Unknown_1:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Orig_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Orig_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Orig_Unknown_2:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title_Orig_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title_Orig_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Title_Orig_Other:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Title Original Other: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Title Original Other: \n");
			}
			break;

		case IDCD_Other_Scene:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other Scene: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other Scene: \n");
			}
			break;

		case IDCD_Other_Cut:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other Cut: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other Cut: \n");
			}
			break;

		case IDCD_Other_Take:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other Take: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other Take: \n");
			}
			break;

		case IDCD_Other_Label:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other Label: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other Label: \n");
			}
			break;

		case IDCD_Other_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("IDCD_Other_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("IDCD_Other_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Other_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Other_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Other_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Language_Language:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Language: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Language: \n");
			}
			break;

		case IDCD_Language_Original_Language:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original Language: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original Language: \n");
			}
			break;

		case IDCD_Language_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Language_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Language_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Language_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Language_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Language_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Work_Picture_Size:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Picture_Size: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Picture_Size: \n");
			}
			break;

		case IDCD_Work_Picture_Color:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Picture_Color: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Picture_Color: \n");
			}
			break;

		case IDCD_Work_Picture_Feature:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Picture_Feature: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Picture_Feature: \n");
			}
			break;

		case IDCD_Work_Audio_Feature:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Audio_Feature: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Audio_Feature: \n");
			}
			break;

		case IDCD_Work_Other_Feature:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Other_Feature: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Other_Feature: \n");
			}
			break;

		case IDCD_Work_Shooting_Location:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Shooting_Location: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Shooting_Location: \n");
			}
			break;

		case IDCD_Work_Country_Of_Production:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Country_Of_Production: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Country_Of_Production: \n");
			}
			break;

		case IDCD_Work_Production_Company:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Production_Company: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Production_Company: \n");
			}
			break;

		case IDCD_Work_Production_Studio:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Production_Studio: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Production_Studio: \n");
			}
			break;

		case IDCD_Work_Awarded_Prize:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Awarded_Prize: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Awarded_Prize: \n");
			}
			break;

		case IDCD_Work_Original_Writing:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Original_Writing: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Original_Writing: \n");
			}
			break;

		case IDCD_Work_Suggested_Age:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Suggested_Age: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Suggested_Age: \n");
			}
			break;

		case IDCD_Work_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Work_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Work_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Work_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Work_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Work_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Character_Leading_Actor:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Leading Actor: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Leading Actor: \n");
			}
			break;

		case IDCD_Character_Leading_Actress:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Leading Actress: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Leading Actress: \n");
			}
			break;

		case IDCD_Character_Supporting_Actor:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Supporting Actor: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Supporting Actor: \n");
			}
			break;

		case IDCD_Character_Supporting_Actress:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Supporting Actress: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Supporting Actress: \n");
			}
			break;

		case IDCD_Character_Other_Actor_Or_Actress:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Other_Actor_Or_Actress: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Other_Actor_Or_Actress: \n");
			}
			break;

		case IDCD_Character_Producer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Producer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Producer: \n");
			}
			break;

		case IDCD_Character_Director:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Director: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Director: \n");
			}
			break;

		case IDCD_Character_Scenario_Writer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Scenario_Writer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Scenario_Writer: \n");
			}
			break;

		case IDCD_Character_Original_Writer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original_Writer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original_Writer: \n");
			}
			break;

		case IDCD_Character_Camera_Man:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Camera_Man: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Camera_Man: \n");
			}
			break;

		case IDCD_Character_Special_Effect_Director:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Special_Effect_Director: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Special_Effect_Director: \n");
			}
			break;

		case IDCD_Character_Voice_Actor_or_Actress_for_Leading_Actor:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Voice_Actor_or_Actress_for_Leading_Actor: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Voice_Actor_or_Actress_for_Leading_Actor: \n");
			}
			break;

		case IDCD_Character_Voice_Actor_or_Actress_for_Leading_Actress:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Voice_Actor_or_Actress_for_Leading_Actress: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Voice_Actor_or_Actress_for_Leading_Actress: \n");
			}
			break;

		case IDCD_Character_Voice_Actor_or_Actress_for_Supporting_Actor:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Voice_Actor_or_Actress_for_Supporting_Actor: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Voice_Actor_or_Actress_for_Supporting_Actor: \n");
			}
			break;

		case IDCD_Character_Voice_Actor_or_Actress_for_Supporting_Actress:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Voice_Actor_or_Actress_for_Supporting_Actress: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Voice_Actor_or_Actress_for_Supporting_Actress: \n");
			}
			break;

		case IDCD_Character_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Character_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Character_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Character_Leading_Musician:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Leading_Musician: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Leading_Musician: \n");
			}
			break;

		case IDCD_Character_Musician:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Musician: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Musician: \n");
			}
			break;

		case IDCD_Character_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Vocalist:	%s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Vocalist: \n");
			}
			break;

		case IDCD_Character_Conductor:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Conductor: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Conductor: \n");
			}
			break;

		case IDCD_Character_Orchestra:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Orchestra: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Orchestra: \n");
			}
			break;

		case IDCD_Character_Song_Writer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Song_Writer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Song_Writer: \n");
			}
			break;

		case IDCD_Character_Music_Composer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Music_Composer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Music_Composer: \n");
			}
			break;

		case IDCD_Character_Music_Arranger:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Music_Arranger: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Music_Arranger: \n");
			}
			break;

		case IDCD_Character_Music_Producer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Music_Producer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Music_Producer: \n");
			}
			break;

		case IDCD_Character_Master_Of_Ceremonies:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Master_Of_Ceremonies: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Master_Of_Ceremonies: \n");
			}
			break;

		case IDCD_Character_Vocal_Trainer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Vocal_Trainer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Vocal_Trainer: \n");
			}
			break;

		case IDCD_Character_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Character_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Character_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");

			break;

		case IDCD_Character_Picture_Creator:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Picture_Creator: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Picture_Creator: \n");
			}
			break;

		case IDCD_Character_Animator:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Animator: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Animator: \n");
			}
			break;

		case IDCD_Character_Character_Designer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Character_Designer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Character_Designer: \n");
			}
			break;

		case IDCD_Character_Mechanism_Designer:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Mechanism_Designer: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Mechanism_Designer: \n");
			}
			break;

		case IDCD_Character_Leading_Artist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Leading_Artist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Leading_Artist: \n");
			}
			break;

		case IDCD_Character_Artist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Artist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Artist: \n");
			}
			break;

		case IDCD_Character_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Character_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Character_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Character_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Character_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Character_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Data_Production:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Production: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Production: \n");
			}
			break;

		case IDCD_Data_Award:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Award: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Award: \n");
			}
			break;

		case IDCD_Data_Historical_Background:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Historical_Background: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Historical_Background: \n");
			}
			break;

		case IDCD_Data_Unknown:
			do_tabs(nr_of_tabs);
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Data_Unknown: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Data_Unknown: \n");
			}
			break;

		case IDCD_Karaoke_Male_Melody_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Male_Melody_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Male_Melody_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Male_Harmony_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Male_Harmony_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Male_Harmony_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Other_Male_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Other_Male_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Other_Male_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Female_Melody_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Female_Melody_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Female_Melody_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Female_Harmony_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Female_Harmony_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Female_Harmony_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Other_Female_Original_Vocalist:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Other_Female_Original_Vocalist: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Other_Female_Original_Vocalist: \n");
			}
			break;

		case IDCD_Karaoke_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");

			break;

		case IDCD_Karaoke_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Karaoke_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Karaoke_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Category_General_Category:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("General_Category: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("General_Category: \n");
			}
			break;

		case IDCD_Category_Video_Category:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Video_Category: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Video_Category: \n");
			}
			break;

		case IDCD_Category_Music_Or_Karaoke_Category:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Music_Or_Karaoke_Category: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Music_Or_Karaoke_Category: \n");
			}
			break;

		case IDCD_Category_Sub_Category:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Sub_Category: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Sub_Category: \n");
			}
			break;

		case IDCD_Lyrics_Lyrics:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Lyrics: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Lyrics: \n");
			}
			break;

		case IDCD_Lyrics_Begining_Of_Lyrics:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Begining_Of_Lyrics: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Begining_Of_Lyrics: \n");
			}
			break;

		case IDCD_Lyrics_Famous_Lyrics:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Famous_Lyrics: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Famous_Lyrics: \n");
			}
			break;

		case IDCD_Lyrics_Unknown:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Lyrics_Unknown: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Lyrics_Unknown: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Document_Liner_Notes:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Document_Liner_Notes: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Document_Liner_Notes: \n");
			}
			break;

		case IDCD_Document_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Document_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Document_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Document_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Document_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Document_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Document_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Document_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Document_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Others_Others:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Others_Others: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Others_Others: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Others_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Others_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Others_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Others_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Others_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Others_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Others_Unknown_3:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Others_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Others_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Admin_Product_Code:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Product_Code: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Product_Code: \n");
			}
			break;

		case IDCD_Admin_POS_Code:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("POS_Code: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("POS_Code: \n");
			}
			break;

		case IDCD_Admin_ISRC:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("ISRC: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("ISRC: \n");
			}
			break;
		case IDCD_Admin_Copyright:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Copyrigh: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Copyrigh: \n");
			}
			break;

		case IDCD_Admin_Supplier:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Supplier: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Supplier: \n");
			}
			break;

		case IDCD_Admin_Others:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Admin_Others: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Admin_Others: \n");
			}
			break;

		case IDCD_Admin_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Admin_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Admin_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Admin_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Admin_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Admin_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Release_Date:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Release_Date: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Release_Date: \n");
			}
			break;

		case IDCD_Add_Admin_Rental_Ban_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Rental_Ban_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Rental_Ban_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Resale_Ban_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Resale_Ban_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Resale_Ban_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Printing_Company:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Printing_Company: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Printing_Company: \n");
			}
			break;

		case IDCD_Add_Admin_Original_Product_Number:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original_Product_Number: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original_Product_Number: \n");
			}
			break;

		case IDCD_Add_Admin_Original_Release_Data:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original_Release_Data: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original_Release_Data: \n");
			}
			break;

		case IDCD_Add_Admin_Original_Supplier:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original_Supplier: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original_Supplier: \n");
			}
			break;

		case IDCD_Add_Admin_Original_Copyright:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Original_Copyright: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Original_Copyright: \n");
			}
			break;

		case IDCD_Add_Admin_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Recording_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Recording_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Recording_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Pre_Mastering_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Pre_Mastering_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Pre_Mastering_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Encoding_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Encoding_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Encoding_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Authoring_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Authoring_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Authoring_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Master_Tape_infromation:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Master_Tape_infromation: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Master_Tape_infromation: \n");
			}
			break;

		case IDCD_Add_Admin_Other_Information:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Admin_Other_Information: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Admin_Other_Information: \n");
			}
			break;

		case IDCD_Add_Admin_Unknown_5:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_5: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_6:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_6: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_7:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_7: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_8:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_8: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_8: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_9:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_9: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_9: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Add_Admin_Unknown_A:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Add_Admin_Unknown_A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Add_Admin_Unknown_A: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_5:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_5: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_6:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_6: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_7:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_7: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_8:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_8: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_8: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_9:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_9: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_9: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_A:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_A: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_B:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_B: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_C:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_C: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_C: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_D:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_D: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_D: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_E:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_E: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_E: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_F:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_F: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_F: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_11:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_11: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_11: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_12:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_12: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_12: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_13:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_13: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_13: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_14:
			errors++;
			do_tabs(nr_of_tabs);

			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_14: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_14: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_15:
			errors++;
			do_tabs(nr_of_tabs);

			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_15: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_15: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Vendor_0:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_0: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_0: \n");
			}
			break;

		case IDCD_Application_Vendor_1:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_1: \n");
			}
			break;

		case IDCD_Application_Vendor_2:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_2: \n");
			}
			break;

		case IDCD_Application_Vendor_3:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_3: \n");
			}
			break;

		case IDCD_Application_Vendor_4:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_4: \n");
			}
			break;

		case IDCD_Application_Vendor_5:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_5: \n");
			}
			break;

		case IDCD_Application_Vendor_6:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_6: \n");
			}
			break;

		case IDCD_Application_Vendor_7:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_7: \n");
			}
			break;

		case IDCD_Application_Vendor_8:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_8: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_8: \n");
			}
			break;

		case IDCD_Application_Vendor_9:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_9: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_9: \n");
			}
			break;

		case IDCD_Application_Vendor_A:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_A: \n");
			}
			break;

		case IDCD_Application_Vendor_B:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_B: \n");
			}
			break;

		case IDCD_Application_Vendor_C:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_C: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_C: \n");
			}
			break;

		case IDCD_Application_Vendor_D:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_D: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_D: \n");
			}
			break;

		case IDCD_Application_Vendor_E:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_E: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_E: \n");
			}
			break;

		case IDCD_Application_Vendor_F:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Vendor_F: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Vendor_F: \n");
			}
			break;

		case IDCD_Extension_Sorting:
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Sorting: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Sorting: \n");
			}
			break;

		case IDCD_Extension_Unknown_1:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_1: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_1: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_2:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_2: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_2: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_3:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_3: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_3: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_4:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_4: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_4: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_5:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_5: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_5: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_6:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_6: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_6: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Extension_Unknown_7:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Extension_Unknown_7: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Extension_Unknown_7: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_16:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_16: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_16: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_17:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_17: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_17: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_18:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_18: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_18: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_19:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_19: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_19: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_1A:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_1A: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_1A: \n");
			}
			break;

		case IDCD_Application_Reserved_1B:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_1B: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_1B: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_1C:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_1C: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_1C: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

		case IDCD_Application_Reserved_1D:
			errors++;
			do_tabs(nr_of_tabs);
			if (txtdt->it_txt_srp[entry].it_txt_entry_nr != 0 ) {
				printf("Application_Reserved_1D: %s\n", txtdt->it_txt[txtdt->it_txt_srp[entry].it_txt_entry_nr -1].it_txt);
			} else {
				printf("Application_Reserved_1D: \n");
			}
			do_tabs(nr_of_tabs);
			printf("Please send a bug report\n");
			break;

	}


	return(nr_of_tabs);
}
