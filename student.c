#include <stdio.h>		// 필요한 header file 추가 가능
#include <stdlib.h>
#include <io.h>
#include <string.h>
//#include <unistd.h>	//<- 리눅스용
#include "student.h"


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
// 레코드의 필드명을 enum FIELD 타입의 값으로 변환시켜 준다.
// 예를 들면, 사용자가 수행한 명령어의 인자로 "NAME"이라는 필드명을 사용하였다면 
// 프로그램 내부에서 이를 NAME(=1)으로 변환할 필요성이 있으며, 이때 이 함수를 이용한다.
//
FIELD getField(char *fieldname);

int init(FILE *fp);					//이니셜라이즈
int headcontrol(FILE *fp);			//헤드페이지 +1
int deletehead(FILE *fp, int pagenumber, int recordnumber);	// 삭제시 헤드 수정


//헤더설명 짱기네...
//오 삭제 없어				<- 응 아냐

// 1. write 구현			<--
// 2. read 구현
// 3. 검색 구현
// 4. 버그 찾기

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
	strcpy(temp.id,"20101235");
	strcpy(temp.name,"gildong");
	strcpy(temp.dept,"computer");
	strcpy(temp.year,"3");
	strcpy(temp.addr,"dongjak");
	strcpy(temp.phone,"010-585-1234");
	strcpy(temp.email,"gdhong@ssu.ac.kr");

//	writeRecord(fp, &temp);

	search(fp, 0, "20101235");

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
//언팩. 다 짰나? <- 아니요
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
		pick[2 * head.records] = pick[2 * (head.records - 1)]+pick[2*(head.records-1)+1];
		pick[2 * head.records + 1] = strlen(recordbuf);
	}
	else {
		pick[2] = 0;
		pick[3] = strlen(recordbuf);
	}
	//pagebuf[0] = head.records;
	printf("head::%d,size:%d,[1]offset:%d,size:%d\n", pick[0],pick[1],pick[2],pick[3]);

//	printf("%s\n", pagebuf);

	printf("head = %d , %d \n", head.records, head.size);
//	fread(pagebuf, PAGE_SIZE, 1, fp);
	printf("파일포인터 위치3 : %d\n", ftell(fp));

	//pagebuf[HEADER_SIZE] = *recordbuf;
	for (int i = 0; i < strlen(recordbuf); i++) {
		printf("현 위치 : %d\n", HEADER_SIZE + (int)pick[1] - recordsize + i);
		pagebuf[HEADER_SIZE + (int)pick[1] - recordsize + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
	}
	

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

	for (int i=1;i<pagenum+1;i++){
		readPage(fp, pagebuf, i);
		while (getNextRecord(pagebuf, recordbuf)) {
			printf("읽은 레코드 : %s, 길이 : %d\n", recordbuf,strlen(recordbuf));
			unpack(recordbuf, temp);
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
			memset(recordbuf, 0, MAX_RECORD_SIZE);
		}

	}
	printRecord(result, count);

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
