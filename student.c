#include <stdio.h>		// 필요한 header file 추가 가능
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

//헤더설명 짱기네...
//오 삭제 없어

// 1. write 구현			<--
// 2. read 구현
// 3. 검색 구현
// 4. 버그 찾기

int main(int argc, char *argv[])
{
	FILE *fp;						// 모든 file processing operation은 C library를 사용할 것.
	fp=fopen("students.dat","a+b");
	
	//아래 코드는 argv[], 헤더 인풋으로 변경되어야 합니다.
	STUDENT temp;
	memset(&temp, 0, sizeof(STUDENT));

	strcpy(temp.id,"20101234");
	strcpy(temp.name,"gildong");
	strcpy(temp.dept,"computer");
	strcpy(temp.year,"3");
	strcpy(temp.addr,"dongjak");
	strcpy(temp.phone,"010-585-1234");
	strcpy(temp.email,"gdhong@ssu.ac.kr");

	

	/*
	scanf("%s",temp.id);
	scanf("%s",temp.name);
	scanf("%s",temp.dept);
	scanf("%s",temp.year);
	scanf("%s",temp.addr);
	scanf("%s",temp.phone);
	scanf("%s",temp.email);
	*/
	/**/
	/*
	temp->id = argv[0];
	temp->name = argv[1];
	temp->dept = argv[2];
	temp->year = argv[3];
	temp->addr = argv[4];
	temp->phone = argv[5];
	temp->email = argv[6];
	*/

	writeRecord(fp,&temp);
	
	return 1;
}

void printRecord(const STUDENT *s, int n)
{
	int i;

	printf("NumOfRecords = %d\n", n);
	
	for(i=0; i<n; i++)
	{
		printf("%s#%s#%s#%s#%s#%s#%s\n", s[i].id, s[i].name, s[i].dept, s[i].year, s[i].addr, s[i].phone, s[i].email);
	}
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
int getNextRecord(const char *pagebuf, char *recordbuf){
	static int recordnum = 0;	//몇번 레코드인가
	static int pagepoint = 0;	//페이지버퍼에서 몇번째인가.
	

	recordbuf=pagebuf;

	recordnum++;
	return 1;

}
//언팩. 다 짰나?
void unpack(const char *recordbuf, STUDENT *s){
	char *temp;
	int size;
	int count;
	int j;
	count = 0;
	j=0;
	size = sizeof(recordbuf)/sizeof(recordbuf[0]);
	temp=(char)malloc(sizeof(char)*31); // 최대치
	for (int i=0;i<size;i++){		//or MAX_RECORD_SIZE
		if ((int)recordbuf[i]==35){			//'#' 만나면
			if (count==0){
				*s->id=temp;			//맞아?
				//학번 넣고
			}
			else if(count==1){
				*s->name=temp;
				//이름 넣고
			}
			else if(count==2){
				*s->dept = temp;
				//학과 넣고
			}
			else if(count==3){
				*s->year = temp;
				//주소 넣고
			}
			else if(count==4){
				*s->addr = temp;
				//주소 넣고
			} //7이네?			
			else if(count==5){
				*s->phone = temp;
				//폰번호 넣고
			}
			else if(count==6){
				*s->email = temp;
				//이메일 주소 넣고
			}
			else
				printf("언팩 에러 발생\n");

			temp = "";
			j=0;			//뭐임
			count++;
		}
		else{					//'#' 안만나면
			temp[j]=recordbuf[i];	//데이터 산입
			j++;
		}	
			
	}
	
}	

int writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
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
	short *pick;
	HEADSIZE head;
	memset(&head, 0, sizeof(HEADSIZE));
	recordbuf= (char *)malloc(sizeof(char) * MAX_RECORD_SIZE);
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);

	pack(recordbuf, s);
	printf("%s\n", recordbuf);
	recordsize=strlen(recordbuf);


//	printf("%d\n", recordsize);
	fseek(fp, PAGE_SIZE, SEEK_SET);		//헤더 페이지 점프


	printf("파일포인터 위치1 : %d\n", ftell(fp));
	fread(&head, PAGE_SIZE, 1, fp);
	while (fp != EOF) {
		if (head.records == 0) {	// 빈공간 도달시 => 새로 만들어줘야됨
			head.records = 1;
			head.size = strlen(recordbuf);
			
			break;
		}
		else if (head.records == 24) {		// 최대 레코드 수 도달 
			fseek(fp, PAGE_SIZE, SEEK_CUR);
		}
		else if (head.size + strlen(recordbuf) > PAGE_SIZE - HEADER_SIZE) { // => 용량 초과.
			fseek(fp, PAGE_SIZE, SEEK_CUR);
		}
		else											//데이터가 들어가있긴한데 쓸순 있음
		{												//= 빠른 로드
			fseek(fp, -4, SEEK_CUR);
			fread(pagebuf, PAGE_SIZE, 1, fp);
			break;
		}
	}
	pick = pagebuf;

	//pagebuf[0] = head.records;
	
	printf("%s\n", pagebuf);
	printf("파일포인터 위치2 : %d\n", ftell(fp));
	printf("head = %d , %d \n", head.records, head.size);
	fread(pagebuf, PAGE_SIZE, 1, fp);
	printf("파일포인터 위치3 : %d\n", ftell(fp));

	//pagebuf[HEADER_SIZE] = *recordbuf;
	for (int i = 0; i < strlen(recordbuf); i++) {
		pagebuf[HEADER_SIZE + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
	}
	
	printf("%s\n",recordbuf);
	printf("%s\n", pagebuf);

	writePage(fp, pagebuf, 1);

}


void pack(char *recordbuf, const STUDENT *s){
	//http://mwultong.blogspot.com/2006/10/c-strcat.html
	sprintf(recordbuf,"%s#%s#%s#%s#%s#%s#%s#",s->id,s->name,s->dept,s->year,s->addr,s->phone,s->email);
	
}
void search(FILE *fp, FIELD f, char *keyval){
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
