//Phone book processing program reads a random-access file sequentially, updates records already
//writes to the file, creates new record to be placed in the file, deletes records in the file, and prints
//records in the file.

#include<stdio.h>
#include <string.h>
#include<stdlib.h>

//Phone record structure definition
typedef struct {
	char lastName[20];
	char firstName[20];
	char phoneNum[15];
}phoneRecord;


int main(void) {

	printf("%s%c%c\n","Content-Type:text/html;charset=iso- 8859-1",13,10);
	printf("<!DOCTYPE html><html><head><title>Jerry's Phonebook</title></head><body>");
	char firstName[20];
	char lastName[20];
	char phoneNum[15];

	FILE * fPtr; //myPhoneBook file pointer
	char *formData;
	char qString[300];

	//fopen opens the file; exists if file cannot be opened
	if( (fPtr = fopen("myPhoneBook", "rb+") ) == NULL ) {
		printf("<p> Phonebook not found. <p>");

	}else{
		if(strcmp(getenv("REQUEST_METHOD"),"POST") == 0){
			int n = atoi(getenv("CONTENT_LENGTH"));
			fgets(qString, n+1, stdin);

			formData = strtok(qString, "=");
			formData = strtok(NULL, "&");
			if(strlen(formData) > 19){
				printf("<p> First Name exceeded allotted length. <p>");
				return;
			}else{
				strcpy(firstName, formData);
			}

			formData = strtok(NULL, "=");
			formData = strtok(NULL, "&");
			strcpy(lastName, formData);
			if(strlen(formData) > 19){
				printf("<p> Last Name exceeded allotted length. <p>");
				return;
			}else{
				strcpy(lastName, formData);
			}

			formData = strtok(NULL, "=");
			formData = strtok(NULL, "\0");
			if(strlen(formData) > 14){
				printf("<p> Phone number exceeded allotted length. <p>");
				return;
			}else{
				strcpy(phoneNum, formData);
			}

			updateRecord(fPtr, firstName, lastName, phoneNum);
		}

		if(strcmp(getenv("REQUEST_METHOD"), "GET") == 0){
			strcpy(qString, getenv("QUERY_STRING"));
			formData = strtok(qString, "=");
			formData = strtok(NULL, "&");
			if(strlen(formData) > 19){
				printf("<p> First Name exceeded allotted length. <p>");
				return;
			}else{
				strcpy(firstName, formData);
			}

			formData = strtok(NULL, "=");
			formData = strtok(NULL, "&");
			strcpy(lastName, formData);
			if(strlen(formData) > 19){
				printf("<p> Last Name exceeded allotted length. <p>");
				return;
			}else{
				strcpy(lastName, formData);
			}
			findPhoneNum(fPtr, firstName, lastName);
		}
	}
	fclose(fPtr);
	printf("</body></html>");
	return 0;
} //end main

void updateRecord(FILE *fPtr, char firstName[], char lastName [], char phoneNum){

	phoneRecord current;

	rewind(fPtr);
	int fileExists = 0;

	while(!feof(fPtr)){
		fread(&current, sizeof(phoneRecord), 1, fPtr);
		if (strcmp(current.lastName, lastName) == 0 && strcmp(current.firstName, firstName) == 0){
			strcpy(current.phoneNum, phoneNum);
			fseek(fPtr, -(sizeof(phoneRecord)), SEEK_CUR);
			fwrite(&current, sizeof(phoneRecord), 1, fPtr);
			printf("<p>Phone Record has been updated to %s \n \n <p>", current.phoneNum);
			fileExists = 1;
			return;
		}
	}

	if(fileExists == 0){
		addRecord(fPtr, firstName, lastName, phoneNum);
	}
	return;
}

void addRecord(FILE *fPtr, char firstName[], char lastName[], char phoneNum[]){

	phoneRecord newRecord;
	strcpy(newRecord.firstName, firstName);
	strcpy(newRecord.lastName, lastName);
	strcpy(newRecord.phoneNum, phoneNum);

	rewind(fPtr);
	int fileExists = 0;

	fseek(fPtr, -sizeof(phoneRecord), 2);
	fwrite(&newRecord, sizeof(phoneRecord), 1, fPtr);
	printf("<p>Your new contact has been added.</p>");
	return;
}


void findPhoneNum(FILE *fPtr, char firstName[], char lastName []){

	phoneRecord current;

	rewind(fPtr);
	int fileExists = 0;

	while(!feof(fPtr)){
		fread(&current, sizeof(phoneRecord), 1, fPtr);
		if (strcmp(current.lastName, lastName) == 0 && strcmp(current.firstName, firstName) == 0){

			printf("The number you are looking for is %s \n \n", current.phoneNum);
			fileExists = 1;
			return;
		}
	}

	if(fileExists == 0){
		printf("<p>The person you are looking for is not found. Please use Update to add a new phone number </p>");
	}
	return;
}



