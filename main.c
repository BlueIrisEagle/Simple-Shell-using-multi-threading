#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    NoAmpersand = 0,
    GotAmpersand = 1
} enAmpersand;

unsigned short int iterator = 0;
unsigned char ampFlag = NoAmpersand;
unsigned char stringNo = 0;
unsigned char waitstate;

char *arrStr2[10];
void tokenizer2(char *stri)
{

    char *temp = malloc(sizeof(char));

    temp = strtok(stri, " ");

    arrStr2[0] = temp;
   

    while (temp != NULL)
    {

        stringNo++;

        temp = strtok(NULL, " ");

        if (temp != NULL)
        {
            arrStr2[stringNo] = temp;


            if (!strcmp(temp, "&"))
            {
                arrStr2[stringNo] = NULL;
                ampFlag = GotAmpersand;
            }
            else
            {
                arrStr2[stringNo] = temp;
            }
        }
    }

    free(temp);
    return;
}


void handler(int sig)
{
    time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

    FILE* fptr=fopen("log.txt","a");

    fprintf(fptr,"Child terminated at %s",asctime(timeinfo));
    

    fclose(fptr);
}

int main(void)
{
    int i;
    char stri[] = "exit";
    char input[30];

        
    
    while (1)
    {
        
        
        waitstate=99;
        stringNo = 0;
        printf(">>>");
        getchar();
        
        scanf("%[^\n]s", input);


        pid_t childState;

    

        tokenizer2(input);

        childState = fork();                       //Creating a child

        // fork validation
        if (childState < 0)
        {
            printf("Fork Failed");      
            exit(-1);
        }

        if(strcmp(arrStr2[0], "cd") == 0)   // Handling directory change
        chdir(arrStr2[1]);

        if (strcmp(arrStr2[0], "exit") == 0) // Exiting handling
            exit(0);

        if (childState == 0) //child is working
        {
            execvp(arrStr2[0],arrStr2);
            waitstate=0;
            signal(SIGCHLD,handler); //Sending Signal when child is terminated
        }
        else                            //parent role
        {
            if (ampFlag == GotAmpersand)
            {
                /*run in background*/
            }
            else
            {
                //waitpid(-1, &waitstate, 0); /*Waiting for one of its child to die*/
                wait(NULL);
                
            }
        }

    }

    return 0;
}
