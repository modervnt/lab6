#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_DATE_LENGTH 11
#define MAX_FILENAME_LENGTH 50
#define MAX_LINES 100


typedef struct student {
    char Name[MAX_NAME_LENGTH];
    char date[MAX_DATE_LENGTH];
    int SES[3];
} Student;

Student *records;
int numLines;

char* mystrtok(char* str, const char* delim) {
    static char* next;

    if (str) {
        next = str;
        while (*next && strchr(delim, *next))
            *next++ = '\0';
    }

    if (!*next) {
        str = NULL;

    }
    else {
        str = next;

        while (*next && !strchr(delim, *next)) {
            ++next;
        }

        while (*next && strchr(delim, *next))
            *next++ = '\0';

    }

    return str;
}

int numberOfLines(FILE *file) {
    int count = 0;
    char line[MAX_NAME_LENGTH];
    
    if (file != NULL) {
        while (fgets(line, MAX_NAME_LENGTH, file) != NULL) {
            count++;
        }
        fseek(file, 0, SEEK_SET);  /* Réinitialiser la position du fichier au début*/
    }
    return count;
}




void read_and_save_data() {
     FILE *file;
     char filename[50] = "lab6.csv";
     file = fopen(filename, "r");

    if (file == NULL) {
        printf("Échec de l'ouverture du fichier");
        exit(1);
    }

    numLines = numberOfLines(file);
    records = malloc(numLines * sizeof(Student));

    if (records == NULL) {
        printf("Échec d'allocation de mémoire");
        exit(1);
    }

    char line[MAX_NAME_LENGTH];
    char *token;
    int i = 0;
    while (fgets(line, MAX_NAME_LENGTH, file) != NULL) {
        token = mystrtok(line, ",");
        if (token != NULL) {
            strncpy(records[i].Name, token, MAX_NAME_LENGTH - 1);
        }
        token = mystrtok(NULL, ",");
        if (token != NULL) {
            strncpy(records[i].date, token, MAX_DATE_LENGTH - 1);
        }
        for (int j = 0; j < 3; j++)
        {   
            token = mystrtok(NULL, ",");
            if (token != NULL) {
                records[i].SES[j] = atoi(token);
            }
        }
        i++;
    }
    fclose(file);
}

int year_converter_and_tester(char *date){
    char temp[15];
    strncpy(temp, date, MAX_DATE_LENGTH - 1);

    char *token;
    int yrs = 0;
    token = mystrtok(date,"/");
    if (token != NULL)
    {
        yrs = atoi(token);
    }

    token = mystrtok(NULL,"/");
    if (token != NULL)
    {
        yrs = yrs + (atoi(token)*30);
    }

    token = mystrtok(NULL,"/");
    if (token != NULL)
    {
        yrs = yrs + (atoi(token)*365);
    }
    /*01.12.1997 == 729277;*/
    if (yrs >= 729277)/* l'age ne dépasse pas 20 an a cette date*/
    {  
        return 1;
    }else{
        return 0;
    } 
    strncpy(date,temp, MAX_DATE_LENGTH - 1);
}

void sort_arrayBySES(){
    for (int i = 0; i < numLines - 1; i++)
    {
        for (int j = 0; j < numLines -1 -i; j++)
        {
            /*calculate the average and sort*/
            int average1 = (records[j].SES[0] + records[j].SES[1] + records[j].SES[2]);
            int average2 = (records[j+1].SES[0] + records[j+1].SES[1] + records[j+1].SES[2]);
            if (average1 > average2)
            {
                Student temp = records[j];
                records[j] = records[j+1];
                records[j+1] = temp;
            }
        }  
    } 
}

void print_student(){
    sort_arrayBySES();

    int k = 0;
    for (int i = 0; i < numLines; i++)
    {
        if(year_converter_and_tester(records[i].date)){
            k++;
            if (k == 1)
            {
                printf("|Name           |date \t        | SES  | \n");
            }
            printf("|%s\t| %s\t| %d %d %d|\n",records[i].Name,records[i].date, records[i].SES[0], records[i].SES[1], records[i].SES[2]); 
        }
    
    }
    if (k == 0)
    {
        printf("No students\n");
    }
     
}

int main(){
    read_and_save_data();
    print_student();    
    free(records);
    return 0;
}
