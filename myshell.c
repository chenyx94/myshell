#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> //to get the current date and time
#include <unistd.h> //to use system call fork()
#include <sys/types.h>//to use system call waitpid
#include <sys/wait.h>//to use system call waitpid
#include <errno.h>//to get the return error


#define TRUE 1
#define MAX 20
#define MAXINPUT 4096

int shell_prompt();//initialize the shell prompt
int read_cmd(char **arg);//deal with the command

int main(int argc, const char * argv[]) {
    
    char *pathname;//the path of the command
    pid_t ChildPid;//the pid of the child process
    int status;
    char **arg;//the argument of the execv()
    int j,k;
    while(TRUE){
        
        arg=(char **)malloc(MAX*sizeof(char*));//malloc memory for **arg
        shell_prompt();//initialize the shell prompt
        k=read_cmd(arg);//deal with the command
        
        if(arg[0]==NULL)
            continue;//if the command is empty, continue to the next line
        else if(strcmp(arg[0],"exit")==0)
            exit(0);//while the command is exit
        
        pathname=(char*)malloc(sizeof(char)*50);//malloc memory for pathname
        strcpy(pathname,"/bin/");//initialize the path
        strcat(pathname,arg[0]);//add the command to the path
        
        //fork a child process
        ChildPid=fork();
        if(ChildPid!=0)//parent process
        {
            waitpid(ChildPid,&status,0);
        }
        else//child process
        {
            int a;
            a=execv(pathname,arg);
            if(a==-1){
                printf("error code: %d (%s)\n",errno,strerror(errno));//return the error
                return 0;
            }
        }
        
        //free the memory of argument
        for(j=0;j<=k;j++)
        {
            free(arg[j]);
            arg[j]=NULL;
        }
        free(arg);
        arg=NULL;
        //free the memory of pathname
        free(pathname);
        pathname=NULL;
    }
    
    return 0;
}

//function to deal with the command
int read_cmd(char **arg)
{
    char *ch;
    int i=0;
    unsigned long length=0;
    char buffer[MAXINPUT];
    
    fgets(buffer,MAXINPUT,stdin);//store the command into buffer
    length=strlen(buffer);//get the length of the command
    buffer[length-1]=' ';//convert the last '\n' to ' '
    ch=strtok(buffer," ");//get the first string
    while(ch!=NULL)
    {
        arg[i]=(char *)malloc(sizeof(char)*MAX);//malloc size for array arg[i]
        strcpy(arg[i++],ch);//copy the command into the array arg[i], then i++
//        printf("%s",arg[i-1]);
        ch=strtok(NULL," ");//get the next string
    }
    arg[i]=NULL;
    return i;//return the number of the argument
}

//function to initialize the shell prompt
int shell_prompt()
{
    time_t t;//define the variable for time
    struct tm *timeinfo;
    time(&t);//get the time information
    timeinfo=localtime(&t);//invert the time into local time
    printf("%4d-%02d-%02d %02d:%02d:%02d#",1900+timeinfo->tm_year,1+timeinfo->tm_mon,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);//print the time and # before every command line
    return 0;
}
