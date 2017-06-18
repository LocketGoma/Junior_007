#include <stdio.h>		// 필요한 header file 추가 가능
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>	//<- 리눅스용
#include "student.h"

// 수정할것
// 1. argv 인풋 확인
// 2. writerecord 부분 코드 수정 (fread 등 fp 포인터 직접 접근 삭제)
// 3. 기타 버그 잡기.



#pragma warning(disable: 4996)	// 지우셈
// 일단, 실행라인을 안쓰는거로 제작하고, 그담에 바꿀것.
 
//
// readPage() 함수는 레코드 파일에서 주어진 pagenum(=0, 1, 2, ...)에 해당하는 page를
// 읽어서 pagebuf가 가리키는 곳에 저장하는 역할을 수행한다. 정상적인 수행을 한 경우
// '1'을, 그렇지 않은 경우는 '0'을 리턴한다.
// getNextRecord() 함수는 readPage()를 통해 읽어온 page에서 record를 차례대로 전달하는
// 일을 한다. 만약 page에서 전달할 record가 없으면 '0'을, 그렇지 않으면 '1'을 리턴한다.
// unpack() 함수는 recordbuf에 저장되어 있는 record에서 각 field 값을 추출하여 학생 객체에 저장하는 일을 한다.
//
int readPage(FILE *fp, char *pagebuf, int pagenum);
int getNextRecord(const char *pagebuf, char *recordbuf);
void unpack(const char *recordbuf, STUDENT *s);

//
// writePage() 함수는 프로그램 상의 data page(pagebuf)를 레코드 파일의 주어진 pagenum(=0, 1, 2, ...)의 위치에 
// 저장하는 역할을 수행한다. 정상적인 수행을 한 경우 '1'을, 그렇지 않은 경우는 '0'을 리턴한다.
//
//
// 레코드 파일에 새로운 레코드를 추가한다. 이때 'right size'를 가지는 최초의 삭제 레코드를 찾아서
// 그 자리에 새로운 레코드를 추가한다. 만약 right size가 존재하지 않는 경우 파일의 끝에 append한다.
// 함수가 성공적으로 수행되면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int writePage(FILE *fp, const char *pagebuf, int pagenum);

//
// 주어진 학생 객체를 레코드 파일에 저장할 때 사용되며, 이때 파일을 구성하는 
// page 중에서 맨마지막 page에 해당 record를 저장한다. 만약, 맨마지막 page에 새로운
// 레코드를 저장할 공간이 부족하면 새로운 page를 하나 할당해서 여기에 저장한다.
// writeRecord() 함수의 내부에서는 pack() 함수를 호출하며, pack() 함수는 주어진 학생 객체를 실제로 
// 새로운 record가 page에 저장되었을 때의 형태를 recordbuf에 저장하는 역할을 수행한다.
// writeRecord()는 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int writeRecord(FILE *fp, const STUDENT *s);
void pack(char *recordbuf, const STUDENT *s);

//
// 레코드 파일에서 field의 키값(keyval)을 갖는 레코드를 검색하고 그 결과를 출력한다.
// 검색된 레코드를 출력할 때 반드시 printRecord() 함수를 사용한다.
//
void search(FILE *fp, FIELD f, char *keyval);
void printRecord(const STUDENT *s, int n);

//
// 레코드 파일에서 주어진 '학번' 키값을 만족하는 레코드를 삭제한다. 조건을 만족하는 레코드를
// 화면에 출력하며, 이때 아래의 printDeletedRecord()를 반드시 사용한다. 참고로, 조건을 만족하는
// 레코드를 찾을 때, 삭제된 레코드는 검색 대상 레코드에서 제외되어야 한다(판별여부는 delete mark '*'를 이용).
// 마찬가지로, search() 함수도 이를 검사하는 코드가 포함되어야 한다.
// 함수가 성공적으로 수행되면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int deleteRecord(FILE *fp, FIELD f, char *keyval);
void printDeletedRecord(const STUDENT *s, int n);


//
// 레코드의 필드명을 enum FIELD 타입의 값으로 변환시켜 준다.
// 예를 들면, 사용자가 수행한 명령어의 인자로 "NAME"이라는 필드명을 사용하였다면 
// 프로그램 내부에서 이를 NAME(=1)으로 변환할 필요성이 있으며, 이때 이 함수를 이용한다.
//
FIELD getField(char *fieldname);

int init(FILE *fp);					//이니셜라이즈
int headcontrol(FILE *fp);			//헤드페이지 +1
int deletehead(FILE *fp, int pagenumber, int recordnumber);	// 삭제시 헤드 수정

// 추가 구현용
// 1. blanknext - 다음 빈 항목 리턴, 다음에도 빈곳이 있다면 1, 없으면 (지금이 -1,-1 이면) 0 리턴
// 2. blanksize - 현재 빈 항목의 사이즈가 몇인지.
// 3. blankaddrchnage - A -> B -> C 에서, B가 채워질때 A->C로 바꿔주는 역할.
int blanknext(FILE *fp, int addr[2]);		// addr : 다음 주소 리턴, pagenum/recordnum : 입력
int blanksize(FILE *fp,int pagenum, int recordnum);					// 사이즈 리턴.
int blankaddrchange(FILE *fp,int *before, int *next);				// before : 현재 포인트를 리턴한 값, next : 현재 값, 정상 수행시 리턴 1, 아니면 리턴 0


// 1. write 구현			
// 2. read 구현
// 3. 검색 구현
// 4. 버그 찾기
// 5. 삭제 구현				
// 6. 삽입 수정			<--

int main(int argc, char *argv[])
{
	FILE *fp;						// 모든 file processing operation은 C library를 사용할 것.

	char file[30];
	if (access("students.dat", 0) != 0) {
		printf("미존재");
		fp = fopen("students.dat", "a+b");
		init(fp);			//실행시키기
//		char *buf;
//		unsigned short *pick;
//		buf = (char *)malloc(sizeof(char)*PAGE_SIZE);
//		pick = (unsigned short *)buf;
//		pick[0] = 1;
//		fwrite(buf, PAGE_SIZE, 1, fp);
	}
	else {
		printf("존재");
		fp = fopen("students.dat", "r+b");
	}
//	fp = fopen("students.dat", "a+b");

	STUDENT temp;
	*temp.id = (char *)malloc(sizeof(char) * 9);
	*temp.name = (char *)malloc(sizeof(char) * 21);
	*temp.dept = (char *)malloc(sizeof(char) * 21);
	*temp.year = (char *)malloc(sizeof(char) * 2);
	*temp.addr = (char *)malloc(sizeof(char) * 31);
	*temp.phone = (char *)malloc(sizeof(char) * 16);
	*temp.email = (char *)malloc(sizeof(char) * 27);

	char *field;
	char *find;
	field = (char *)malloc(sizeof(char) * 5);
	find = (char *)malloc(sizeof(char) * 31);

//	temp = malloc(sizeof(STUDENT));
//	memset(&temp, 0, sizeof(STUDENT));

	//VS 테스트시
	strcpy(temp.id,"20101241");
	strcpy(temp.name,"gildong");
	strcpy(temp.dept,"computer");
	strcpy(temp.year,"4");
	strcpy(temp.addr,"dongjak");
	strcpy(temp.phone,"010-585-1234");
	strcpy(temp.email,"goodhong@ssu.ac.kr");

	writeRecord(fp, &temp);

//	search(fp, 0, "20101236");

//	deleteRecord(fp, 0, "20101237");				// 삭제 파트 다시 찾아볼것.

	int addr[2] = { -1,-1 };
	int next[2] = { 1,1 };
//	blanknext(fp, addr);
//	printf("%d,%d\n", addr[0], addr[1]);
//	blankaddrchange(fp, addr, next);

/*	
	//argument 사용시.
	if (argv[1][1] == 'i') {
		printf("input\n");
		strcpy(temp.id, argv[3]);
		strcpy(temp.name, argv[4]);
		strcpy(temp.dept, argv[5]);
		strcpy(temp.year, argv[6]);
		strcpy(temp.addr, argv[7]);
		strcpy(temp.phone, argv[8]);
		strcpy(temp.email, argv[9]);
			writeRecord(fp,&temp);
	}
	else if (argv[1][1] == 's') {
		//파싱 프로세스
		printf("search\n");
		int i = 0;
		int j = 0;
		for (; i < 36; i++) {
			if (argv[3][i] != 61)
				field[i] = argv[3][i];
			else
				break;
		}
		i++;
		while (argv[3][i] != NULL) {
			find[j] = argv[3][i];
			j++;
			i++;
		}
		//파싱 끝

		printf("field : %s, find : %s\n", field, find);
		printf("%d",getField(field));				// 잘됨
		search(fp, getField(field), find);
	}

*/
	
	return 1;
}

int init(FILE *fp) {			//이니셜라이즈
	if (fp == NULL)
		return 0;
	char temp[PAGE_SIZE] = "";
	unsigned short *pick;
	pick = (unsigned short*)temp;
	pick[0] = 1;
	pick[1] = -1;				//last delete Pages
	pick[2] = -1;				//last delete Record
	writePage(fp, temp, 0);

	return 1;
}
int headcontrol(FILE *fp) {
	int current;
	char temp[PAGE_SIZE] = "";
	unsigned short *pick = 0;
	if (fp == NULL)
		return 0;
	printf("[%d]\n", ftell(fp));

	current = ftell(fp);				//현재 위치 읽고
	fseek(fp, 0, SEEK_SET);				//헤드페이지 보내고
	readPage(fp, temp, 0);

	pick = (unsigned short*)temp;		//헤드정보 읽고
	printf("pick : %d\n", pick[0]);		//헤드 정보 출력좀..
	pick[0]++;							//헤드값 +1
	printf("pick after : %d\n", pick[0]);		//after
	writePage(fp, temp, 0);				//헤드 수정하고
	fseek(fp, current, SEEK_SET);		//다시 원래 위치로 보내기

	return 1;
}
int deletehead(FILE *fp, int pagenumber, int recordnumber) {	//삭제시 헤드 수정용.

}
int blanknext(FILE *fp, int addr[2]) {		// addr : 입력 및 다음 주소 리턴, -1, -1 투입시 헤더로 감.
	char *pagebuf;							// 0 : 에러 혹은 더이상 없음
	char *header;							// 1 : 다음이 있음.
	char *recordbuf;
	unsigned short *headpick;		// 0 : 총 페이지 수 / 1 : 마지막 삭제 페이지 번호 / 2 : 마지막 삭제 레코드 번호
	unsigned short *pick;
	int pointsize = HEADER_SIZE;
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	recordbuf = (char *)malloc(sizeof(char)*MAX_RECORD_SIZE);
	header = (char *)malloc(sizeof(char)*PAGE_SIZE);


	if (addr[0] == -1)			//됨
		if (addr[1] == -1) {
			readPage(fp, header, 0);
			headpick = (unsigned short *)header;
			if (headpick[1] == -1) {
				return 0;
			}
			addr[0] = headpick[1];
			addr[1] = headpick[2];
			return 1;
		}
		else
			return 0;
	
	readPage(fp, pagebuf, addr[0]);
	pick = (unsigned short *)pagebuf;
	//while (getnextrecord(pagebuf, recordbuf)) {
		printf("size : %d \n", pick[2 + addr[1] * 2]);			//[2][3] - 2 : offset , -3 : size
		pointsize+=pick[2 + addr[1] * 2];
	//}
		printf("pointsize : %d\n", pointsize);
		printf("%d,%d", pick[pointsize / 2+1], pick[pointsize / 2 + 2]);

		if (pick[pointsize / 2 + 1] == 65535) {
			addr[0] = -1;
			addr[1] = -1;
			return 0;
		}
		addr[0] = pick[pointsize / 2 + 1];
		addr[1] = pick[pointsize / 2 + 2];
		

		printf("size : %d <-> size : %d\n", pick[2 + addr[1] * 2+1], blanksize(fp, addr[0], addr[1]));

		return 1;
	
}
int blanksize(FILE *fp, int pagenum, int recordnum) {					// 사이즈 리턴.
	char *pagebuf;
	unsigned short *pick;
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	readPage(fp, pagebuf, pagenum);

	pick = (unsigned short *)pagebuf;
	if (pick[2 + recordnum * 2 + 1] > MAX_RECORD_SIZE)
		return -1;		//비정상

	return pick[2 + recordnum * 2 +1];

}
int blankaddrchange(FILE *fp,int *bef, int *nex) {				// before : 현재 포인트를 리턴한 값, next : 현재 값, 정상 수행시 리턴 1, 아니면 리턴 0
	int before[2];
	int now[2];													// 여기 먹통
	int next[2];
	unsigned short * pick;
	char *pagebuf;
	int pointsize=HEADER_SIZE;
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);

	before[0] = bef[0];
	before[1] = bef[1];
	now[0] = nex[0];
	now[1] = nex[1];
	next[0] = nex[0];
	next[1] = nex[1];
	printf("before : %d,%d\nnow : %d,%d\nnext:%d,%d\n--1--\n", before[0], before[1], now[0], now[1], next[0], next[1]);

	blanknext(fp, next);		//3가지 준비 끝
								//Before > now > next ==> before -> next
								//더이상 남지 않을때 (before 인풋이 -1일때)
	//테스트//
	printf("before : %d,%d\nnow : %d,%d\nnext:%d,%d\n--2--\n", before[0], before[1], now[0], now[1], next[0], next[1]);

	if (before[0] == -1) {				//이전이 시작점인 경우
		readPage(fp, pagebuf, 0);
		pick = (unsigned short *)pagebuf;
		pick[1] = next[0];
		pick[2] = next[1];
		writePage(fp, pagebuf, 0);
	}	
	else if (next[0] == -1) {			//다음이 끝인경우
		readPage(fp, pagebuf, 0);
		pick = (unsigned short *)pagebuf;
		pick[1] = -1;
		pick[2] = -1;
		writePage(fp, pagebuf, 0);
	}
	else {
		readPage(fp, pagebuf, before[0]);
		pick = (unsigned short *)pagebuf;
		pointsize += pick[2 + before[1] * 2];
		printf("pickker : %d,%d\n", pick[pointsize / 2 + 1], pick[pointsize / 2 + 2]);
		pick[pointsize / 2 + 1] = next[0];
		pick[pointsize / 2 + 2] = next[1];
		writePage(fp, pagebuf, before[0]);
	}
	printf("before : %d,%d\nnow : %d,%d\nnext:%d,%d\n--3--\n", before[0], before[1], now[0], now[1], next[0], next[1]);
	printf("pickker : %d,%d\n", pick[pointsize / 2 + 1], pick[pointsize / 2 + 2]);
	return 1;

}
//void parse_Record(); 
int readPage(FILE *fp, char *pagebuf, int pagenum){
	if (fp != NULL) {
		fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);			//pagenum 만큼 이동하고
		fread(pagebuf, sizeof(char), PAGE_SIZE, fp);	//pagebuf에 페이지 통째로 로드
		return 1;
	}
	else
		return 0;
}
int getNextRecord(const char *pagebuf, char *recordbuf){			// 여기서부터 다시 짜세요.
	static int recordnum = 0;	//몇번 레코드 읽어야하는가
	static int pagepoint = 0;	//페이지버퍼에서 몇번째인가.
	unsigned short *pick;
	pick = (unsigned short *)pagebuf;

	int ret = 0;				//0~7
	int rotate=0;				//몇번레코드 읽고 있는가

	if(recordnum==0)
	for (int i = 0; i < MAX_RECORD_SIZE; i++) {
		recordbuf[i] = pagebuf[100 + i];
//		printf("[i:%d]", i);
		if (pagebuf[HEADER_SIZE + i] == 35) {
			ret++;
//			printf("%d번 \n", ret);
		}
		if (ret == 7) {
			recordbuf[i+1] = '\0';
			rotate++;
			ret = 0;
			break;
		}
	}
	else if (recordnum+1 > pick[0]) {
		return 0;
	}
	else
		for (int i = 0; i < PAGE_SIZE - HEADER_SIZE; i++) {
			if (pagebuf[HEADER_SIZE + i] == 35) {
				ret++;
			}
			if (ret == 7) {
				rotate++;
				ret = 0;
				//break;
			}
			else if (rotate == recordnum) {
				for (int j = 0; j < MAX_RECORD_SIZE; j++) {
					printf("[i:%d]", i);
					recordbuf[j] = pagebuf[HEADER_SIZE + i];
					i++;
					if (pagebuf[HEADER_SIZE + i] == 35)
						ret++;
					if (ret == 7) {	
						recordbuf[j + 1] = '#';
						recordbuf[j + 2] = '\0';
						rotate=-1;
						break;
					}
				}
				break;
			}
			else if (rotate == -1)
				break;
		}
	
	printf("레코드 번호 : %d, 로테이트 : %d\n", recordnum, rotate);
	recordnum++;
	return 1;

}
//언팩. 다 짰나? <- 아마도?
void unpack(const char *recordbuf, STUDENT *s){
	char temp[31]="";
	int size;
	int count;
	int j;
	count = 0;
	j=0;
	size = strlen(recordbuf);
//	temp=(char *)malloc(sizeof(char)*31); // 최대치
//	printf("size : %d\n", size);
	

	for (int i=0;i<size;i++){		//or MAX_RECORD_SIZE
//		printf("i = %d", i);
		if (recordbuf[i]=='#'){			//'#' 만나면
//			temp[j + 1] = '\0';
//			printf("\n[%s]/lenght:%d", temp,strlen(temp));
			if (count==0){
				strcpy(s->id, temp);
				//학번 넣고
			}
			else if(count==1){
				strcpy(s->name, temp);
				//이름 넣고
			}
			else if(count==2){
				strcpy(s->dept, temp);
				//학과 넣고
			}
			else if(count==3){
				strcpy(s->year, temp);
				//주소 넣고
			}
			else if(count==4){
				strcpy(s->addr, temp);
				//주소 넣고
			} //7이네?			
			else if(count==5){
				strcpy(s->phone, temp);
				//폰번호 넣고
			}
			else if(count==6){
				strcpy(s->email, temp);
				//이메일 주소 넣고
			}
			else if (count == 7) {
				break;
			}
			else
				printf("언팩 에러 발생\n");

			temp[0] = "\0";
//			temp = (char *)malloc(sizeof(char) * 31);
//			printf("[:%s]\n", temp);
			j=0;			//뭐임
			count++;
		}
		else{					//'#' 안만나면
			temp[j]=recordbuf[i];	//데이터 산입
//			printf("(%c)\n", temp[j]);
			j++;
		}	
			
	}
	
}	

int writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	printf("파일포인터 위치4 : %d\n", ftell(fp));
	if (fwrite(pagebuf, PAGE_SIZE, 1, fp) == PAGE_SIZE) {
		return 1;
	}
	else
		return 0;
}

//1 페이지 = [헤더][레코드][레코드][레코드]
//헤더 = [레코드 개수][레코드사이즈][레코드offset][레코드길이][레코드offset][레코드길이]... = 100byte
//레코드 = [7개 데이터]*n = 924byte
//여기서 나중에 getnextRecord의 데이터를 갖고옴.

//그러니까. io 단위가 page란 거지?
int writeRecord(FILE *fp, const STUDENT *s){
	char *recordbuf;			//레코드버퍼
	char *pagebuf;				//리턴
	int recordsize;
	int closet;					//꽉 찼었는지 테스트.
	int start;					//파일포인터 비교, start = ftell(fp) 비교하여 일치하면 => 현재 페이지가 가득찼고, 다음이 없음.
	int stock;					//페이지 수.
	unsigned short *pick;		// picker
	HEADSIZE head;				//페이지 헤더 읽는 녀석. 
	memset(&head, 0, sizeof(HEADSIZE));
	recordbuf= (char *)malloc(sizeof(char) * MAX_RECORD_SIZE);
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	memset(pagebuf, 0, sizeof(char)*PAGE_SIZE);

	//추가//
	int addr[2] = { -1,-1 };
	int before[2] = { -1,-1 };
	int bsize=0;

	pack(recordbuf, s);
	printf("%s\n", recordbuf);
	recordsize=strlen(recordbuf);


	if (fp == NULL) {
		printf("에러");
	}
	printf("%d\n", strlen(recordbuf));

//	printf("%d\n", recordsize);
	fseek(fp, PAGE_SIZE, SEEK_SET);		//헤더 페이지 점프

	start = 0;
	closet = 0;
	stock = 1;				//1로 바꾸기
	
	printf("파일포인터 위치1 : %d\n", ftell(fp));
	while (blanknext(fp, addr)) {
		bsize=blanksize(fp, addr[0], addr[1]);
		if (bsize >= recordsize) {
			break;
		}
		before[0] = addr[0];
		before[1] = addr[1];
	}										//<<-----------------------

	if (bsize >= recordsize) {
		readPage(fp, pagebuf, addr[0]);
		blankaddrchange(fp, before, addr);
		pick = (unsigned short *)pagebuf;
		//pick[2+addr[1]*2]=;

		for (int i = 0; i < strlen(recordbuf); i++) {
			printf("현 위치 : %d\n", HEADER_SIZE + (int)addr[1] - recordsize + i);
			pagebuf[HEADER_SIZE + pick[2+addr[1]*2] + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
		}

		stock = addr[0];
	}
	else {
		fseek(fp, PAGE_SIZE, SEEK_SET);
		while (fp != EOF) {
			//		headcontrol(fp); // 테스트 구문
			fread(pagebuf, PAGE_SIZE, 1, fp);
			
			pick = (unsigned short *)pagebuf;
			head.records = pick[0];
			head.size = pick[1];
			//		printf("fread 리턴 : %d\n",fread(&head, PAGE_SIZE, 1, fp));
			printf("%d:%d\n", head.records, head.size);
			printf("파일포인터 위치2 : %d\n", ftell(fp));
			if (ftell(fp) != start)
				closet = 0;

			if (head.records == 0) {	// 빈공간 도달시 => 새로 만들어줘야됨			<- 다시 생각좀.
				head.records = 1;
				head.size = recordsize;


				printf("케이스 1\n");
				break;
			}
			else if (closet == 1) {
				printf("케이스 4\n");
				headcontrol(fp);			//되려나..
				fclose(fp);
				memset(pagebuf, 0, sizeof(char)*PAGE_SIZE);
				head.records = 1;
				head.size = recordsize;

				fp = fopen("students.dat", "a+b");

				closet = 0;
				break;
			}
			else if (head.records == 24) {		// 최대 레코드 수 도달		==> 다음 페이지 로드.
				printf("케이스 3-1\n");
				start = ftell(fp);
				closet = 1;
				stock++;
				//	fseek(fp, PAGE_SIZE-4, SEEK_CUR);
			}
			else if ((int)head.size + recordsize > PAGE_SIZE - HEADER_SIZE) {
				printf("케이스 3-2\n");
				start = ftell(fp);
				closet = 1;
				stock++;
				//fseek(fp, PAGE_SIZE, SEEK_CUR);
			}
			else											//데이터가 들어가있긴한데 쓸순 있음
			{												//= 빠른 로드
	//			fseek(fp, -PAGE_SIZE, SEEK_CUR);
	//			fread(pagebuf, PAGE_SIZE, 1, fp);
				fseek(fp, stock, SEEK_SET);
				head.records++;
				head.size += strlen(recordbuf);
				printf("케이스2\n");
				break;
			}
		}
		pick = (unsigned short *)pagebuf;
		pick[0] = head.records;
		pick[1] = head.size;

		if (head.records != 1) {
			pick[2 * head.records] = pick[2 * (head.records - 1)] + pick[2 * (head.records - 1) + 1];
			pick[2 * head.records + 1] = strlen(recordbuf);
		}
		else {
			pick[2] = 0;
			pick[3] = strlen(recordbuf);
		}

		for (int i = 0; i < strlen(recordbuf); i++) {
			printf("현 위치 : %d\n", HEADER_SIZE + (int)pick[1] - recordsize + i);
			pagebuf[HEADER_SIZE + (int)pick[1] - recordsize + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
		}
	}
	//pagebuf[0] = head.records;
//	printf("head::%d,size:%d,[1]offset:%d,size:%d\n", pick[0],pick[1],pick[2],pick[3]);

//	printf("%s\n", pagebuf);

//	printf("head = %d , %d \n", head.records, head.size);
//	fread(pagebuf, PAGE_SIZE, 1, fp);
//	printf("파일포인터 위치3 : %d\n", ftell(fp));

	//pagebuf[HEADER_SIZE] = *recordbuf;

	

	printf("결과:%d",writePage(fp, pagebuf, stock));

	printf("");
}


void pack(char *recordbuf, const STUDENT *s){
	//http://mwultong.blogspot.com/2006/10/c-strcat.html
	sprintf(recordbuf,"%s#%s#%s#%s#%s#%s#%s#",s->id,s->name,s->dept,s->year,s->addr,s->phone,s->email);
	
}
void search(FILE *fp, FIELD f, char *keyval){
	int pagenum;
	char *recordbuf;	//레코드 저장
	char *pagebuf;		//페이지 읽기
	char *header;		//헤더 읽기
	int count=0;
	STUDENT *temp;
	STUDENT result[100];//100...
	unsigned short *pick;
	recordbuf = (char *)malloc(sizeof(char) * MAX_RECORD_SIZE);
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	header = (char *)malloc(sizeof(char)*PAGE_SIZE);
	temp = (STUDENT *)malloc(sizeof(STUDENT));
	
	readPage(fp, header, 0);
	pick = (unsigned short *)header;
	pagenum = pick[0];

	/*추가*/
	unsigned short * delpick;
	int recordsize = HEADER_SIZE;
	int recordnum = -1;

	for (int i=1;i<pagenum+1;i++){
		readPage(fp, pagebuf, i);
		while (getNextRecord(pagebuf, recordbuf)) {
			recordnum++;
			delpick= (unsigned short *) recordbuf;
			unpack(recordbuf, temp);
			if (strlen(recordbuf) == 1) {													//'*'		삭제된 레코드면 해당 헤더로 가서 사이즈 읽어서 오기.
				printf("레코드 원래 사이즈 : %d\n", pick[2 + recordnum * 2 + 1]);		//2 = 레코드 헤드 정보 + i * 2 = i번째 레코드 정보로 점프 + '+1' 데이터 사이즈 체크
				recordsize += ((pick[2 + recordnum * 2 + 1]) - 1);							//레코드 정보 :: [offset][size]
			}
			else 
				printf("파싱결과:%s\n", temp->id);


			switch (f) {
				case 0: {		//id
					if (!strcmp(temp->id, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 1: {		//name
					if (!strcmp(temp->name, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 2: {		//dept
					if (!strcmp(temp->dept, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 3: {		//year
					if (!strcmp(temp->year, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 4: {		//addr
					if (!strcmp(temp->addr, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 5: {		//phone
					if (!strcmp(temp->phone, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				case 6: {		//email
					if (!strcmp(temp->email, keyval)) {
						result[count++] = *temp;
					}
					break;
				}
				default: {
					break;
				}
			}
			recordsize += strlen(recordbuf);
			memset(recordbuf, 0, MAX_RECORD_SIZE);
		}

	}
	printRecord(result, count);

}

int deleteRecord(FILE *fp, FIELD f, char *keyval) {			//search 기반. 구현 끝
	int totalpage;
	char *recordbuf;	//레코드 저장
	char *pagebuf;		//페이지 읽기
	char *header;		//헤더 읽기
	int count = 0;
	STUDENT *temp;
	STUDENT result[100];//100...
	unsigned short *headpick;		// 0 : 총 페이지 수 / 1 : 마지막 삭제 페이지 번호 / 2 : 마지막 삭제 레코드 번호
	unsigned short *pick;
//	unsigned short *recordpick;
	int pagenum=0;
	int recordnum=-1;
	int recordsize = HEADER_SIZE;
	recordbuf = (char *)malloc(sizeof(char) * MAX_RECORD_SIZE);
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	header = (char *)malloc(sizeof(char)*PAGE_SIZE);
	temp = (STUDENT *)malloc(sizeof(STUDENT));

	readPage(fp, header, 0);
	headpick = (unsigned short *)header;
	totalpage = headpick[0];
	if (f != 0){	//id
		printf("잘못된 field입니다\n");
		return 0;
	}

	for (int i = 1; i<totalpage + 1; i++) {
		readPage(fp, pagebuf, i);
		pick = (unsigned short *)pagebuf;
		while (getNextRecord(pagebuf, recordbuf)) {
			//recordpick = (unsigned short *)recordbuf;
			recordnum++;		//0부터 시작
			printf("읽은 레코드 : %s, 길이 : %d\n", recordbuf, strlen(recordbuf));
			unpack(recordbuf, temp);
			if (strlen(recordbuf)==1) {													//'*'		삭제된 레코드면 해당 헤더로 가서 사이즈 읽어서 오기.
				printf("레코드 원래 사이즈 : %d\n", pick[2 + recordnum * 2 + 1]);		//2 = 레코드 헤드 정보 + i * 2 = i번째 레코드 정보로 점프 + '+1' 데이터 사이즈 체크
				recordsize += ((pick[2+recordnum*2+1]) - 1);							//레코드 정보 :: [offset][size]
			}																			//근데 이거 쓰면 금방 하겠는뎅. 추가도. 귀찮고 시험있으니 다음에.
			else
			printf("파싱결과:%s\n", temp->id);
				if (!strcmp(temp->id, keyval)) {		//요기서 삭제
					printf("일치");
					printf("레코드 사이즈 : %d\n", recordsize);
					printDeletedRecord(temp, recordsize);
					if (headpick[1] == -1) {
						headpick[1] = i;
						headpick[2] = recordnum;

						pick[recordsize / 2] = '*';			//삭제 레코드 맨 처음 *
						pick[(recordsize / 2) + 1] = -1;
						pick[(recordsize / 2) + 2] = -1;
					}
					else {
						pick[recordsize / 2] = '*';			//삭제 레코드 맨 처음 *
						pick[(recordsize / 2) + 1] = headpick[1];
						pick[(recordsize / 2) + 2] = headpick[2];

						headpick[1] = i;
						headpick[2] = recordnum;
					}
					writePage(fp, header, 0);				//헤더 쓰기
					writePage(fp, pagebuf, i);				//몸체 쓰기

					return 1;
				}
				else {
					printf("불일치\n");
				}
			recordsize += strlen(recordbuf);
			memset(recordbuf, 0, MAX_RECORD_SIZE);

		}

	}
	//printRecord(result, count);
	printf("삭제대상이 존재하지 않습니다\n");
}
void printDeletedRecord(const STUDENT *s, int n) {
	printf("Delete Recordsize = %d\n",n);
	printf("%s#%s#%s#%s#%s#%s#%s\n", s->id, s->name, s->dept, s->year, s->addr, s->phone, s->email);
	
}

void printRecord(const STUDENT *s, int n)
{
	int i;

	printf("NumOfRecords = %d\n", n);

	for (i = 0; i< n; i++)
	{
		printf("%s#%s#%s#%s#%s#%s#%s\n", s[i].id, s[i].name, s[i].dept, s[i].year, s[i].addr, s[i].phone, s[i].email);
	}
}

//필드 리턴. 되는질 모르겠음;
FIELD getField(char *fieldname) {		// FIELD 리턴입니당.
	if (strcmp(fieldname, "ID") == 0)
		return ID;
	else if (strcmp(fieldname, "NAME") == 0)
		return NAME;
	else if (strcmp(fieldname, "DEPT") == 0)
		return DEPT;
	else if (strcmp(fieldname, "YEAR") == 0)
		return YEAR;
	else if (strcmp(fieldname, "ADDR") == 0)
		return ADDR;
	else if (strcmp(fieldname, "PHONE") == 0)
		return PHONE;
	else if (strcmp(fieldname, "EMAIL") == 0)
		return EMAIL;
	//else 
	//	return NULL;
}
