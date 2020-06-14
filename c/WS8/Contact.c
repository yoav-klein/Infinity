/******************************************************
*				Writing and reading contacts from file
*				Author: Yoav Klein
*				Reviewer: Alex Zilberman
*				Date: 23.5.19
************************************************************


#include <stdio.h> /* printf */
#include <string.h> /* strcpy */
#include <stdlib.h> /* malloc */

#define LIST_LEN 10
#define FILE_NAME "People.dat"

struct date_of_birth
{
	size_t day;
	size_t month;
	size_t year;
};

typedef struct contact
{
	char f_name[50];
	char l_name[50];
	struct date_of_birth dob;
	size_t ID;
} person_t;

person_t *GetContacts()
{	
	person_t *init_arr = (person_t*)malloc(LIST_LEN * sizeof(person_t));
	if(NULL == init_arr)
	{
		printf("Allocation failed\n");
		exit(1);
	}
	
	strcpy(init_arr[0].f_name, "Nir");
	strcpy(init_arr[0].l_name, "Klinger");
	init_arr[0].dob.day = 1;
	init_arr[0].dob.month = 1;
	init_arr[0].dob.year = 1965;
	init_arr[0].ID = 21321421;
	
	strcpy(init_arr[1].f_name, "Alon");
	strcpy(init_arr[1].l_name, "Mizrachi");
	init_arr[1].dob.day = 2;
	init_arr[1].dob.month = 2;
	init_arr[1].dob.year = 1978;
	init_arr[1].ID = 24382908;
	
	strcpy(init_arr[2].f_name, "Eli");
	strcpy(init_arr[2].l_name, "Ohana");
	init_arr[2].dob.day = 5;
	init_arr[2].dob.month = 10;
	init_arr[2].dob.year = 1940;
	init_arr[2].ID = 1413248;
	
	strcpy(init_arr[3].f_name, "Mota");
	strcpy(init_arr[3].l_name, "Shpigler");
	init_arr[3].dob.day = 7;
	init_arr[3].dob.month = 10;
	init_arr[3].dob.year = 1970;
	init_arr[3].ID = 984378923;
	
	strcpy(init_arr[4].f_name, "Alon");
	strcpy(init_arr[4].l_name, "Harazi");
	init_arr[4].dob.day = 9;
	init_arr[4].dob.month = 10;
	init_arr[4].dob.year = 1975;
	init_arr[4].ID = 89432789;
	
	strcpy(init_arr[5].f_name, "Yossi");
	strcpy(init_arr[5].l_name, "Abuxis");
	init_arr[5].dob.day = 9;
	init_arr[5].dob.month = 9;
	init_arr[5].dob.year = 2010;
	init_arr[5].ID = 47389723;
	
	strcpy(init_arr[6].f_name, "Eyal");
	strcpy(init_arr[6].l_name, "Berkovits");
	init_arr[6].dob.day = 1;
	init_arr[6].dob.month = 10;
	init_arr[6].dob.year = 1988;
	init_arr[6].ID = 78392789;
	
	strcpy(init_arr[7].f_name, "Walid");
	strcpy(init_arr[7].l_name, "Badir");
	init_arr[7].dob.day = 10;
	init_arr[7].dob.month = 12;
	init_arr[7].dob.year = 1922;
	init_arr[7].ID = 9843789;
	
	strcpy(init_arr[8].f_name, "Zinadin");
	strcpy(init_arr[8].l_name, "Zidan");
	init_arr[8].dob.day = 10;
	init_arr[8].dob.month = 12;
	init_arr[8].dob.year = 1999;
	init_arr[8].ID = 43789232;
	
	strcpy(init_arr[9].f_name, "Dudu");
	strcpy(init_arr[9].l_name, "Awat");
	init_arr[9].dob.day = 1;
	init_arr[9].dob.month = 6;
	init_arr[9].dob.year = 1970;
	init_arr[9].ID = 45453434;
	
	return init_arr;		
}

int WriteToFile(person_t *arr)
{

	int items_written = 0;
	FILE *fp = NULL;
	
	fp = fopen(FILE_NAME, "w");
	if(NULL == fp)
	{
		printf("Couldn't open file\n");
		return 0;
	}
	items_written = fwrite(arr, sizeof(person_t), LIST_LEN, fp);
	
	fclose(fp);
	free(arr);
	
	if (LIST_LEN != items_written)
	{
		return 0;
	}
	
	printf("Array written to file successfully\n");
	return 1;
}

int ReadFromFile(person_t *arr)
{
	int status = 0;
	FILE *fp = NULL;
	
	fp = fopen(FILE_NAME, "r");
	if(NULL == fp)
	{
		return 0;
	}
	
	status = fread(arr, sizeof(person_t), LIST_LEN, fp);
	if(LIST_LEN != status)
	{
		return 0;
	}
	
	remove(FILE_NAME);
	
	return 1;
}

int main()
{
	person_t *init_arr_p = GetContacts();
	person_t *final_arr_p = (person_t*)malloc(LIST_LEN * sizeof(person_t));
	
	int status = WriteToFile(init_arr_p);
	
	if(0 == status)
	{
		printf("Failed writing to file\n");
		return 0;
	}
	
	status = ReadFromFile(final_arr_p);
	if(0 == status)
	{
		printf("Failed reading from file\n");
	}
	
	printf("Contacts read successfully from file\n");
	
	return 1;
}



