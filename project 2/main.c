#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

//1 : ls
//2 : cd
//3 : clear
//4 : environ
//5 : echo
//6 : help
//7 : quit

char * f_parse_cmd(char *words_line);
char * f_parse_arg(char *words_line);
char* f_get_line(void);
int begin(void);

char* f_ls(char* arg);
int f_cd(char* arg);
void f_clear(void);
char* f_environ(void);
char* f_echo(char* string);
char* f_help(void);
void f_pause(void);
void f_quit(void);
void case_checking(char* cmd,char* arg,char* original);
int bash(char* user_input);
int piping(char* arg1[], char* arg2[]);
int pipe_checking(char* orginal);
int piping_ready(void);

//----------------------Global Var------------------------------------->

int user_status = 0; // 0 -> user did not quit 1 -> user quit;
int size = 256;
size_t bufsize = 32;

//----------------------Global Var------------------------------------->

//---------------------- Main ----------------------------------------->
int main(int argc, const char * argv[]) {
          begin();
          return 0;
}
//---------------------- Main ----------------------------------------->

// The parsing function; 
int piping_ready(){
    printf("type the first arg below add a space: \n");
     char *input = f_get_line();
     int counter = 0;
       
       char *arr[10];
       char* token = strtok(input," \t\n");
       //printf("[%s]\n",token);
       arr[0] = token;
       while(token!=NULL){
           counter++;
           token = strtok(NULL, " \t\n");
           //printf("[%s]\n",token);
           arr[counter] = token;
       }
    
    printf("type the second arg below add a space: \n");
       char *input2 = f_get_line();
       int counter1 = 0;
       char *arr1[10];
       char* token1 = strtok(input2," \t\n");
       //printf("[%s]\n",token1);
       arr1[0] = token1;
       while(token1!=NULL){
           counter1++;
           token1 = strtok(NULL, " \t\n");
           //printf("[%s]\n",token1);
           arr1[counter1] = token1;
       }
       
       piping(arr, arr1);

    return 0;
}

// ------------------------------------------piping------------------------------------->
int piping(char* arg1[], char* arg2[]){
    
    int pip[2];
    pipe(pip);
    int child1 = fork();
    if(child1 == 0){
        close(pip[0]);
        dup2(pip[1], 1);
        execvp(arg1[0], arg1);
        printf("fail\n");
        exit(1);
    }
    
    // connected to the pipe 
    // file_output connect to the pipe input ;
    // file_input connect with the pipe output;        
    int child2 = fork();
    if(child2 == 0){
        close(pip[1]);
        dup2(pip[0], 0);
        execvp(arg2[0], arg2);
        printf("fail\n");
        exit(1);
    }
    // close when the file finish
    close(pip[0]);
    close(pip[1]);
    wait(0);
    wait(0);
    
    return 0;
}
// ------------------------------------------piping------------------------------------->

int bash(char *user_input){
// using the fork function              
    int pid = fork();
    //  the child 
    if(pid >= 0){
    
        if (pid == 0) {// child
            // ----------------------new parse------------------------------------->
                int counter = 0;
                int size = 1;
                char *arr[10];
                char* token = strtok(user_input," ");
                arr[0] = token;
                token[strcspn(token," \t\n")] = 0;
                //printf("[%s]\n",token);
                    while(token!=NULL){
                        counter++;
                        token = strtok(NULL," ");

                        if(token == NULL){
                            break;
                        }else{
                            size++;
                            token[strcspn(token,"\n")] = 0;
                            arr[counter] = token;
                        }
                    }
                 
                arr[size] = NULL;
        // ----------------------new parse------------------------------------->
        //printf("arrived\n");
        int fd = open("output.txt", O_CREAT|O_WRONLY|0600);
        dup2(fd, 1);
          //  printf("succes\n");
            
            // execute the command which user type in and not build command
            if(execvp(arr[0], arr) < 0){
                perror("execvp failed");
            }
            
        }else{//parent
            wait(NULL);
            //printf("Finish waiting\n");
        }
    }else{
        printf("Fork fail \n");
    }
         
    
    return 0;
}


// THE BEGIN METHOD
int begin(){
    while(user_status == 0){
        char *input = f_get_line();
        char *cmd = f_parse_cmd(input);
        char *arg = f_parse_arg(input);
        // tracking for my user input 
        //printf("The cmd is [%s]\n",cmd);
        //printf("The arg is [%s]\n",arg);
        case_checking(cmd, arg,input);
     
    }
        return 0;
}


// CHECKING WHICH CASE IT THE INPUT BELONG;
void case_checking(char* cmd,char* arg,char* original){
    int x = 0;
    if(cmd  == NULL ){
       
    }else if((strcmp(cmd, "cd") == 0)){
        x = 2;
    }else if((strcmp(cmd, "clear") == 0)){
        x = 3;
    }else if((strcmp(cmd, "environ") == 0)){
        x = 4;
    }else if((strcmp(cmd, "echo") == 0)){
        x = 5;
    }else if((strcmp(cmd, "help") == 0)){
        x = 6;
    }else if((strcmp(cmd, "pause") == 0)){
        x = 7;
    }else if((strcmp(cmd, "quit") == 0)){
        x = 8;
    }else if (strcmp(cmd, "dir") == 0){
        x = 1;
        
    }else if(strcmp(cmd, "pipe") == 0){
        piping_ready();
    }
    else{
        bash(original);
    }
    
     switch (x) {
            case 1:{
                f_ls(arg);
                break;
            }
            case 2:{
                f_cd(arg);
                    break;
            }
            case 3:{
                f_clear();
                    break;
            }

            case 4:{
                f_environ();
                    break;
            }
            case 5:{
                f_echo(arg);
                    break;
            }
            case 6:{
                f_help();
                    break;
            }
            case 7:{
                f_pause();
                    break;
            }case 8:{
                f_quit();
            }

    }
}




// -------------------------Parsing------------------------------>

char* f_get_line(void){
   char *buffer;
       size_t characters;
        
       buffer = (char *)malloc(sizeof(char)*bufsize);
       if( buffer == NULL)
       {
           perror("Unable to allocate buffer");
           exit(1);
       }

       printf("myshell >");
       characters = getline(&buffer,&bufsize,stdin);
       //printf("%zu characters were read.\n",characters);
       //printf("You typed: '%s'\n",buffer);
    free(buffer);
    return buffer;
}

char * f_parse_cmd(char *words_line){
    char* copy = (char *)malloc(sizeof(char));
    strcpy(copy,words_line);
    char* cmd_string = strtok(copy, " \t\n");
    //cmd_string[strcspn(cmd_string,"\n")] = 0;
    return cmd_string;
}


char * f_parse_arg(char *words_line){
    char* copy = (char *)malloc(sizeof(char));
    strcpy(copy,words_line);
    char* arg_string = strtok(copy, " \t\n");
    arg_string = strtok(NULL, " \t\n");
    return arg_string;
}


// -------------------------Parsing------------------------------>

//THE DIR FUNCTION : type dir will shows the list files 
char* f_ls(char* arg){
    DIR *dir;
    struct dirent *sd;
    char* route = NULL;
    char* place = getcwd(route, 1000);
    
    //printf("%s\n",place);
    //dir = opendir("/Users/chenerzhang");
    dir = opendir(place);
    
    if (dir == NULL)
    {
        printf("Could not open current directory\n" );
        return 0;
    }
    while ((sd = readdir(dir)) != NULL){
           printf("%s\n", sd->d_name);
    }
    closedir(dir);
    return 0;
}

// type the cd will change the direction of the file 
// cd filename

int f_cd(char* arg){
    char path[500];
    strcpy(path,arg);
    char cwd[500];
    if(arg[0] != '/'){
        //printf("success\n");
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/");
        strcat(cwd,path);
        chdir(cwd);
    }else{
        chdir(arg);
    }
    return 0;
}


void f_clear(){
    printf("\033[H\033[2J");
    system("clear");
}

char* f_environ(){
     printf("PATH : %s\n", getenv("PATH"));
    printf("HOME : %s\n", getenv("HOME"));
    printf("ROOT : %s\n", getenv("ROOT"));
    return NULL;
    
    return NULL;
}
char* f_echo(char* string){
    printf("%s\n",string);
    return string;
}
char* f_help(){
    printf("when using the pipe, remember to type extral space in the end command \n");
    printf("[dir] - > ls    [cd] - > cd");
    return NULL;
}
void f_pause(){
    sleep(100);

}
void f_quit(){
    printf("Bye Bye --------");
    exit(1);

}


