

#include <stdio.h>

int digit[10] = {0, };

void plus_digit(int n){
	while (n > 0){
		digit[n % 10]++;
		n /= 10;
	}
	return;
}

int main(){
	int N, d;
	scanf("%d", &N);
	for (int n = 1; n <= N; n++){
		plus_digit(n);
	}
	for (int i=0;i<=9;i++)
	printf("%d ",digit[i]);

}
