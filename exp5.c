#include <stdio.h>
#define MAX 20
int n;
int visited[MAX];
int closure[MAX];
int transition[MAX][MAX];
void dfs(int state){
visited[state] = 1;
closure[state] = 1;
for(int i =0; i<n; i++){
if(transition[state][i] == 6 && visited[i] != 1){
dfs(i);
}
}
}
int main(){
printf("Give num of states: ");
scanf("%d", &n);
printf("Give transition matrix (6 - epsilon, 9 - no connection): \n");
for(int i=0; i<n; i++){
for(int j=0; j<n; j++){
scanf("%d", &transition[i][j]);
}
}
printf("Closure of each state: \n");
for(int i = 0; i<n; i++){
for(int j=0; j< n; j++){
visited[j] = 0;
closure[j] = 0;
}
dfs(i);
printf("Closure for S%d : ", i);
for(int k = 0; k<n; k++){
if(closure[k] == 1){
printf(" S%d ", k);
}
}
printf("\n");
}
return 0;
}