#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <error.h>
#include <errno.h>


#define WR_VALUE _IOW('a', 'a', int32_t*)  // write operation (user to kernel space)
#define RD_VALUE _IOR('a', 'b', int32_t*)  // read operation (kernel to user space)

int main(){


    printf("The user program to call IOCTL functions \n");

    int fd;
    int32_t value, number;

    printf("Opening the Driver \n");

    fd = open("/dev/code", O_RDWR);

    if(fd<0) {

        printf("couldn't open the file. \n");
        printf("The reason is : %s\n", strerror(errno));
        return 0;
    }

    printf("Enter the number to write in the driver : ");
    scanf("%d", &number);

    printf("Writing the value into the driver\n");
    ioctl(fd, WR_VALUE, (int32_t*) &number); 

    printf("Reading the value from the driver\n");
    ioctl(fd, RD_VALUE, (int32_t*) &value);

    printf("The value in the driver is %d \n", value);

    printf("End of the program!\n");

    return 0;
}