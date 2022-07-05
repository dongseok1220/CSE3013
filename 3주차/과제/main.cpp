#include "str.h"
#include<iostream>
using namespace std;

int main()
{
	Str a("I'm a girl"); // 객체 a에 내용 할당
	cout << a.contents() << endl; // a의 내용출력
	a="I'm a boy\n"; // a에 새롭게 정의한 =연산자를 통해 문자열 할당
	cout << a.contents(); // a의 내용 출력
	cout << a.compare("I'm a a") << endl; // a와 문자열 비교

	return 0;
}


