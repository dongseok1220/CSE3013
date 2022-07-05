#include "str.h"
#include<string.h>
#include<cstdlib>
#include<iostream>
using namespace std;
Str::Str(int leng)
{
	if (leng < 0) cout << "Error" << endl; // 길이가 음수면 error 출력
	else {
		str = new char[leng+1]; // 길이가 leng인 str생성
		len = leng;
		str[0]='\0'; // 배열크기만 입력받았으므로 첫 문자 지정
	}	
}
Str::~Str()
{
	delete[] str; // 할당된 메모리 해제
}
Str::Str(char *neyong)
{
	len=strlen(neyong); // len에 neyong길이 값
	str = new char [len+1]; // len+1 만큼크기 str 생성
	strcpy(str , neyong); // str함수에 neyong 복사
}
int Str::length(void)
{
	return len; // 배열의 크기 리턴
}
char* Str::contents(void)
{
	return str; //  str 주소 리턴 
}
int Str::compare(char *a)
{
	return strcmp(str, a); // str 과 a 비교해서 값 리턴
}
int Str::compare(Str& a)
{
	return compare(a.contents()); // class a를 인자로받아 a의 contents를  compare 함수에 넘겨줌
}
void Str::operator=(char *a)
{
	delete[] str; // 기존 str 메모리 해제 
	str = new char [strlen(a)+1]; // a의 길이+1크기의 새로운 str  배열 생성/	
	strcpy(str, a); // str에 a 복사
}
void Str::operator=(Str& a)
{
	(*this)=a.contents(); // class a의 contents를 str에 복사
}


