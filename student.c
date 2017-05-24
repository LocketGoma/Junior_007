#include <stdio.h>		// 필요한 header file 추가 가능
#include "student.h"
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

int main(int argc, char *argv[])
{
	FILE *fp;						// 모든 file processing operation은 C library를 사용할 것.
	fp=fopen("students.dat","w+b");
	
	//아래 코드는 argv[], 헤더 인풋으로 변경되어야 합니다.
	STUDENT temp;
	
	scanf("%s",temp.id);
	scanf("%s",temp.name);
	scanf("%s",temp.dept);
	scanf("%s",temp.year);
	scanf("%s",temp.addr);
	scanf("%s",temp.phone);
	scanf("%s",temp.email);
	
	writeRecord(fp,temp);
	
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
}
int getNextRecord(const char *pagebuf, char *recordbuf){
}
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
		if ((int)recordbuf[i]==35){
			if (count==0){
				//학번 넣고
			}
			else if(count==1){
				//이름 넣고
			}
			else if(count==2){
				//학과 넣고
			}
			else if(count==3){
				//주소 넣고
			}
			else if(count==4){
				//이메일 주소 넣고
			} //7이네?			
			else if(count==5){
			}
			else if(count==6){
			}
			else
				printf("언팩 에러 발생\n");
			j=0;
			count++;
		}
		else{
			temp[j]=recordbuf[i];
			j++
		}	
			
	}
	
}
//1 페이지 = [헤더][레코드][레코드][레코드]
//헤더 = [레코드 개수][레코드사이즈][레코드offset][레코드길이][레코드offset][레코드길이]... = 100byte
//레코드 = [7개 데이터]*n = 924byte
int writeRecord(FILE *fp, const STUDENT *s){
	int record_count;	//지금 도착한 페이지의 레코드가 몇개인가.
	int last_offset;	//마지막 페이지 offset
	int last_len;		//마지막 페이지 length
	char *buf;
	buf = (char)melloc(sizeof(s));			//되나
	record_count=0;
	
	pack(buf,s);
	
	if(buf!=NULL){
		strlen(buf);
		//헤더 페이지, 헤더 페이지 수 확인.
		fseek(fp, PAGE_SIZE, SEEK_SET);	// 일단 다음 페이지로 가고.
		//while(빈 페이지 발견시까지)
		// if (레코드 사이즈 + strlen(buf) <= 924)	
		//페이지의 헤더를 읽음. (#record)
		fseek(fp,4+4*record_count-1,SEEK_CUR);
		last_offset=//offset 읽음
		last_len=//length 읽음
		
		//쓰기
		next_offset(2byte) = last_offset+last_len;
		next_size(2byte) = strlen(buf);
		
		//현재 페이지의 맨 앞으로 되돌아감.
		fseek;//
		//#record 수정
		//#TRS 수정
		fseek(fp,96,SEEK_CUR);
		fseek(fp,next_offset,SEEK_CUR);
		fwrite((void *)buf, next_size, 1, fp);
		
		return 1;
	}
	else
		return 0;
	
	
}
void pack(char *recordbuf, const STUDENT *s){
	//http://mwultong.blogspot.com/2006/10/c-strcat.html
	sprintf(recordbuf,"%s#%s#%s#%s#%s#%s#%s\n",s.id,s.name,s.dept,s.year,s.addr,s.phone,s.email);
	
}
void search(FILE *fp, FIELD f, char *keyval){
}
void printRecord(const STUDENT *s, int n){
	
	
}

