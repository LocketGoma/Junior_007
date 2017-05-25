#include <stdio.h>		// �ʿ��� header file �߰� ����
#include "student.h"

#pragma warning(disable: 4996)	// �����
// �ϴ�, ��������� �Ⱦ��°ŷ� �����ϰ�, �״㿡 �ٲܰ�.
 
//
// readPage() �Լ��� ���ڵ� ���Ͽ��� �־��� pagenum(=0, 1, 2, ...)�� �ش��ϴ� page��
// �о pagebuf�� ����Ű�� ���� �����ϴ� ������ �����Ѵ�. �������� ������ �� ���
// '1'��, �׷��� ���� ���� '0'�� �����Ѵ�.
// getNextRecord() �Լ��� readPage()�� ���� �о�� page���� record�� ���ʴ�� �����ϴ�
// ���� �Ѵ�. ���� page���� ������ record�� ������ '0'��, �׷��� ������ '1'�� �����Ѵ�.
// unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� record���� �� field ���� �����Ͽ� �л� ��ü�� �����ϴ� ���� �Ѵ�.
//
int readPage(FILE *fp, char *pagebuf, int pagenum);
int getNextRecord(const char *pagebuf, char *recordbuf);
void unpack(const char *recordbuf, STUDENT *s);

//
// writePage() �Լ��� ���α׷� ���� data page(pagebuf)�� ���ڵ� ������ �־��� pagenum(=0, 1, 2, ...)�� ��ġ�� 
// �����ϴ� ������ �����Ѵ�. �������� ������ �� ��� '1'��, �׷��� ���� ���� '0'�� �����Ѵ�.
//
int writePage(FILE *fp, const char *pagebuf, int pagenum);

//
// �־��� �л� ��ü�� ���ڵ� ���Ͽ� ������ �� ���Ǹ�, �̶� ������ �����ϴ� 
// page �߿��� �Ǹ����� page�� �ش� record�� �����Ѵ�. ����, �Ǹ����� page�� ���ο�
// ���ڵ带 ������ ������ �����ϸ� ���ο� page�� �ϳ� �Ҵ��ؼ� ���⿡ �����Ѵ�.
// writeRecord() �Լ��� ���ο����� pack() �Լ��� ȣ���ϸ�, pack() �Լ��� �־��� �л� ��ü�� ������ 
// ���ο� record�� page�� ����Ǿ��� ���� ���¸� recordbuf�� �����ϴ� ������ �����Ѵ�.
// writeRecord()�� ���������� �����ϸ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
//
int writeRecord(FILE *fp, const STUDENT *s);
void pack(char *recordbuf, const STUDENT *s);

//
// ���ڵ� ���Ͽ��� field�� Ű��(keyval)�� ���� ���ڵ带 �˻��ϰ� �� ����� ����Ѵ�.
// �˻��� ���ڵ带 ����� �� �ݵ�� printRecord() �Լ��� ����Ѵ�.
//
void search(FILE *fp, FIELD f, char *keyval);
void printRecord(const STUDENT *s, int n);

//
// ���ڵ��� �ʵ���� enum FIELD Ÿ���� ������ ��ȯ���� �ش�.
// ���� ���, ����ڰ� ������ ��ɾ��� ���ڷ� "NAME"�̶�� �ʵ���� ����Ͽ��ٸ� 
// ���α׷� ���ο��� �̸� NAME(=1)���� ��ȯ�� �ʿ伺�� ������, �̶� �� �Լ��� �̿��Ѵ�.
//
FIELD getField(char *fieldname);

//������� ¯���...
//�� ���� ����

// 1. write ����			<--
// 2. read ����
// 3. �˻� ����
// 4. ���� ã��

int main(int argc, char *argv[])
{
	FILE *fp;						// ��� file processing operation�� C library�� ����� ��.
	fp=fopen("students.dat","a+b");
	
	//�Ʒ� �ڵ�� argv[], ��� ��ǲ���� ����Ǿ�� �մϴ�.
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
		fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);			//pagenum ��ŭ �̵��ϰ�
		fread(pagebuf, sizeof(char), PAGE_SIZE, fp);	//pagebuf�� ������ ��°�� �ε�
		return 1;
	}
	else
		return 0;
}
int getNextRecord(const char *pagebuf, char *recordbuf){
	static int recordnum = 0;	//��� ���ڵ��ΰ�
	static int pagepoint = 0;	//���������ۿ��� ���°�ΰ�.
	

	recordbuf=pagebuf;

	recordnum++;
	return 1;

}
//����. �� ®��?
void unpack(const char *recordbuf, STUDENT *s){
	char *temp;
	int size;
	int count;
	int j;
	count = 0;
	j=0;
	size = sizeof(recordbuf)/sizeof(recordbuf[0]);
	temp=(char)malloc(sizeof(char)*31); // �ִ�ġ
	for (int i=0;i<size;i++){		//or MAX_RECORD_SIZE
		if ((int)recordbuf[i]==35){			//'#' ������
			if (count==0){
				*s->id=temp;			//�¾�?
				//�й� �ְ�
			}
			else if(count==1){
				*s->name=temp;
				//�̸� �ְ�
			}
			else if(count==2){
				*s->dept = temp;
				//�а� �ְ�
			}
			else if(count==3){
				*s->year = temp;
				//�ּ� �ְ�
			}
			else if(count==4){
				*s->addr = temp;
				//�ּ� �ְ�
			} //7�̳�?			
			else if(count==5){
				*s->phone = temp;
				//����ȣ �ְ�
			}
			else if(count==6){
				*s->email = temp;
				//�̸��� �ּ� �ְ�
			}
			else
				printf("���� ���� �߻�\n");

			temp = "";
			j=0;			//����
			count++;
		}
		else{					//'#' �ȸ�����
			temp[j]=recordbuf[i];	//������ ����
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


//1 ������ = [���][���ڵ�][���ڵ�][���ڵ�]
//��� = [���ڵ� ����][���ڵ������][���ڵ�offset][���ڵ����][���ڵ�offset][���ڵ����]... = 100byte
//���ڵ� = [7�� ������]*n = 924byte
//���⼭ ���߿� getnextRecord�� �����͸� �����.

//�׷��ϱ�. io ������ page�� ����?
int writeRecord(FILE *fp, const STUDENT *s){
	char *recordbuf;			//���ڵ����
	char *pagebuf;				//����
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
	fseek(fp, PAGE_SIZE, SEEK_SET);		//��� ������ ����


	printf("���������� ��ġ1 : %d\n", ftell(fp));
	fread(&head, PAGE_SIZE, 1, fp);
	while (fp != EOF) {
		if (head.records == 0) {	// ����� ���޽� => ���� �������ߵ�
			head.records = 1;
			head.size = strlen(recordbuf);
			
			break;
		}
		else if (head.records == 24) {		// �ִ� ���ڵ� �� ���� 
			fseek(fp, PAGE_SIZE, SEEK_CUR);
		}
		else if (head.size + strlen(recordbuf) > PAGE_SIZE - HEADER_SIZE) { // => �뷮 �ʰ�.
			fseek(fp, PAGE_SIZE, SEEK_CUR);
		}
		else											//�����Ͱ� ���ֱ��ѵ� ���� ����
		{												//= ���� �ε�
			fseek(fp, -4, SEEK_CUR);
			fread(pagebuf, PAGE_SIZE, 1, fp);
			break;
		}
	}
	pick = pagebuf;

	//pagebuf[0] = head.records;
	
	printf("%s\n", pagebuf);
	printf("���������� ��ġ2 : %d\n", ftell(fp));
	printf("head = %d , %d \n", head.records, head.size);
	fread(pagebuf, PAGE_SIZE, 1, fp);
	printf("���������� ��ġ3 : %d\n", ftell(fp));

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

//�ʵ� ����. �Ǵ��� �𸣰���;
FIELD getField(char *fieldname) {		// FIELD �����Դϴ�.
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
