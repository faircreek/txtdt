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

#define CHECK_ZERO(arg) \
  if(memcmp(my_friendly_zeros, &arg, sizeof(arg))) { \
    unsigned int i_CZ; \
    fprintf(stderr, "*** Zero check failed in %s:%i\n    for %s = 0x", \
            __FILE__, __LINE__, # arg ); \
    for(i_CZ = 0; i_CZ < sizeof(arg); i_CZ++) \
      fprintf(stderr, "%02x", *((uint8_t *)&arg + i_CZ)); \
    fprintf(stderr, "\n"); \
  }
static const uint8_t my_friendly_zeros[2048];

/**
 * Usage :)
 */

void usage(){
	fprintf(stderr,"Usage: dvdtxt -n filename\n");
	exit(-1);
}


/**
 * Main
 */
int main(int argc, char *argv[]){

	/* Program control */
	int flags;
	char *file_name;

	/* File handlers */
	int file;

	/* TXTDT_MG structure */
	txtdt_mg_t *txtdt_mg;

	/* Errors in while parsing */
	int errors = 0;


	while ((flags = getopt(argc, argv, "n:")) != -1) {
		switch (flags) {
			case 'n':
				if(optarg[0]=='-') usage();
				file_name = optarg;
				break;
			default:
				usage();
		}
	}




	if ( (file = open(file_name, O_RDONLY)) == -1 ) {
		fprintf(stderr, "Failed to open %s\n", file_name);
		usage();
	}

	txtdt_mg = Read_TXTDT_MG(file);

	if (txtdt_mg == NULL) {
		fprintf(stderr,"Faild to read TXTDT_MG structure\n");
		exit(-2);
	}

	errors = Parse_TXTDT_MG(txtdt_mg);

	if(errors != 0){
		printf("Total number of errors in TXTDT_MG structure is %d\n", errors);
	}

	Free_TXTDT_MG(txtdt_mg);

	return(errors);

}


static void free_internal(int i, txtdt_mg_t * txtdt_mg) {

	int j, k;

	if(i == 0) {
		free(txtdt_mg->txtdt_lu_srp);
		free(txtdt_mg);
		return;
	}
	for(j=0; j < i ; j++) {
		free(txtdt_mg->txtdt_lu_srp[j].txtdt_lu->txtdt->it_txt_srp);
		for(k = 0; k < txtdt_mg->txtdt_lu_srp[j].txtdt_lu->txtdt->nr_of_it_txt; k++) {
			free(txtdt_mg->txtdt_lu_srp[j].txtdt_lu->txtdt->it_txt[k].it_txt);
		}
		free(txtdt_mg->txtdt_lu_srp[j].txtdt_lu->txtdt->it_txt);
		free(txtdt_mg->txtdt_lu_srp[j].txtdt_lu->txtdt);
		free(txtdt_mg->txtdt_lu_srp[j].txtdt_lu);
	}
	free(txtdt_mg->txtdt_lu_srp);
	free(txtdt_mg);


}


txtdt_mg_t * Read_TXTDT_MG(int file){


	/* Offsets */
	off_t offset;
	off_t base_offset;
	uint32_t sector;

	/* Interation*/
	int i, j, k;

	/* Zeros */

	uint8_t zero8;
	uint16_t zero16;
	uint32_t zero32;

	/* Structures */
	txtdt_mg_t *txtdt_mg;
	txtdt_lu_srp_t *txtdt_lu_srp;
	txtdt_lu_t *txtdt_lu;
	it_txt_srp_sa_t *it_txt_srp_sa;
	txtdt_t *txtdt;
	it_txt_srp_t *it_txt_srp;
	it_txt_t  *it_txt;


	/* Temporary arrays and variables */
	uint16_t  array16[100];
	uint8_t  *string_array;
	int  string_array_size;
	int  string_number;
	int  number_of_strings;
	int  string_size;
	int  string_index;
	int  string_offset;
	uint8_t  byte8;
	uint16_t  byte16;
	uint32_t  byte32;

	/* Get the TXTDT_MG offset from the VGMI table
	Return if it's zero i.e. no TXTDT_MG table*/
#ifdef DEBUG
	fprintf(stderr,"In Read_TXTDT_MG \n");
#endif
	offset = 212;

	if (lseek(file, offset, SEEK_SET) != offset ) {
		fprintf(stderr, "Failed to seek the file\n");
		return(0);
	}


	if ( read(file, &sector, sizeof(sector)) !=  sizeof(sector) ) {
		fprintf(stderr, "Failed to read the file\n");
		return(0);
	}

	B2N_32(sector);

	if( sector == 0 ) {
		fprintf(stderr,"No TXTDT_MG in this IFO\n");
		close(file);
		return(0);
	}
#ifdef DEBUG
	fprintf(stderr,"In Read_TXTDT_MG 2\n");
#endif


	/* Read the TXTDT_MG header table */

	offset = sector * 2048;
	base_offset = offset;

	txtdt_mg = (txtdt_mg_t *)malloc(sizeof(txtdt_mg_t));

	if(!txtdt_mg){
		return(0);
	}

        if (lseek(file, offset, SEEK_SET) != offset ) {
                fprintf(stderr, "Failed to seek VIDEO_TS.IFO\n");
		free(txtdt_mg);
                return(0);
        }

        if ( read(file, txtdt_mg->txtdt_id, sizeof(char) * 12) !=  sizeof(char) * 12 ) {
                fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
		free(txtdt_mg);
                return(0);
        }

	txtdt_mg->txtdt_id[12]='\0';

#ifdef DEBUG
	fprintf(stderr,"The ID string is %s\n", txtdt_mg->txtdt_id);
#endif


	/* Check the unknown zero */
        if ( read(file, &zero16, sizeof(uint16_t)) !=  sizeof(uint16_t)) {
                fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
		free(txtdt_mg);
                return(0);
        }

	B2N_16(zero16);
	CHECK_ZERO(zero16);

        if ( read(file, &byte16, sizeof(uint16_t)) !=  sizeof(uint16_t)) {
                fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
		free(txtdt_mg);
                return(0);
        }

	B2N_16(byte16);
	txtdt_mg->nr_of_txtdt_lus = byte16;


        if ( read(file, &byte32, sizeof(uint32_t)) !=  sizeof(uint32_t)) {
                fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
		free(txtdt_mg);
                return(0);
        }

	B2N_32(byte32);
	txtdt_mg->end_byte = byte32;

	txtdt_lu_srp = (txtdt_lu_srp_t *)malloc(sizeof(txtdt_lu_srp_t) * txtdt_mg->nr_of_txtdt_lus);

	txtdt_mg->txtdt_lu_srp = txtdt_lu_srp;

	if(!txtdt_mg->txtdt_lu_srp) {
		free(txtdt_mg);
                return(0);
	}

#ifdef DEBUG
	fprintf(stderr,"nr_of_txtdt_lus is %d \n", txtdt_mg->nr_of_txtdt_lus);
#endif
	/* Read the TXTDT_LU_SRPs */

	for(i=0; i < txtdt_mg->nr_of_txtdt_lus; i++) {

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the TXTDT_LU_SRPs \n");
#endif
		if ( read(file, txtdt_lu_srp[i].txtdt_lcd , sizeof(char) * 2) !=  sizeof(char) *2) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free(txtdt_lu_srp);
			free(txtdt_mg);
			return(0);
		}

		txtdt_lu_srp->txtdt_lcd[2]='\0';

		if ( read(file, &zero8, sizeof(uint8_t)) !=  sizeof(uint8_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free(txtdt_lu_srp);
			free(txtdt_mg);
			return(0);
		}

		CHECK_ZERO(zero8);

		if ( read(file, &byte8, sizeof(uint8_t)) !=  sizeof(uint8_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free(txtdt_lu_srp);
			free(txtdt_mg);
			return(0);
		}

		txtdt_lu_srp[i].char_set = byte8;

		if ( read(file, &byte32, sizeof(uint32_t)) !=  sizeof(uint32_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free(txtdt_lu_srp);
			free(txtdt_mg);
			return(0);
		}


		B2N_32(byte32);
		txtdt_lu_srp[i].start_byte = byte32;
#ifdef DEBUG
		fprintf(stderr,"TXTDT_LCD is %s\n",txtdt_lu_srp->txtdt_lcd);
		fprintf(stderr,"Startbyte LU number %d is %08x\n", i + 1, txtdt_lu_srp[i].start_byte);
		fprintf(stderr,"END - In Read_TXTDT_MG - Read the TXTDT_LU_SRPs \n");
#endif



	}

	for(i=0; i < txtdt_mg->nr_of_txtdt_lus; i++) {

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the TXTDT_LU I is %d\n", i);
#endif

		/* Read TXTDT_LU */

		txtdt_lu = (txtdt_lu_t *)malloc(sizeof(txtdt_lu_t));
		if(!txtdt_lu) {
			free_internal(i, txtdt_mg);
			return(0);
		}

		txtdt_lu_srp[i].txtdt_lu = txtdt_lu;

		offset = base_offset + txtdt_lu_srp[i].start_byte;

		if (lseek(file, offset, SEEK_SET) != offset ) {
			fprintf(stderr, "Failed to seek VIDEO_TS.IFO\n");
			free_internal(i, txtdt_mg);
			return(0);
		}

		if ( read(file, &byte32, sizeof(uint32_t)) !=  sizeof(uint32_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free_internal(i, txtdt_mg);
			return(0);
		}

		B2N_32(byte32);
		txtdt_lu->end_byte = byte32;

#ifdef DEBUG
		fprintf(stderr,"Offset is  %08x\n", offset);
		fprintf(stderr,"Endbyte is  %08x\n", byte32);
#endif


       		if ( read(file, &array16, sizeof(uint16_t) * 100) !=  sizeof(uint16_t)* 100) {
       		         fprintf(stderr, "Faild to read VIDEO_TS.IFO\n");
			 free_internal(i, txtdt_mg);
       		         return(0);
       		}

                for(j=0; j < 100; j++) {
			B2N_16(array16[j]);
		}

		/* Now we know that the dist between the start of TXTDT_LU_SRP
		and the first entry in the TXTDT_LU is 0xCE and we also
		know that each entry is 4 bytes hence we can take
		"(array16[x] - 0xCE)/4" and get a reference number to the entry
		we search (We know the mount of "offsets" == tt_srpts + 1
		but that is not always the truth so we will use the below
		instead NOTE We add 1 to enable 0 == false i.e. no entry*/



		for(j=0; j < 100; j++) {
			if( array16[j] != 0) {
				txtdt_lu->it_txt_srp_sa[j] = (array16[j] - 0xCE)/4 + 1;
#ifdef DEBUG
				printf("Entry nr is: %d\n",txtdt_lu->it_txt_srp_sa[j]);
#endif
			} else {
				txtdt_lu->it_txt_srp_sa[j] = 0;
			}
		}

		/* Read TXTDT */
#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the TXTDT \n");
#endif


		txtdt = (txtdt_t *)malloc(sizeof(txtdt_t));
		if(!txtdt) {
			free_internal(i, txtdt_mg);
			free(txtdt_lu);
			return(0);
		}

		txtdt_lu->txtdt = txtdt;


		if ( read(file, &byte16, sizeof(uint16_t)) !=  sizeof(uint16_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free_internal(i, txtdt_mg);
			free(txtdt);
			free(txtdt_lu);
			return(0);
		}


		B2N_16(byte16);
		txtdt->nr_of_it_txt_srps = byte16;


		if ( read(file, &zero16, sizeof(uint16_t)) !=  sizeof(uint16_t)) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free_internal(i, txtdt_mg);
			free(txtdt);
			free(txtdt_lu);
			return(0);
		}

		B2N_16(zero16);
		CHECK_ZERO(zero16);


		it_txt_srp = (it_txt_srp_t *)malloc(sizeof(it_txt_srp_t) * txtdt->nr_of_it_txt_srps);

		if(!it_txt_srp) {
			free_internal(i, txtdt_mg);
			free(txtdt);
			free(txtdt_lu);
			return(0);
		}

		txtdt->it_txt_srp = it_txt_srp;


		/* Read all IT_TXT_SRPs */

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT_SRP \n");
#endif


		for(j=0; j < txtdt->nr_of_it_txt_srps; j++) {


			if ( read(file, &byte8, sizeof(uint8_t)) !=  sizeof(uint8_t)) {
				fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
				free_internal(i, txtdt_mg);
				free(it_txt_srp);
				free(txtdt);
				free(txtdt_lu);
				return(0);
			}

			it_txt_srp[j].it_txt_idcd = byte8;

			if ( read(file, &zero8, sizeof(uint8_t)) !=  sizeof(uint8_t)) {
				fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
				free_internal(i, txtdt_mg);
				free(it_txt_srp);
				free(txtdt);
				free(txtdt_lu);
				return(0);
			}

			CHECK_ZERO(zero8);

			if ( read(file, &byte16, sizeof(uint16_t)) !=  sizeof(uint16_t)) {
				fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
				return(0);
			}

			B2N_16(byte16);
			it_txt_srp[j].it_txt_sa = byte16;


			if (it_txt_srp[j].it_txt_sa == 0) {
				it_txt_srp[j].it_txt_entry_nr = 0;
			} else {
				it_txt_srp[j].it_txt_entry_nr = 1;
			}
		}

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT \n");
#endif


		/* Read all IT_TXT's ie the big string */


		/* Now we read all stings, it so that a string can be shared
		hence several it_txt_srps can point to the same string. Hence we can't
		store the string in the entry since we will end up with duplications
		it's also less effective when we later write strings with IFOedit. You should also
		know that the string is not always a string it may be a category/sorting Code
		number :( See the parser section

		Anyways we know the endbyte of the TXTDT_LU we know where the strings starts relative
		to lu ( 4 + 200 + 2 + 2 (nr_entries * 4) */

		string_array_size = txtdt_lu->end_byte -  (4 + 200 + 2 + 2 + (txtdt->nr_of_it_txt_srps * 4)) + 1;

		/* Make a temporaty array that we use to read the string */
		string_array = (uint8_t *)malloc(string_array_size * sizeof(uint8_t));
#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 2\n");
#endif


		if ( read(file, string_array, sizeof(uint8_t) * string_array_size) !=
						sizeof(uint8_t) *string_array_size ) {
			fprintf(stderr, "Failed to read VIDEO_TS.IFO\n");
			free_internal(i, txtdt_mg);
			free(it_txt_srp);
			free(txtdt);
			free(txtdt_lu);
			return(0);
		}

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 3\n");
#endif


		/* Count the number of strings */

		number_of_strings = 0;

		for(j = 0; j < string_array_size; j++) {
#ifdef DEBUG
			printf("String value is %02x \n",string_array[j]);
#endif
			if(string_array[j] == 0x09) {
					number_of_strings++;
#ifdef DEBGU
					printf("NUM string is %d\n",number_of_strings);
#endif
			}
		}

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 4\n");
#endif


		/* We can now malloc our array of string structures */
		it_txt = (it_txt_t *)malloc(number_of_strings * sizeof(it_txt_t));
		if(!it_txt) {
			free_internal(i, txtdt_mg);
			free(it_txt_srp);
			free(txtdt);
			free(txtdt_lu);
			return(0);
		}

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 5\n");
#endif

		txtdt->it_txt = it_txt;
		txtdt->nr_of_it_txt = number_of_strings;

		/* The offset of the first string is always 2 + 2 + (nr_entries * 4) */

		string_offset = 2 + 2 + (txtdt->nr_of_it_txt_srps * 4);
		string_number = 0;
		string_size=0;

		it_txt[0].string_offset = string_offset;
		it_txt[0].string_number = string_number + 1;

		for(j = 0; j < string_array_size; j++) {
			string_offset++;
			string_size++;
			if(string_array[j] == 0x09) {
				it_txt[string_number].string_size = string_size;
				string_size=0;
				string_number = string_number + 1;
				//printf("String offset is %04x\n",string_offset);
				//printf("String number is %d\n",string_number);
				if(string_number < number_of_strings) {
					it_txt[string_number].string_offset = string_offset;
					it_txt[string_number].string_number = string_number + 1;
				}
			}
		}

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 6\n");
#endif

		/* Now new know the size of each stinng hence we can malloc each IT_TXT */
		for(j = 0; j < number_of_strings; j++) {
			it_txt[j].it_txt = (uint8_t *)malloc(it_txt[j].string_size * sizeof(uint8_t));
			if(!it_txt[j].it_txt){
				free_internal(i, txtdt_mg);
				for(k=0; k < j; k++){
					free(it_txt[k].it_txt);
				}
				free(it_txt);
				free(it_txt_srp);
				free(txtdt);
				free(txtdt_lu);
				return(0);
			}
		}

		string_number = 0;
		string_index=0;

#ifdef DEBUG
		fprintf(stderr,"In Read_TXTDT_MG - Read the IT_TXT 7\n");
#endif


		for(j = 0; j < string_array_size && string_number < number_of_strings; j++) {
			//printf("String number: %d\n",string_number);
			//printf("String End: %d\n", it_txt[string_number].string_size -1 );
			//printf("String Index: %d\n",string_index);
			//printf("String entry is %02x\n",string_array[j]);
			if(it_txt[string_number].string_size == 0 ) {
				break;
			}
 			it_txt[string_number].it_txt[string_index] = string_array[j];
			string_index++;
			if(string_array[j] == 0x09) {
				it_txt[string_number].it_txt[string_index - 1] ='\0';
				string_number++;
				string_index=0;
			}
		}

		/* Free our temporary array it's not needed anymore */
		free(string_array);


		/* Now insert the right string number in each
		of the IT_TXT_SRPs */

		for(j = 0; j < txtdt->nr_of_it_txt_srps; j++) {
			if(it_txt_srp[j].it_txt_sa == 0) {
#ifdef DEBUG
				printf("Got a break type is: %02x \n", it_txt_srp[j].it_txt_idcd);
#endif
				continue;
			}
#ifdef DEBUG
			printf("Try to find offset %04x for type %02x \n", it_txt_srp[j].it_txt_sa, it_txt_srp[j].it_txt_idcd);
#endif
			for(k=0; k < number_of_strings; k++) {
				if(it_txt_srp[j].it_txt_sa == it_txt[k].string_offset) {
#ifdef DEBUG
					printf("Got it string offset is %04x - %04x\n", it_txt_srp[j].it_txt_sa, it_txt[k].string_offset);
#endif
					it_txt_srp[j].it_txt_entry_nr = it_txt[k].string_number;
					break;
				}
			}
		}
		
#ifdef DEBUG
		for(j = 0; j < txtdt->nr_of_it_txt_srps; j++) {
				printf("Type is %02x\n",it_txt_srp[j].it_txt_idcd);
				printf("String offset is: %04x\n", it_txt_srp[j].it_txt_sa);
		}

		for(j = 0; j < number_of_strings; j++) {
//			printf("\nString size is %d \n",it_txt[j].string_size);
			printf("String offset is %04x \n",it_txt[j].string_offset);
			//printf("String offset total is %04x \n", it_txt[j].string_offset + boffset + offsets[i] + 4 + 200);
			printf("String number is %d \n",it_txt[j].string_number);
//			printf("The string is %s\n",it_txt[j].it_txt);
		}

#endif


	}

	/* Ugh :) */
	return(txtdt_mg);

}


void Free_TXTDT_MG(txtdt_mg_t * txtdt_mg){
	int i;

	i = txtdt_mg->nr_of_txtdt_lus;
	free_internal(i, txtdt_mg);

}



