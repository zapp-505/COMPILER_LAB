#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//rules already defined no need of rules input only input of 
//expression to be eparsed needed
char input[100];
int pos=0;
int valid=1;
void E();
void T();
void F();

void E()
{
    T();
    while(input[pos]=='+'){
        pos++;
        T();
    }
}

void T()
{
    F();
    while(input[pos]=='*'){
        pos++;
        F();
    }
}

void F()
{
    if(input[pos]=='(')
    {
        pos++;
        E();
        if(input[pos]!=')'){
            valid=0;
        }
        else{
            pos++;
        }
    }   
    else if(strncmp(input+pos,"id",2)==0){
        pos+=2;
    }
    else{
        valid=0;
    }
}

int main(){
    printf("Enter the expression");
    scanf("%s",input);
    
    E();
//dont forget this part 
    if(valid && input[pos]=='\0'){
        printf("Accepted");
    }
    else{
        printf("Rejected");
    }
    return 0;
}


