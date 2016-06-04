#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGIT 4
#define FND_DEVICE "/dev/fpga_fnd"
#define MY_STRING "[js/sw] : "

int str_size;
unsigned char data[4];
unsigned char data2[4];
unsigned char temp[4];
unsigned char temp2[4];
unsigned char ans[4];
unsigned char operator;
unsigned char retval;
int n1, n2;
int answer;

void init_member(){
	memset(data, 0, sizeof(data));
	memset(data2, 0, sizeof(data2));
	memset(temp, 0, sizeof(temp));
	memset(temp2, 0, sizeof(temp2));
	memset(ans, 0, sizeof(ans));
}

int main(int argc, char * argv[]){
	int dev;
	int i;
	
	init_member(); // init

	int re_input = 0; // 1이면 재입력 로직

	printf(MY_STRING "argc : %d\n",argc);

	if(argc != 4){
		printf(MY_STRING "Please Input the right parameter! \n");
		printf(MY_STRING "ex) %s -> 1 + 2\n", argv[0]);
		return -1;
	}
	do{
		if(re_input == 0){
			str_size = strlen(argv[1]);
			operator = argv[2][0];
			if(str_size > MAX_DIGIT){
				printf(MY_STRING "Warning! 4 Digit number only\n");
				str_size = MAX_DIGIT;
			}

			for(i=0; i<str_size; i++){
				data[i] = argv[1][i];
				temp[i] = argv[1][i];
				data2[i] = argv[3][i];
				temp2[i] = argv[3][i];
			}
		}else{
			init_member(); // init
			scanf("%s", data);
			str_size = strlen(data);
			strcpy(temp, data);
			scanf("%c", &operator);
			getchar();
			sacnf("%s", data2);
			strcpy(temp2, data2);
		}

		n1 = atoi(temp);
		n2 = atoi(temp2);
		if(operator=='+')
		{
			sprintf(data,"%04d",(atoi(data)+atoi(data2)));
			answer = n1 + n2;
		}
		else if(operator=='-')
		{
			sprintf(data,"%04d",(atoi(data)-atoi(data2)));
			answer = n1 - n2;
		}
		else if(operator=='x')
		{
			sprintf(data,"%04d",(atoi(data)*atoi(data2)));
			answer = n1 * n2;
		}
		else if(operator=='/')
		{
			sprintf(data,"%04d",(atoi(data)/atoi(data2)));
			answer = n1 / n2;
		}
		else if(operator=='%')
		{
			sprintf(data,"%04d",(atoi(data)%atoi(data2)));
			answer = n1 % n2;
		}
		else
		{
			printf("OPERATOR ERROR!\n");
			return -1;
		}
		if(answer <0 || answer > 9999){
			re_input = 1; // 범위가 넘어가는 경우에 재 입력 작업
		}else{
			re_input = 0; // 범위가 넘어가지 않으면 재입력 작업 하지 않는다
		}
		strcpy(ans,data);
		for(i=0; i<str_size; i++){
			if((argv[1][i] < 0x30) || (argv[1][i] > 0x39) ){
				printf(MY_STRING "Error! Invalid Value!\n");
				return -1;
			}
			data[i] = data[i] - 0x30;
		}
	}while(re_input);
	
	

	dev = open(FND_DEVICE, O_RDWR);
	if(dev < 0){
		printf(MY_STRING "Device open error! : %s\n", FND_DEVICE);
		exit(1);
	}

	retval = write(dev, &data, 4);

	if(retval <0){
		printf(MY_STRING "Write Error!\n");
		return -1;
	}

	memset(data, 0, sizeof(data));
	memset(data2, 0, sizeof(data2));
	sleep(1);
	retval = read(dev, &data, 4);

	if(retval < 0){
		printf(MY_STRING "Read Error!\n");
		return -1;
	}

	printf(MY_STRING "Current FND Value : ");
	printf("%d %c %d = %d\n",n1,operator,n2,answer);
	close(dev);
	return 0;
	
}
