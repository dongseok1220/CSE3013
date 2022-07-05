class Str {
	private :
		char *str; // str 배열의 포인터로 내용을 담고 있다
		int len;	// string의 길이

	public :
		Str(int leng); // leng+1크기의 str 배열 생성
		Str(char *neyong); // neyong의 내용을 복사
		
		~Str(); // 소멸자
		
		int length(void); // len을 리턴
		char *contents(void); // str의 내용 리턴
		int compare(class Str &a); // class a와 str 비교
		int compare(char *a); // 문자열 a와 str비교 
		void operator=(char *a); // = 연산자 재정의
		void operator=(class Str &a); // = 연산자 재정의 
};
