#include <time.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <libgen.h>
#define BLOCK 512

char* name;
char path[50];
char func[50];
char outputPath[50];
long file_size(char *name);
void logs();
char buff[512];
char dateLog[50];
char dateTimeLog[50];
int store();
char * getDate();
char * getDateTime();
int size;


void logs(){
      FILE *logPath;
	chdir("./logs");
     logPath = fopen(dateLog, "a");
	getDateTime();
        if(strcmp(func, "store") ==0){
                fputs(dateTimeLog, logPath);
                fputs(" File successfully stored\n", logPath);
        }
        else if(strcmp(func, "delete")==0){
		fputs(dateTimeLog, logPath);
                fputs(" File successfully deleted\n", logPath);
        }
	else if(strcmp(func, "retrieve")==0){
		fputs(dateTimeLog, logPath);
		fputs(" File successfully retrieved\n", logPath);
	}
        fclose(logPath);
        chdir("..");
}


void retrieve(){
	int i;
	char ch;
	char retrievePath[50];
	char blockName[10]={"BLOCK"};
	FILE *fp1, *fp2;
	fp1 = fopen(outputPath, "a");
	for(i=0;i<=8;i++)
	{
		sprintf(retrievePath, "./DISK/%s%d", blockName, i);
		chdir(retrievePath);
		fp2 = fopen(path,"r");
		if(fp2 == NULL)
		{
			break;
		}
		while((ch = fgetc(fp2)) != EOF)
		{
			fputc(ch, fp1);
		}
		fclose(fp2);
		chdir("..");
		chdir("..");
	}
	chdir("..");
	chdir("..");
	fclose(fp1);
	

}
void del(){
    
	int status;
	int i;
	char blockPath[50];
	char blockName[10]={"BLOCK"};
	for(i=0;i<=8;i++){
		sprintf(blockPath, "./DISK/%s%d",blockName,i);
		chdir(blockPath);
		status = remove(path);
		chdir("..");
		chdir("..");
	}
        /

}
char * getDateTime(){
        time_t t;
        struct tm *info;
        time(&t);
        info = localtime(&t);
        strftime(dateTimeLog, 12, "%r",info);
        return dateTimeLog;
}
char * getDate()
{
        time_t t;
        struct tm *info;
        time(&t);
        info = localtime(&t);
        strftime(dateLog, 12, "%d:%m:%Y",info);
        return dateLog;
}
int store(){  // logic to split blocks is also found here
        int segments=0, i, len, accum;
	char* fileTitle = basename(path);
    	FILE *fp1, *fp2;
    	long sizeFile = file_size(path);// gets filelength
    	segments = sizeFile/BLOCK + 1;
        char storedFileName[260];//new file name for file containing 512 byte block
    	char line[1080];
	char diskLocation[50];
	//TODO: Update this using the malloc function so I can dynamic memory allocation. 
    	fp1 = fopen(path, "r");
    	if(fp1)
    	{
        	for(i=0;i<segments;i++)
        	{
            		accum = 0;	 //This is the identifier used when naming files.
			sprintf(diskLocation,"./DISK/BLOCK%d", i);
			chdir(diskLocation);
            		sprintf(storedFileName, "%s",fileTitle, i);
            		fp2 = fopen(storedFileName, "w");
            		if(fp2)
            		{
                		while(fgets(line, 1080, fp1) && accum <= BLOCK)// Runs code until our previously set blocksize is reached
                		{
                    			accum += strlen(line);
                    			fputs(line, fp2);//prints next line
                		}
                		fclose(fp2);
				chdir("..");
				chdir("..");
            		}
        	}
        	fclose(fp1);
	}
	return 0;
}

int main(int argc, char *argv[])
{
        getDate();
        getDateTime();
	store();
        
        if(argc == 3) // Must be 3 args to run this method. The code here is fairly simple so I haven't put many comments for it.
        {
            strcpy(path, argv[2]); 
	        strcpy(func, argv[1]);
            if(strcmp(argv[1],"store")==0){
			store();
			logs();
		}

        else if(strcmp(argv[1],"delete")==0){
                        del();
			logs();
                }
		else if(strcmp(argv[1],"retrieve")==0){
                        printf("The retreive function must take 2 arguments\nExample: ./myfs retreive test.txt Output/test.txt\n");//Provides help if user tries to run the retreive function with only 1 file argument.
                }

                else{
                        printf("Not a valid function\n");
                        exit(0);
                }

        }
	else if(argc == 4)
	{
		strcpy(path, argv[2]);
		strcpy(func, argv[1]);
		strcpy(outputPath, argv[3]);
		if(strcmp(argv[1],"retrieve")==0){
			retrieve();
			logs();
                }
		else{
			printf("Only the retrieve function takes 4 arguments\n");
			exit(0);
		}

	}
        else
        {
        printf("\nYou must input the function and filename(s) you would like to use as arguments.\n");
        printf("Example: ./myfs store ./Documents/data.txt\n");
		printf("Example: ./myfs delete test.txt\n");
		printf("Example: ./myfs retreive test.txt ./Output/test.txt\n");
		printf("Information on successful running of the program can be found in the log files\n");
                exit(0);
        }
	
        return 0;
}


// The code below is used to determine the file size.
// Editing the below code might create an error in the sysetm where it appears to be working fine, but is making inaccuate calculations.

long file_size(char *name)
{
    FILE *fp = fopen(name, "rb"); 

    long size=-1;
    if(fp)
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp)+1;
        fclose(fp);
    }
    return size;
}


