//
//  Created by Behnam Dezfouli
//  CSEN, Santa Clara University
//
//
// This program offers two options to the user:
// -- Option 1: enables the user to copy a file to another file. 
// The user needs to enter the name of the source and destination files.
// -- Option 2: enables the user to measure the performance of file copy. 
// In addition to the name of source and destination files, the user needs to enter the maximum source file size as well as the step size to increment the source file size.
//
//
// double copier(FILE *s_file, FILE *d_file)
// Precondition: s_file is a pointer to a file opened in read mode, d_file is a file pointer opened in write mode
// Postcondition: Copies the contents of the file pointed to by s_file to the file pointed to by d_file
//


#include <stdio.h>
#include <stdlib.h> // For exit()
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 1
#define DUM_CHAR 'A'


//function used in Option 1
void fcopier(FILE *s_file, FILE *d_file)
{
    char buffer[SIZE];
    while(fread(buffer, SIZE, 1, s_file) == 1){
        fwrite(buffer, SIZE, 1, d_file);
    }
    
}
void scopier(int s_file, int d_file){
    char buffer[SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(s_file, buffer, SIZE)) > 0) {
        write(d_file, buffer, bytes_read);
    }

}



int main()
{
    
    //Filenames and Option
    char infile[100], outfile[100];
    int option;

    //Get User Inputs
    printf("Source File: ");
    fgets(infile, sizeof(infile), stdin);
    printf("Destination File: ");
    fgets(outfile, sizeof(outfile), stdin);
    printf("Option: ");
    scanf("%d", &option);

    if ( option == 1 ) // File copy
    {
        //Open File
        FILE *s_file1 = fopen(infile, "r");
        FILE *d_file1 = fopen(outfile, "wb");
        if(s_file1 == NULL || d_file1 == NULL){
            printf("Opening file with function call fails");
            exit(-1);
        }
        int s_file2 = open(infile, O_RDONLY);
        int d_file2 = open(strcat("systemcall", outfile), O_WRONLY | O_CREAT);
        if(s_file2 == -1 || d_file2 == -1){
            printf("Opening file with system call fails");
            exit(-1);
        }

        fcopier(s_file1, d_file1);
        scopier(s_file2, d_file2);

        printf("File copied.\n");
        fclose(s_file1);
        close(s_file2);
        fclose(d_file1);
        close(d_file2);
    }
    
    else if ( option == 2 ) // File copy with performance measurement
    {
        long maximum_file_size, step_size, current_size = 0;
        printf("Enter the maximum_file_size: ");
        scanf("%ld", &maximum_file_size);

        printf("Enter the step_size: ");
        scanf("%ld", &step_size);

        while(current_size < maximum_file_size){

            //Dynamic file name
            snprintf(infile, sizeof(infile), "%ld.bin", current_size);

            FILE *s_file = fopen(infile, "wb+");
            FILE *d_file = fopen(outfile, "wb");
            if(s_file == NULL || d_file == NULL){
                printf("Opening file with function call fails");
                exit(-1);
            }
            current_size = current_size+step_size;
            long i;
            for (i = 0; i < current_size; i++) {
                fputc(DUM_CHAR, s_file);
            }

            clock_t start, end;
            double cpu_time_used;
            start = clock();

            fcopier(s_file, d_file);

            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

            printf("%d byte used %f seconds.\n", current_size, cpu_time_used);

            fclose(s_file);
            fclose(d_file);
        }

    }
    
    else
    {
        printf("Invalid option!");
        return 0;
    }
    
    return 0;
}

