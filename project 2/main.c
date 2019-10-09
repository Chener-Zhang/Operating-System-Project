#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


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
void f_environ(void);
void f_echo(char* string);
void f_help(void);
void f_pause(void);
void f_quit(void);
int bash_checking(char* cmd);
void case_checking(char* cmd,char* arg);


//////////////////////////////
int user_status = 0; // 0 -> user did not quit 1 -> user quit;
int is_bash = 0; // whether call the external command 0 -> no bash, 1-> yes bash;

char *current_route  = "/Users/chenerzhang";;
char *new_route;
char *cwd_path = NULL;
int size = 256;

char* lists;
size_t bufsize = 32;
//////////////////////////////


int main(int argc, const char * argv[]) {

    begin();
    return 0;
}



void case_checking(char* cmd,char* arg){
    int x = 0;
    if(strcmp(cmd, "ls") == 0){
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
        printf("error!\n");
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

int begin(){
    
  //  /*
    lists = malloc(sizeof(char)*bufsize);
    char *input = f_get_line();
     char *cmd = f_parse_cmd(input);
     char *arg = f_parse_arg(input);
     bash_checking(arg);
    if(is_bash == 1){
        
    
    }else{
          case_checking(cmd, arg);
         printf("The comand is : [%s] \nThe argument is : [%s] \n",cmd,arg);
    }

       // case_checking(cmd,arg);
//    */
    
    
   /*
    while(user_status == 0){
        lists = malloc(sizeof(char)*1026);
        char *input = f_get_line();
        char *cmd = f_parse_cmd(input);
        char *arg = f_parse_arg(input);
        case_checking(cmd, arg);
        free(lists);
     //   printf("while loop : %s\n",current_route);
    }
    */
    

        return 0;
}

//[<,<<<,|,>,>>>]
int bash_checking(char* arguments){

    char* copy = (char *)malloc(sizeof(char)*bufsize);
    strcpy(copy,arguments);
    
    char* cmd_string = strtok(copy, " ");
        
    while(cmd_string != NULL){
        if(strcmp(cmd_string, ">") == 0){
        //printf("The special is: %s\n",cmd_string);
            return 1;
        }else if(strcmp(cmd_string, ">>") == 0){
        //printf("The special is: %s\n",cmd_string);
            return 1;
        }else if(strcmp(cmd_string, "<") == 0){
        //printf("The special is: %s\n",cmd_string);
            return 1;
        }else if(strcmp(cmd_string, "|") == 0){
        //printf("The special is: %s\n",cmd_string);
            return 1;
        }
        
        cmd_string = strtok(NULL," ");
        
    }
    
    free(copy);

    return 0;
}
  


//how many hours you spend on it?

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
    char* copy = (char *)malloc(sizeof(char)*bufsize);
    strcpy(copy,words_line);
    char* cmd_string = strtok(copy, " ");
    cmd_string[strcspn(cmd_string,"\n")] = 0;
        
    return cmd_string;
}

char * f_parse_arg(char *words_line){
    char* copy = (char *)malloc(sizeof(char)*bufsize);
    strcpy(copy,words_line);
    for (int i = 0; i < strlen(copy)-2; i++) {
        if(copy[i] == ' '){
        i++;
            //printf("%c", copy[i]);
            copy = &copy[i];
            copy[strcspn(copy,"\n")] = 0;
            return copy;
        }
    }
    free(copy);
    return "NULL";
}







char* f_ls(char* arg){

    DIR *dir;
    struct dirent *sd;
        
    
    
    // If there is nothing in arg. arg = NULL
    // Just print what we have;
    if(strcmp(arg, "NULL") == 0){
        dir = opendir(current_route);
        printf("the route is :%s\n",current_route);
                if(dir){
                        while( (sd = readdir(dir)) != NULL){
                        //printf("%s\n",sd->d_name);
                            strcat(lists,sd->d_name);
                            strcat(lists,"\n");
                                       }
                }else{
                    printf("error\n");
                    return 0;
                }
    }
    // There is something
    else{
        char* route_for_ls = NULL;
        route_for_ls = (char *) malloc(strlen(current_route)+strlen(arg)*bufsize);
        strcpy(route_for_ls, current_route);
        strcat(route_for_ls, "/");
        strcat(route_for_ls, arg);
        
        dir = opendir(route_for_ls);
            printf("the route is :%s\n",route_for_ls);
                if(dir){
                    while( (sd = readdir(dir)) != NULL){
                            printf("%s\n",sd->d_name);
                        
                        
                                            }
                        }
        free(route_for_ls);
        }
    printf("%s",lists);
    return lists;
}




int f_cd(char* arg){
    
    if(strcmp(arg, "NULL") == 0){
        //do nothing
    }else{
        new_route = (char *) malloc(strlen(current_route)+strlen(arg)*bufsize);
        strcpy(new_route, current_route);
        strcat(new_route, "/");
        
        strcat(new_route, arg);

        int yes_no = chdir(new_route);
        if(yes_no == 0){
            char *temp = NULL; // tracker
            printf("change success\n");
            //printf("OLD PATH: %s:\n",cwd_path);
            cwd_path = getcwd(temp, sizeof(new_route));
            //printf("NEW PATH: %s:\n",cwd_path);
            current_route = cwd_path;
            
        }else{
            printf("change unsuccess\n");
        }
        free(new_route);
    }
    
    return 0;
}






void f_clear(){
    printf("\033[H\033[2J");
    system("clear");
}

void f_environ(){
    printf("This is environ function\n");
}
void f_echo(char* string){
    printf("%s\n",string);
    
}
void f_help(){
    printf("This is help function\n");
}
void f_pause(){
    printf("This is pause function\n");
}
void f_quit(){
    system("quit");
    printf("This is quit function\n");
}
