#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MANAGE_SIZE 148

void manager_mode();
void user_mode();

int add_point();
int del_point();
int update_point();

int add_room(int point);
int del_room(int point);
int update_room(int point);

int inq_res(char id[]);
int room_inq_sel(char* id, int point);
int room_info_res(char* id, int point, int room);
int date_check(char* id, int point, int room);
int res_time_num(char* id, int point, int room, int date);

int id_inq(char* id);
int del_res(int num);


typedef struct _POINT{
	int point_exist;
	int people_num[5];
}POINT;

typedef struct _MANAGE{
	int reserve_num;
	POINT points[6];	
}MANAGE;

typedef struct _RESERVE{
	char id[11];
	int point_num;
	int room_num;
	int date;
	int time;
	int people_num;
}RESERVE;


void manager_mode(){
	while(1){
		printf("\n*************관리자 모드**************\n");

		FILE *fp;
		fp = fopen("record.dat", "r+");
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);
		printf("현재 지점: ");
		for(int i=0; i<6; i++){
			if(mdata.points[i].point_exist==1)
				printf("%d ", i+1);
		}
		fclose(fp);
		
		printf("\n(지점 추가: 1 / 지점 수정: 2 / 지점 삭제: 3 / 돌아가기: 4)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"1")==0){
			int select = add_point();
			if(select == 1)
				continue;
			else
				return;
		}
		else if(strcmp(input,"2")==0){
			int select = update_point();
			if(select == 1)
				continue;
			else
				return;
		}
		else if(strcmp(input,"3")==0){
			int select = del_point();
			if(select == 1)
				continue;
			else
				return;
		}
		else if(strcmp(input,"4")==0){
			printf("초기 화면으로 돌아갑니다.\n\n");
			return;
		}
		else{
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}	
	}
}

int add_point(){
	while(1){
		printf("\n*************지점 추가*************\n");
		printf("추가할 지점 번호를 입력하시오.\n");
		printf("(1 ~ 6 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && strcmp(input,"6")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int point_num = (int)input[0] - 49;
		
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point_num].point_exist == 0){
			mdata.points[point_num].point_exist = 1;
			fseek(fp, 0, SEEK_SET);
			fwrite(&mdata, MANAGE_SIZE, 1, fp);
			printf("해당 지점이 추가되었습니다.\n\n");
			fclose(fp);
			break;
		}
		else{
			printf("해당 지점은 이미 존재합니다.\n\n");
			fclose(fp);
			return 1;
		}
	}
	return 0;
}

int del_point(){
	while(1){
		printf("\n*************지점 삭제*************\n");
		printf("삭제할 지점 번호를 입력하시오.\n");
		printf("(1 ~ 6 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && strcmp(input,"6")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int point_num = (int)input[0] - 49;
		
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point_num].point_exist != 0){
			mdata.points[point_num].point_exist = 0;
			fseek(fp, 0, SEEK_SET);
			fwrite(&mdata, MANAGE_SIZE, 1, fp);

			for(int i=0; i<mdata.reserve_num; i++){
				RESERVE rdata;
				fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
				fread(&rdata, 32, 1, fp);
				if(rdata.point_num == point_num){
					strcpy(rdata.id, "NULL");
					rdata.point_num = -1;
					rdata.room_num = -1;
					rdata.date = 0;
					rdata.time = 0;
					rdata.people_num = 0;

					fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
					fwrite(&rdata, 32, 1, fp);
				}
			}

			printf("해당 지점이 삭제되었습니다.\n\n");
			fclose(fp);
			break;
		}
		else{
			printf("해당 지점은 존재하지 않습니다.\n");
			fclose(fp);
			return 1;
		}
	}
	return 0;
}

int update_point(){
	while(1){
		printf("\n*************지점 수정*************\n");
		printf("수정할 지점 번호를 입력하시오.\n");
		printf("(1 ~ 6 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && strcmp(input,"6")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int point_num = (int)input[0] - 49;

		FILE *fp;
		fp = fopen("record.dat", "r+");
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point_num].point_exist != 0){
			fclose(fp);
		}
		else{
			printf("해당 지점은 존재하지 않습니다.\n");
			fclose(fp);
			return 1;
		}


		while(1){
			printf("\n*************%d번 지점**************\n", point_num+1);

			FILE *fp;
			fp = fopen("record.dat", "r+");
			MANAGE mdata;
			fseek(fp, 0, SEEK_SET);
			fread(&mdata, MANAGE_SIZE, 1, fp);
			printf("현재 공간: ");
			for(int i=0; i<5; i++){
				if(mdata.points[point_num].people_num[i] != 0)
					printf("%d ", i+1);
			}
			fclose(fp);
			
			printf("\n(공간 추가: 1 / 공간 수정: 2 / 공간 삭제: 3 / 돌아가기: 4)\n");
			printf("입력: ");
			char input[10];
			scanf("%s", input);
			if(strcmp(input,"1")==0){
				int select = add_room(point_num);
				if(select == 1)
					continue;
				else
					return 0;
			}
			else if(strcmp(input,"2")==0){
				int select = update_room(point_num);
				if(select == 1)
					continue;
				else
					return 0;
			}
			else if(strcmp(input,"3")==0){
				int select = del_room(point_num);
				if(select == 1)
					continue;
				else
					return 0;
			}
			else if(strcmp(input,"4")==0){
				printf("이전 화면으로 돌아갑니다.\n\n");
				return 1;
			}
			else{
				printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
				continue;
			}	
		}
	}
	return 0;
}

int add_room(int point){
	while(1){
		printf("\n*************%d번 지점 공간 추가*************\n", point+1);
		printf("추가할 공간 번호를 입력하시오.\n");
		printf("(1 ~ 5 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int room_num = (int)input[0] - 49;
		
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point].people_num[room_num] == 0){
			while(1){
				printf("허용 인원을 입력하시오.\n");
				printf("(1 ~ 10 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
				printf("입력: ");
				char input[2];
				scanf("%s", input);
				if(strcmp(input,"0")==0)
					return 1;
				if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && 
				   strcmp(input,"6")!=0 && strcmp(input,"7")!=0 && strcmp(input,"8")!=0 && strcmp(input,"9")!=0 && strcmp(input,"10")!=0){
					printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
					continue;
				}
				int room_people_num = atoi(input);
				mdata.points[point].people_num[room_num] = room_people_num;

				fseek(fp, 0, SEEK_SET);
				fwrite(&mdata, MANAGE_SIZE, 1, fp);
				printf("해당 공간이 추가되었습니다.\n\n");
				fclose(fp);
				return 0;
			}
		}
		else{
			printf("해당 공간은 이미 존재합니다.\n\n");
			fclose(fp);
			return 1;
		}
	}
	return 0;
}
int del_room(int point){
	while(1){
		printf("\n*************%d번 지점 공간 삭제*************\n", point+1);
		printf("삭제할 공간 번호를 입력하시오.\n");
		printf("(1 ~ 5 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int room_num = (int)input[0] - 49;
		
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point].people_num[room_num] != 0){
			mdata.points[point].people_num[room_num] = 0;
			fseek(fp, 0, SEEK_SET);
			fwrite(&mdata, MANAGE_SIZE, 1, fp);

			for(int i=0; i<mdata.reserve_num; i++){
				RESERVE rdata;
				fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
				fread(&rdata, 32, 1, fp);
				if(rdata.point_num==point && rdata.room_num == room_num){
					strcpy(rdata.id, "NULL");
					rdata.point_num = -1;
					rdata.room_num = -1;
					rdata.date = 0;
					rdata.time = 0;
					rdata.people_num = 0;

					fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
					fwrite(&rdata, 32, 1, fp);
				}
			}
			
			printf("해당 공간이 삭제되었습니다.\n\n");
			fclose(fp);
			return 0;
		}
		else{
			printf("해당 공간은 존재하지 않습니다.\n\n");
			fclose(fp);
			return 1;
		}
	}
	return 0;
}


int update_room(int point){
	while(1){
		printf("\n*************%d번 지점 공간 수정*************\n", point+1);
		printf("수정할 공간 번호를 입력하시오.\n");
		printf("(1 ~ 5 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int room_num = (int)input[0] - 49;
		
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		if(mdata.points[point].people_num[room_num] != 0){
			while(1){
				printf("허용 인원을 입력하시오.\n");
				printf("(1 ~ 10 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
				printf("입력: ");
				char input[2];
				scanf("%s", input);
				if(strcmp(input,"0")==0)
					return 1;
				if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && 
				   strcmp(input,"6")!=0 && strcmp(input,"7")!=0 && strcmp(input,"8")!=0 && strcmp(input,"9")!=0 && strcmp(input,"10")!=0){
					printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
					continue;
				}
				int room_people_num = atoi(input);
				mdata.points[point].people_num[room_num] = room_people_num;

				fseek(fp, 0, SEEK_SET);
				fwrite(&mdata, MANAGE_SIZE, 1, fp);

				for(int i=0; i<mdata.reserve_num; i++){
					RESERVE rdata;
					fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
					fread(&rdata, 32, 1, fp);
					if(rdata.point_num==point && rdata.room_num==room_num && rdata.people_num > room_people_num){
						strcpy(rdata.id, "NULL");
						rdata.point_num = -1;
						rdata.room_num = -1;
						rdata.date = 0;
						rdata.time = 0;
						rdata.people_num = 0;

						fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
						fwrite(&rdata, 32, 1, fp);
					}
				}
				printf("해당 공간이 수정되었습니다.\n\n");
				fclose(fp);
				return 0;
			}
		}
		else{
			printf("해당 공간은 존재하지 않습니다.\n\n");
			fclose(fp);
			return 1;
		}
	}
	return 0;
}



void user_mode(){
	char id[11];
	while(1){
		printf("\n*************사용자 모드*************\n");
		strcpy(id, "");
		printf("ID를 입력하시오.\n");
		printf("(영문자와 숫자 조합, 최소 5글자 최대 10글자 가능, 뒤로가기는 0)\n");
		printf("입력: ");
		scanf("%s", id);
		if(strcmp(id, "0")==0)
			return;
		if(strlen(id)>=5 && strlen(id)<=10){
			int wrong = 0;
			int alpha_exist = 0;
			int num_exist = 0;
			for(int i=0; i<strlen(id); i++){
				if(isdigit(id[i])!=0){
					num_exist++;
					continue;
				}
				else if(isalpha(id[i])!=0){
					alpha_exist++;
					continue;
				}
				else{
					wrong = 1;
					break;
				}
						
			}
			if(wrong==0 && alpha_exist!=0 && num_exist!=0)
				break;
			else{
				printf("잘못된 입력입니다. 영문자 및 숫자 조합, 최소 5글자 최대 10글자로 입력해주십시오.\n\n");
				continue;
			}
		}
		else{
			printf("잘못된 입력입니다. 영문자 및 숫자 조합, 최소 5글자 최대 10글자로 입력해주십시오.\n\n");
			continue;
		}	
		
	}
	char id_new[11];
	strcpy(id_new, id);
	while(1){
		printf("\n-----메뉴 선택-----");
		printf("\n(공간 조회 및 예약: 1 / 예약 조회 및 수정: 2 / 돌아가기: 3\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"1")==0){
			printf("\n*************공간 조회 및 예약*************\n");
			int select = inq_res(id_new);
			if(select == 1)
				continue;
			else
				return;
		}
		else if(strcmp(input,"2")==0){
			int select = id_inq(id_new);
			if(select == 1)
				continue;
			else
				return;
		}
		else if(strcmp(input,"3")==0){
			printf("초기 화면으로 돌아갑니다.\n\n");
			return;
		}
		else{
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}	
	}
}

int inq_res(char id[]){
	while(1){
		//printf("\n*************공간 조회 및 예약*************\n");

		FILE *fp;
		fp = fopen("record.dat", "r");
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);
		printf("예약 가능한 지점: ");
		for(int i=0; i<6; i++){
			if(mdata.points[i].point_exist==1)
				printf("%d ", i+1);
		}
		fclose(fp);
		printf("\n");

		printf("\n예약할 지점을 선택하시오.\n");
		printf("(1 ~ 6 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		int point_num;
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0 && strcmp(input,"6")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			while(getchar() != '\n');
			continue;
		}
		else{
			point_num = (int)input[0] - 49;
			int exist = 0;
			for(int i=0; i<6; i++){
				if(mdata.points[i].point_exist==1 && i==point_num){
					exist = 1;
					int select = room_inq_sel(id, i);
					if(select == 1){
						exist = 0;
						break;
					}
					else
						return 0;
				}
			}
			if(exist==0){
				printf("해당 지점이 존재하지 않습니다.\n\n");
				continue;
			}
		}
	}
	return 0;
}

int room_inq_sel(char* id, int point){
	while(1){
		FILE *fp;
		fp = fopen("record.dat", "r");
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);
		printf("\n%d번 지점 예약 가능한 공간: ", point+1);
		for(int i=0; i<5; i++){
			if(mdata.points[point].people_num[i]!=0)
				printf("%d ", i+1);
		}
		fclose(fp);
		printf("\n");

		printf("예약할 공간을 선택하시오.\n");
		printf("(1 ~ 5 사이의 자연수, 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		int room_num;
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0 && strcmp(input,"4")!=0 && strcmp(input,"5")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			continue;
		}
		else{
			room_num = (int)input[0] - 49;
			int exist = 0;
			for(int i=0; i<5; i++){
				if(mdata.points[point].people_num[i]!=0 && i==room_num){
					exist = 1;					
					int select = room_info_res(id, point ,i);
					if(select == 1){
						exist = 0;
						continue;
					}
					else
						return 0;
				}
			}
			if(exist==0){
				printf("공간이 존재하지 않습니다.\n");
				continue;
			}
		}
	}
	return 0;
}

int room_info_res(char* id, int point, int room){
	while(1){
		printf("\n_____%d번 지점 %d번 공간 정보_____\n", point+1, room+1);
		FILE *fp;
		fp = fopen("record.dat", "r");
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);
		printf("사용 가능 인원: %d\n", mdata.points[point].people_num[room]);
		fclose(fp);

		printf("예약하기는 1, 뒤로가기는 0을 입력하시오.\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"0")==0)
			return 1;
		if(strcmp(input,"1")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			continue;
		}
		else{
			int select = date_check(id, point, room);
			if(select == 1){
				continue;
			}
			else
				return 0;
		}
	}
	return 0;
}

int date_check(char* id, int point, int room){
	while(1){
		printf("\n예약 일자를 선택하십시오.\n");
		printf("(6자리 숫자(YYMMDD), 당일 예약은 불가합니다. 뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[6];
		scanf("%s", input);
					
		if(strcmp(input,"0")==0)
			return 1;
		int wrong = 0;
		for(int i=0; i<6; i++){
			if(isdigit(input[i])==0){
				wrong =1;
				break;
			}		
		}	
		if(wrong ==1){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			continue;
		}
		int date = atoi(input);
		if(date/100%100<1 || date/100%100>12 || date%100<1 || date%100>31){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			continue;
		}

		time_t timer;
		struct tm *t;
		int nowday;
		timer = time(NULL);
		t = localtime(&timer);
		nowday = (t->tm_year%100)*10000+(t->tm_mon+1)*100+t->tm_mday;

		if(date<=nowday){
			printf("해당 날짜는 예약 불가능합니다. 당일 이후부터 예약 가능합니다.\n");
			continue;
		}
		int select = res_time_num(id, point, room, date);
		if(select == 1){
			continue;
		}
		else
			return 0;
	}			
	return 0;
}

int res_time_num(char* id, int point, int room, int date){
	while(1){
		FILE *fp;
		fp = fopen("record.dat", "r+");

		printf("\n시간 예약. 운영 시간은 8시부터 22시까지입니다. 뒤로가기는 0을 입력하시오.\n");

		printf("\n예약 시작 시간: ");
		char input1[2];
		scanf("%s", input1);
		if(strcmp(input1,"0")==0)
			return 1;
		if(strcmp(input1,"8")!=0 && strcmp(input1,"9")!=0 && strcmp(input1,"10")!=0 && strcmp(input1,"11")!=0 && strcmp(input1,"12")!=0 && 
		   strcmp(input1,"13")!=0 && strcmp(input1,"14")!=0 && strcmp(input1,"15")!=0 && strcmp(input1,"16")!=0 && strcmp(input1,"17")!=0 &&
		   strcmp(input1,"18")!=0 && strcmp(input1,"19")!=0 && strcmp(input1,"20")!=0 && strcmp(input1,"21")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int start = atoi(input1);
		if(atoi(input1)<8 || atoi(input1)>21){
			printf("운영 시간은 8시부터 22시까지입니다. 다시 입력하시오.\n");
			continue;				
		}

		printf("\n사용 예정 시간: ");
		char input2[2];
		scanf("%s", input2);
		if(strcmp(input2,"0")==0)
			return 1;
		if(strcmp(input2,"1")!=0 && strcmp(input2,"2")!=0 && strcmp(input2,"3")!=0 && strcmp(input2,"4")!=0 && strcmp(input2,"5")!=0 && 
		   strcmp(input2,"6")!=0 && strcmp(input2,"7")!=0 && strcmp(input2,"8")!=0 && strcmp(input2,"9")!=0 && strcmp(input2,"10")!=0 &&
		   strcmp(input2,"11")!=0 && strcmp(input2,"12")!=0 && strcmp(input2,"13")!=0 && strcmp(input2,"14")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		if(start+atoi(input2)>22){
			printf("운영 시간은 8시부터 22시까지입니다. 다시 입력하시오.\n");
			continue;				
		}
		int time = atoi(input2);
		
		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);

		printf("\n사용 인원(가능 인원: %d명): ", mdata.points[point].people_num[room]);
		char input3[2];
		scanf("%s", input3);
		if(strcmp(input3,"0")==0)
			return 1;
		if(strcmp(input3,"1")!=0 && strcmp(input3,"2")!=0 && strcmp(input3,"3")!=0 && strcmp(input3,"4")!=0 && strcmp(input3,"5")!=0 && 
		   strcmp(input3,"6")!=0 && strcmp(input3,"7")!=0 && strcmp(input3,"8")!=0 && strcmp(input3,"9")!=0 && strcmp(input3,"10")!=0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		int people_num = atoi(input3);
		
		if(people_num<1 || mdata.points[point].people_num[room]<people_num){
			printf("사용 가능 인원을 초과했거나 잘못된 입력입니다.\n");
			continue;
		}

		RESERVE rdata, rdata2;
		strcpy(rdata.id, id);
		rdata.point_num = point;
		rdata.room_num = room;
		rdata.date = date;
		rdata.time = start;
		rdata.people_num = people_num;
		
		for(int i=0; i<mdata.reserve_num; i++){
			fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
			fread(&rdata2, 32, 1, fp);
			if(rdata.point_num==rdata2.point_num && rdata.room_num==rdata2.room_num && rdata.date==rdata2.date && rdata.time==rdata2.time){
				fclose(fp);
				printf("해당 시간의 공간은 이미 예약이 존재하여 예약이 불가합니다.\n");
				return 1;
			}
		}
		for(int i=0; i<time; i++){
			fseek(fp, 0, SEEK_END);
			fwrite(&rdata, 32, 1, fp);
			mdata.reserve_num++;
			rdata.time = rdata.time + 1;
		}
		fseek(fp, 0, SEEK_SET);
		fwrite(&mdata, MANAGE_SIZE, 1, fp);
		printf("예약 되었습니다.\n");
		fclose(fp);
		return 0;
	}			
	return 0;
}

int id_inq(char* id){
	while(1){
		printf("\n*************예약 조회 및 수정*************\n");
		FILE *fp;
		fp = fopen("record.dat", "r+");

		MANAGE mdata;
		fseek(fp, 0, SEEK_SET);
		fread(&mdata, MANAGE_SIZE, 1, fp);
					
		printf("번호 지점 공간 날짜      시간 인원\n");

		RESERVE rdata;
		int count = 1;
		for(int i=0; i<mdata.reserve_num; i++){
			fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
			fread(&rdata, 32, 1, fp);
			if(strcmp(rdata.id, id)==0){
				printf("%d    ", count);				
				printf("%d    ", rdata.point_num+1);
				printf("%d    ", rdata.room_num+1);				
				printf("%d    ", rdata.date);
				printf("%d    ", rdata.time);				
				printf("%d\n", rdata.people_num);
				count++;
			}
		}
		printf("수정 혹은 삭제할 번호를 선택하시오.\n");
		printf("(뒤로가기는 0을 입력하시오.)\n");
		printf("입력: ");
		char input[3];
		scanf("%s", input);	
		if(strcmp(input,"0")==0){
			fclose(fp);			
			return 1;
		}
		int num_right = 0;
		for(int i=0; i<count; i++){
			char num[3];
			sprintf(num, "%d", i);
			if(strcmp(input, num)==0)
				num_right = 1;		
		}
		if(num_right == 0){
			printf("잘못된 입력입니다. 다시 입력하시오.\n");
			fclose(fp);
			continue;
		}
		
		int sel_num = atoi(input);
		int sel_res;
		count = 1;
		for(int i=0; i<mdata.reserve_num; i++){
			fseek(fp, MANAGE_SIZE+i*32, SEEK_SET);
			fread(&rdata, 32, 1, fp);
			if(strcmp(rdata.id, id)==0){
				if(count == sel_num)
					sel_res=i;
				count++;
			}
		}
		fclose(fp);
	
		printf("삭제: 1 / 수정: 2 / 뒤로가기: 0\n");
		printf("입력: ");
		char input2[1];
		scanf("%s", input2);
		if(strcmp(input2,"1")==0){
			del_res(sel_res);
			printf("삭제되었습니다.\n");
			return 0;
		}
		else if(strcmp(input2,"2")==0){
			int select = inq_res(id);
			if(select == 1)
				continue;
			else{
				del_res(sel_res);
				return 0;
			}
		}
		else if(strcmp(input2,"0")==0){
			return 1;
		}
		else{
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}
		break;	
	}
	return 0;
}

int del_res(int num){

	FILE *fp;
	fp = fopen("record.dat", "r+");
	RESERVE rdata;
	fseek(fp, MANAGE_SIZE+num*32, SEEK_SET);
	fread(&rdata, 32, 1, fp);
	
	strcpy(rdata.id, "NULL");
	rdata.point_num = -1;
	rdata.room_num = -1;
	rdata.date = 0;
	rdata.time = 0;
	rdata.people_num = 0;

	fseek(fp, MANAGE_SIZE+num*32, SEEK_SET);
	fwrite(&rdata, 32, 1, fp);

	fclose(fp);

	return 0;
}


void main(){
	FILE *fp;
	if(!(fp = fopen("record.dat", "r+"))){
		fp = fopen("record.dat", "w+");
		
		MANAGE mdata;
		mdata.reserve_num = 0;
		for(int i=0; i<6; i++){
			mdata.points[i].point_exist = 0;
			for(int j=0; j<5; j++)
				mdata.points[i].people_num[j]=0;
		}

		fseek(fp, 0, SEEK_SET);
		fwrite(&mdata, MANAGE_SIZE, 1, fp);
		fclose(fp);
	}
	else{
		fclose(fp);
	}

	printf("**************스터디 공간 예약 프로그램**************\n");

	while(1){
		printf("\n모드를 선택하시오.\n");
		printf("(관리자 모드: 1 / 사용자 모드: 2 / 프로그램 종료: 3)\n");
		printf("입력: ");
		char input[10];
		scanf("%s", input);
		if(strcmp(input,"1")==0){
			manager_mode();
			continue;
		}
		else if(strcmp(input,"2")==0){
			user_mode();
			continue;
		}
		else if(strcmp(input,"3")==0){
			printf("프로그램을 종료합니다.\n");
			return;
		}
		else{
			printf("잘못된 입력입니다. 다시 입력하시오.\n\n");
			continue;
		}		
	}
}
