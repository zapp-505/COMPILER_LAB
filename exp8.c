#include<stdio.h>
#include<string.h>
#include<ctype.h>

char table[4][4] = {
    {'>', '<', '<', '>'}, // +
    {'>', '>', '<', '>'}, // *
    {'>', '>', 'e', '>'}, // i
    {'<', '<', '<', 'e'}  // $
};

int get_index(char c){
    if(c=='+') return 0;
    if(c=='*') return 1;
    if(c=='i') return 2;
    if(c=='$') return 3;
    return -1;
}

int main(){
    int top=0,i=0,k;
    char stack[30],input[30];
//remember
    stack[top]='$';

    printf("Enter input end with $");
    scanf("%s",&input);
    printf("STACK IPMUT ACTIOMN \n");
    while(1){
        for(k=0;k<=top;k++)
            {
                printf("%c",stack[k]);
            }

        for(k=i;input[k]!='\0';k++)
            {
                printf("%c",input[k]);
            }
//remember
        char row_sym = stack[top];
        char col_sym = input[i];

        if(row_sym =='$' && col_sym == '$')
        {
            printf("Accepted\n");
            break;
        }
        int r= get_index(stack[top]);
        int l= get_index(input[i]);
        
        if (r==-1 || l==-1){
            printf("Error");
            break;
        }

        if(table[r][l]=='<'){
            stack[++top]=input[i];
            i++;
            printf("SHIFT %c\n",col_sym);
        }

        else if(table[r][l]=='>'){
            char popped=stack[top];
            if(popped=='i' || popped == '+' || popped == '*')
            {    
                top--;
                printf("Reduce %c\n",popped);
            }
            else
            {
                printf("syntax error during reduction");
                break;
            }
        }
        else
        {
            printf("Syntax Error\n");
            break;
        }





        
    }
    return 0;
    
}