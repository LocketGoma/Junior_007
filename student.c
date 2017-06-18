#include <stdio.h>		// �ʿ��� header file �߰� ����
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>	//<- ��������
#include "student.h"

// �����Ұ�
// 1. argv ��ǲ Ȯ��
// 2. writerecord �κ� �ڵ� ���� (fread �� fp ������ ���� ���� ����)
// 3. ��Ÿ ���� ���.



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
//
// ���ڵ� ���Ͽ� ���ο� ���ڵ带 �߰��Ѵ�. �̶� 'right size'�� ������ ������ ���� ���ڵ带 ã�Ƽ�
// �� �ڸ��� ���ο� ���ڵ带 �߰��Ѵ�. ���� right size�� �������� �ʴ� ��� ������ ���� append�Ѵ�.
// �Լ��� ���������� ����Ǹ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
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
// ���ڵ� ���Ͽ��� �־��� '�й�' Ű���� �����ϴ� ���ڵ带 �����Ѵ�. ������ �����ϴ� ���ڵ带
// ȭ�鿡 ����ϸ�, �̶� �Ʒ��� printDeletedRecord()�� �ݵ�� ����Ѵ�. �����, ������ �����ϴ�
// ���ڵ带 ã�� ��, ������ ���ڵ�� �˻� ��� ���ڵ忡�� ���ܵǾ�� �Ѵ�(�Ǻ����δ� delete mark '*'�� �̿�).
// ����������, search() �Լ��� �̸� �˻��ϴ� �ڵ尡 ���ԵǾ�� �Ѵ�.
// �Լ��� ���������� ����Ǹ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
//
int deleteRecord(FILE *fp, FIELD f, char *keyval);
void printDeletedRecord(const STUDENT *s, int n);


//
// ���ڵ��� �ʵ���� enum FIELD Ÿ���� ������ ��ȯ���� �ش�.
// ���� ���, ����ڰ� ������ ��ɾ��� ���ڷ� "NAME"�̶�� �ʵ���� ����Ͽ��ٸ� 
// ���α׷� ���ο��� �̸� NAME(=1)���� ��ȯ�� �ʿ伺�� ������, �̶� �� �Լ��� �̿��Ѵ�.
//
FIELD getField(char *fieldname);

int init(FILE *fp);					//�̴ϼȶ�����
int headcontrol(FILE *fp);			//��������� +1
int deletehead(FILE *fp, int pagenumber, int recordnumber);	// ������ ��� ����

// �߰� ������
// 1. blanknext - ���� �� �׸� ����, �������� ����� �ִٸ� 1, ������ (������ -1,-1 �̸�) 0 ����
// 2. blanksize - ���� �� �׸��� ����� ������.
// 3. blankaddrchnage - A -> B -> C ����, B�� ä������ A->C�� �ٲ��ִ� ����.
int blanknext(FILE *fp, int addr[2]);		// addr : ���� �ּ� ����, pagenum/recordnum : �Է�
int blanksize(FILE *fp,int pagenum, int recordnum);					// ������ ����.
int blankaddrchange(FILE *fp,int *before, int *next);				// before : ���� ����Ʈ�� ������ ��, next : ���� ��, ���� ����� ���� 1, �ƴϸ� ���� 0


// 1. write ����			
// 2. read ����
// 3. �˻� ����
// 4. ���� ã��
// 5. ���� ����				
// 6. ���� ����			<--

int main(int argc, char *argv[])
{
	FILE *fp;						// ��� file processing operation�� C library�� ����� ��.

	char file[30];
	if (access("students.dat", 0) != 0) {
		printf("������");
		fp = fopen("students.dat", "a+b");
		init(fp);			//�����Ű��
//		char *buf;
//		unsigned short *pick;
//		buf = (char *)malloc(sizeof(char)*PAGE_SIZE);
//		pick = (unsigned short *)buf;
//		pick[0] = 1;
//		fwrite(buf, PAGE_SIZE, 1, fp);
	}
	else {
		printf("����");
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

	//VS �׽�Ʈ��
	strcpy(temp.id,"20101241");
	strcpy(temp.name,"gildong");
	strcpy(temp.dept,"computer");
	strcpy(temp.year,"4");
	strcpy(temp.addr,"dongjak");
	strcpy(temp.phone,"010-585-1234");
	strcpy(temp.email,"goodhong@ssu.ac.kr");

	writeRecord(fp, &temp);

//	search(fp, 0, "20101236");

//	deleteRecord(fp, 0, "20101237");				// ���� ��Ʈ �ٽ� ã�ƺ���.

	int addr[2] = { -1,-1 };
	int next[2] = { 1,1 };
//	blanknext(fp, addr);
//	printf("%d,%d\n", addr[0], addr[1]);
//	blankaddrchange(fp, addr, next);

/*	
	//argument ����.
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
		//�Ľ� ���μ���
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
		//�Ľ� ��

		printf("field : %s, find : %s\n", field, find);
		printf("%d",getField(field));				// �ߵ�
		search(fp, getField(field), find);
	}

*/
	
	return 1;
}

int init(FILE *fp) {			//�̴ϼȶ�����
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

	current = ftell(fp);				//���� ��ġ �а�
	fseek(fp, 0, SEEK_SET);				//��������� ������
	readPage(fp, temp, 0);

	pick = (unsigned short*)temp;		//������� �а�
	printf("pick : %d\n", pick[0]);		//��� ���� �����..
	pick[0]++;							//��尪 +1
	printf("pick after : %d\n", pick[0]);		//after
	writePage(fp, temp, 0);				//��� �����ϰ�
	fseek(fp, current, SEEK_SET);		//�ٽ� ���� ��ġ�� ������

	return 1;
}
int deletehead(FILE *fp, int pagenumber, int recordnumber) {	//������ ��� ������.

}
int blanknext(FILE *fp, int addr[2]) {		// addr : �Է� �� ���� �ּ� ����, -1, -1 ���Խ� ����� ��.
	char *pagebuf;							// 0 : ���� Ȥ�� ���̻� ����
	char *header;							// 1 : ������ ����.
	char *recordbuf;
	unsigned short *headpick;		// 0 : �� ������ �� / 1 : ������ ���� ������ ��ȣ / 2 : ������ ���� ���ڵ� ��ȣ
	unsigned short *pick;
	int pointsize = HEADER_SIZE;
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	recordbuf = (char *)malloc(sizeof(char)*MAX_RECORD_SIZE);
	header = (char *)malloc(sizeof(char)*PAGE_SIZE);


	if (addr[0] == -1)			//��
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
int blanksize(FILE *fp, int pagenum, int recordnum) {					// ������ ����.
	char *pagebuf;
	unsigned short *pick;
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	readPage(fp, pagebuf, pagenum);

	pick = (unsigned short *)pagebuf;
	if (pick[2 + recordnum * 2 + 1] > MAX_RECORD_SIZE)
		return -1;		//������

	return pick[2 + recordnum * 2 +1];

}
int blankaddrchange(FILE *fp,int *bef, int *nex) {				// before : ���� ����Ʈ�� ������ ��, next : ���� ��, ���� ����� ���� 1, �ƴϸ� ���� 0
	int before[2];
	int now[2];													// ���� ����
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

	blanknext(fp, next);		//3���� �غ� ��
								//Before > now > next ==> before -> next
								//���̻� ���� ������ (before ��ǲ�� -1�϶�)
	//�׽�Ʈ//
	printf("before : %d,%d\nnow : %d,%d\nnext:%d,%d\n--2--\n", before[0], before[1], now[0], now[1], next[0], next[1]);

	if (before[0] == -1) {				//������ �������� ���
		readPage(fp, pagebuf, 0);
		pick = (unsigned short *)pagebuf;
		pick[1] = next[0];
		pick[2] = next[1];
		writePage(fp, pagebuf, 0);
	}	
	else if (next[0] == -1) {			//������ ���ΰ��
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
		fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);			//pagenum ��ŭ �̵��ϰ�
		fread(pagebuf, sizeof(char), PAGE_SIZE, fp);	//pagebuf�� ������ ��°�� �ε�
		return 1;
	}
	else
		return 0;
}
int getNextRecord(const char *pagebuf, char *recordbuf){			// ���⼭���� �ٽ� ¥����.
	static int recordnum = 0;	//��� ���ڵ� �о���ϴ°�
	static int pagepoint = 0;	//���������ۿ��� ���°�ΰ�.
	unsigned short *pick;
	pick = (unsigned short *)pagebuf;

	int ret = 0;				//0~7
	int rotate=0;				//������ڵ� �а� �ִ°�

	if(recordnum==0)
	for (int i = 0; i < MAX_RECORD_SIZE; i++) {
		recordbuf[i] = pagebuf[100 + i];
//		printf("[i:%d]", i);
		if (pagebuf[HEADER_SIZE + i] == 35) {
			ret++;
//			printf("%d�� \n", ret);
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
	
	printf("���ڵ� ��ȣ : %d, ������Ʈ : %d\n", recordnum, rotate);
	recordnum++;
	return 1;

}
//����. �� ®��? <- �Ƹ���?
void unpack(const char *recordbuf, STUDENT *s){
	char temp[31]="";
	int size;
	int count;
	int j;
	count = 0;
	j=0;
	size = strlen(recordbuf);
//	temp=(char *)malloc(sizeof(char)*31); // �ִ�ġ
//	printf("size : %d\n", size);
	

	for (int i=0;i<size;i++){		//or MAX_RECORD_SIZE
//		printf("i = %d", i);
		if (recordbuf[i]=='#'){			//'#' ������
//			temp[j + 1] = '\0';
//			printf("\n[%s]/lenght:%d", temp,strlen(temp));
			if (count==0){
				strcpy(s->id, temp);
				//�й� �ְ�
			}
			else if(count==1){
				strcpy(s->name, temp);
				//�̸� �ְ�
			}
			else if(count==2){
				strcpy(s->dept, temp);
				//�а� �ְ�
			}
			else if(count==3){
				strcpy(s->year, temp);
				//�ּ� �ְ�
			}
			else if(count==4){
				strcpy(s->addr, temp);
				//�ּ� �ְ�
			} //7�̳�?			
			else if(count==5){
				strcpy(s->phone, temp);
				//����ȣ �ְ�
			}
			else if(count==6){
				strcpy(s->email, temp);
				//�̸��� �ּ� �ְ�
			}
			else if (count == 7) {
				break;
			}
			else
				printf("���� ���� �߻�\n");

			temp[0] = "\0";
//			temp = (char *)malloc(sizeof(char) * 31);
//			printf("[:%s]\n", temp);
			j=0;			//����
			count++;
		}
		else{					//'#' �ȸ�����
			temp[j]=recordbuf[i];	//������ ����
//			printf("(%c)\n", temp[j]);
			j++;
		}	
			
	}
	
}	

int writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp, PAGE_SIZE*pagenum, SEEK_SET);
	printf("���������� ��ġ4 : %d\n", ftell(fp));
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
	int closet;					//�� á������ �׽�Ʈ.
	int start;					//���������� ��, start = ftell(fp) ���Ͽ� ��ġ�ϸ� => ���� �������� ����á��, ������ ����.
	int stock;					//������ ��.
	unsigned short *pick;		// picker
	HEADSIZE head;				//������ ��� �д� �༮. 
	memset(&head, 0, sizeof(HEADSIZE));
	recordbuf= (char *)malloc(sizeof(char) * MAX_RECORD_SIZE);
	pagebuf = (char *)malloc(sizeof(char)*PAGE_SIZE);
	memset(pagebuf, 0, sizeof(char)*PAGE_SIZE);

	//�߰�//
	int addr[2] = { -1,-1 };
	int before[2] = { -1,-1 };
	int bsize=0;

	pack(recordbuf, s);
	printf("%s\n", recordbuf);
	recordsize=strlen(recordbuf);


	if (fp == NULL) {
		printf("����");
	}
	printf("%d\n", strlen(recordbuf));

//	printf("%d\n", recordsize);
	fseek(fp, PAGE_SIZE, SEEK_SET);		//��� ������ ����

	start = 0;
	closet = 0;
	stock = 1;				//1�� �ٲٱ�
	
	printf("���������� ��ġ1 : %d\n", ftell(fp));
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
			printf("�� ��ġ : %d\n", HEADER_SIZE + (int)addr[1] - recordsize + i);
			pagebuf[HEADER_SIZE + pick[2+addr[1]*2] + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
		}

		stock = addr[0];
	}
	else {
		fseek(fp, PAGE_SIZE, SEEK_SET);
		while (fp != EOF) {
			//		headcontrol(fp); // �׽�Ʈ ����
			fread(pagebuf, PAGE_SIZE, 1, fp);
			
			pick = (unsigned short *)pagebuf;
			head.records = pick[0];
			head.size = pick[1];
			//		printf("fread ���� : %d\n",fread(&head, PAGE_SIZE, 1, fp));
			printf("%d:%d\n", head.records, head.size);
			printf("���������� ��ġ2 : %d\n", ftell(fp));
			if (ftell(fp) != start)
				closet = 0;

			if (head.records == 0) {	// ����� ���޽� => ���� �������ߵ�			<- �ٽ� ������.
				head.records = 1;
				head.size = recordsize;


				printf("���̽� 1\n");
				break;
			}
			else if (closet == 1) {
				printf("���̽� 4\n");
				headcontrol(fp);			//�Ƿ���..
				fclose(fp);
				memset(pagebuf, 0, sizeof(char)*PAGE_SIZE);
				head.records = 1;
				head.size = recordsize;

				fp = fopen("students.dat", "a+b");

				closet = 0;
				break;
			}
			else if (head.records == 24) {		// �ִ� ���ڵ� �� ����		==> ���� ������ �ε�.
				printf("���̽� 3-1\n");
				start = ftell(fp);
				closet = 1;
				stock++;
				//	fseek(fp, PAGE_SIZE-4, SEEK_CUR);
			}
			else if ((int)head.size + recordsize > PAGE_SIZE - HEADER_SIZE) {
				printf("���̽� 3-2\n");
				start = ftell(fp);
				closet = 1;
				stock++;
				//fseek(fp, PAGE_SIZE, SEEK_CUR);
			}
			else											//�����Ͱ� ���ֱ��ѵ� ���� ����
			{												//= ���� �ε�
	//			fseek(fp, -PAGE_SIZE, SEEK_CUR);
	//			fread(pagebuf, PAGE_SIZE, 1, fp);
				fseek(fp, stock, SEEK_SET);
				head.records++;
				head.size += strlen(recordbuf);
				printf("���̽�2\n");
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
			printf("�� ��ġ : %d\n", HEADER_SIZE + (int)pick[1] - recordsize + i);
			pagebuf[HEADER_SIZE + (int)pick[1] - recordsize + i] = recordbuf[i];			//HEADERSIZE+OFFSET+i
		}
	}
	//pagebuf[0] = head.records;
//	printf("head::%d,size:%d,[1]offset:%d,size:%d\n", pick[0],pick[1],pick[2],pick[3]);

//	printf("%s\n", pagebuf);

//	printf("head = %d , %d \n", head.records, head.size);
//	fread(pagebuf, PAGE_SIZE, 1, fp);
//	printf("���������� ��ġ3 : %d\n", ftell(fp));

	//pagebuf[HEADER_SIZE] = *recordbuf;

	

	printf("���:%d",writePage(fp, pagebuf, stock));

	printf("");
}


void pack(char *recordbuf, const STUDENT *s){
	//http://mwultong.blogspot.com/2006/10/c-strcat.html
	sprintf(recordbuf,"%s#%s#%s#%s#%s#%s#%s#",s->id,s->name,s->dept,s->year,s->addr,s->phone,s->email);
	
}
void search(FILE *fp, FIELD f, char *keyval){
	int pagenum;
	char *recordbuf;	//���ڵ� ����
	char *pagebuf;		//������ �б�
	char *header;		//��� �б�
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

	/*�߰�*/
	unsigned short * delpick;
	int recordsize = HEADER_SIZE;
	int recordnum = -1;

	for (int i=1;i<pagenum+1;i++){
		readPage(fp, pagebuf, i);
		while (getNextRecord(pagebuf, recordbuf)) {
			recordnum++;
			delpick= (unsigned short *) recordbuf;
			unpack(recordbuf, temp);
			if (strlen(recordbuf) == 1) {													//'*'		������ ���ڵ�� �ش� ����� ���� ������ �о ����.
				printf("���ڵ� ���� ������ : %d\n", pick[2 + recordnum * 2 + 1]);		//2 = ���ڵ� ��� ���� + i * 2 = i��° ���ڵ� ������ ���� + '+1' ������ ������ üũ
				recordsize += ((pick[2 + recordnum * 2 + 1]) - 1);							//���ڵ� ���� :: [offset][size]
			}
			else 
				printf("�Ľ̰��:%s\n", temp->id);


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

int deleteRecord(FILE *fp, FIELD f, char *keyval) {			//search ���. ���� ��
	int totalpage;
	char *recordbuf;	//���ڵ� ����
	char *pagebuf;		//������ �б�
	char *header;		//��� �б�
	int count = 0;
	STUDENT *temp;
	STUDENT result[100];//100...
	unsigned short *headpick;		// 0 : �� ������ �� / 1 : ������ ���� ������ ��ȣ / 2 : ������ ���� ���ڵ� ��ȣ
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
		printf("�߸��� field�Դϴ�\n");
		return 0;
	}

	for (int i = 1; i<totalpage + 1; i++) {
		readPage(fp, pagebuf, i);
		pick = (unsigned short *)pagebuf;
		while (getNextRecord(pagebuf, recordbuf)) {
			//recordpick = (unsigned short *)recordbuf;
			recordnum++;		//0���� ����
			printf("���� ���ڵ� : %s, ���� : %d\n", recordbuf, strlen(recordbuf));
			unpack(recordbuf, temp);
			if (strlen(recordbuf)==1) {													//'*'		������ ���ڵ�� �ش� ����� ���� ������ �о ����.
				printf("���ڵ� ���� ������ : %d\n", pick[2 + recordnum * 2 + 1]);		//2 = ���ڵ� ��� ���� + i * 2 = i��° ���ڵ� ������ ���� + '+1' ������ ������ üũ
				recordsize += ((pick[2+recordnum*2+1]) - 1);							//���ڵ� ���� :: [offset][size]
			}																			//�ٵ� �̰� ���� �ݹ� �ϰڴµ�. �߰���. ������ ���������� ������.
			else
			printf("�Ľ̰��:%s\n", temp->id);
				if (!strcmp(temp->id, keyval)) {		//��⼭ ����
					printf("��ġ");
					printf("���ڵ� ������ : %d\n", recordsize);
					printDeletedRecord(temp, recordsize);
					if (headpick[1] == -1) {
						headpick[1] = i;
						headpick[2] = recordnum;

						pick[recordsize / 2] = '*';			//���� ���ڵ� �� ó�� *
						pick[(recordsize / 2) + 1] = -1;
						pick[(recordsize / 2) + 2] = -1;
					}
					else {
						pick[recordsize / 2] = '*';			//���� ���ڵ� �� ó�� *
						pick[(recordsize / 2) + 1] = headpick[1];
						pick[(recordsize / 2) + 2] = headpick[2];

						headpick[1] = i;
						headpick[2] = recordnum;
					}
					writePage(fp, header, 0);				//��� ����
					writePage(fp, pagebuf, i);				//��ü ����

					return 1;
				}
				else {
					printf("����ġ\n");
				}
			recordsize += strlen(recordbuf);
			memset(recordbuf, 0, MAX_RECORD_SIZE);

		}

	}
	//printRecord(result, count);
	printf("��������� �������� �ʽ��ϴ�\n");
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
