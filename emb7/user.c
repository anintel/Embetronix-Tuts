#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main(){

	int fd;
	char option;

	fd = open("/dev/code", O_RDWR);
	if(fd < 0) {
		printf("Cannot open device file..\n");
		return 0;
	}

	while(1) {

		printf("Enter a option \n1. Write \n2.Read \n3.Exit \n");
		scanf(" %c", &option);

		printf("you selected %c\n", option);

		switch(option){

			case '1' :
				printf("\n\nEnter the string to write into driver: ");
				scanf(" %[^\t\n]s", write_buf);
				printf("your data is getting written\n ");
				write(fd, write_buf, strlen(write_buf+1));
				printf("Data written!\n");
				break;
			case '2' :
				printf("Data reading ...\n");
				read(fd, read_buf, 1024);
				printf("reading is done!\n");
				printf("Data in LKM is - %s\n", read_buf);
				break;
			case '3' :
				close(fd);
				exit(1);
				break;
			default :
				printf("Please enter a valid option = %c \n", option);
				break;

		}

	}
	close(fd);
}

				
