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
int bash_checking(char* cmd);
void case_checking(char* cmd,char* arg,char* original);
void RD_function(char* line);
int bash(char* user_input);

// ----------------------Global Var------------------------------------->

int user_status = 0; // 0 -> user did not quit 1 -> user quit;
int size = 256;
size_t bufsize = 32;
// ----------------------Global Var------------------------------------->


int main(int argc, const char * argv[]) {
        begin();
        return 0;
}

// ----------------------working------------------------------------->

int bash(char *user_input){
    //execvp(arr[0], arr);
    
    int pid = fork();
    
    if(pid >= 0){
    
        if (pid == 0) {// child
            // ----------------------new parse------------------------------------->
                int counter = 0;
                int size = 1;
                char *arr[10];
                char* token = strtok(user_input," ");
                arr[0] = token;
                token[strcspn(token," \t\n")] = 0;
                
                printf("[%s]\n",token);
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
            printf("arrived\n");
            if(execvp(arr[0], arr) < 0){
                perror("execvp failed");
            }
            
        }else{//parent
            
            wait(NULL);
            printf("Finish waiting\n");
        }
    }else{
        printf("Fork fail \n");
    }
         
    
    return 0;
}


// ----------------------working------------------------------------->
// THE BEGIN METHOD
int begin(){

    while(user_status == 0){

        char *input = f_get_line();
        char *cmd = f_parse_cmd(input);
        char *arg = f_parse_arg(input);
        printf("The cmd is [%s]\n",cmd);
        printf("The arg is [%s]\n",arg);
        case_checking(cmd, arg,input);
     
    }
        return 0;
}

// bash function
void RD_function(char* line){
    
    int counter = 0;
    int size = 1;
    char *arr[10];
    char* token = strtok(line," ");
    arr[0] = token;
    token[strcspn(token,"\n")] = 0;
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
//   int fd_file = open("output.txt", O_CREAT|O_APPEND|O_WRONLY);
//   close(1);
//   dup2(fd_file,1);
    int child_process_id = fork();
    if (child_process_id >= 0){
        if(child_process_id == 0){// child
            execvp("ls",arr);
        }else{// parent
            wait(0);
        }
    }else{
        printf("faril\n");
    }
    //close(fd_file);
     
}

// CHECKING WHICH CASE IT THE INPUT BELONG;
void case_checking(char* cmd,char* arg,char* original){
    int x = 0;
    if(strcmp(cmd, "dir") == 0){
        x = 1;
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
    }else{
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
                f_quit();
                    break;
            }
    }
}


// ----------------------------------------------------------------------->

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
    char* cmd_string = strtok(copy, " ");
    cmd_string[strcspn(cmd_string,"\n")] = 0;
    return cmd_string;
}


char * f_parse_arg(char *words_line){
    char* copy = (char *)malloc(sizeof(char));
    strcpy(copy,words_line);
    char* arg_string = strtok(copy, " \t\n");
    arg_string = strtok(NULL, " \t\n");
    return arg_string;
}


// ----------------------------------------------------------------------->

//THE DIR FUNCTION
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


int f_cd(char* arg){
    char path[500];
    strcpy(path,arg);
    char cwd[500];
    if(arg[0] != '/'){
        printf("yes\n");
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
    
    
    return NULL;
}
char* f_echo(char* string){
    printf("%s\n",string);
    return string;
}
char* f_help(){
    return NULL;
}
void f_pause(){
    //sleep(100);
    printf("This is pause function\n");
}
void f_quit(){
    system("quit");
    printf("This is quit function\n");
}

