#include     <stdio.h>  
#include     <stdlib.h>   
#include     <unistd.h>    
#include     <sys/types.h>  
#include     <sys/stat.h>  
#include     <fcntl.h>   
#include     <termios.h>  
#include     <errno.h>  

//data buffer
char read_buf[256];  
char write_buf[256] = "lukuan shi shabi AAA"; 

//init serial 
int serial_init(char *serial){
    int fd;         
    struct termios opt;   
      
    fd = open(serial, O_RDWR|O_NOCTTY|O_NDELAY);  
    if(fd == -1)  
    {  
        perror("open serial 0\n");  
        exit(0);  
    }  
  
    tcgetattr(fd, &opt);        
    bzero(&opt, sizeof(opt));  
      
    tcflush(fd, TCIOFLUSH);  
  
    cfsetispeed(&opt, B115200);  
    cfsetospeed(&opt, B115200);  
      
    opt.c_cflag &= ~CSIZE;    
    opt.c_cflag |= CS8;     
    opt.c_cflag &= ~CSTOPB;   
    opt.c_cflag &= ~PARENB;   
    opt.c_cflag &= ~CRTSCTS;  
    opt.c_cflag |= (CLOCAL | CREAD);  
   
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
   
    opt.c_oflag &= ~OPOST;  
      
    opt.c_cc[VTIME] = 0;  
    opt.c_cc[VMIN] = 0;  
      
    tcflush(fd, TCIOFLUSH);  
   
    printf("configure complete\n");  
      
    if(tcsetattr(fd, TCSANOW, &opt) != 0)  
    {  
        perror("serial error");  
        return -1;  
    } 

    return fd;
}

//return the len of the data
int  serial_read(int fd){
    int len = 0;
    int n = 0;

    while( (n = read(fd, read_buf, sizeof(read_buf))) > 0 )  
    {             
            len += n;  
    }

    read_buf[len] = '\0';
    return len;
}

void serial_write(int fd, int len){
    int n;
    int size=0;
    while(len>0){
        n = write(fd, write_buf, len);  
        len -= n;
        size += n;
    }
    printf("write %d chars\n",size);  

}

int main()  
{  
    int fd;  
    int i;  
    int len;  
    int n = 0;        
      
    fd = serial_init("/dev/ttyS0");
  
    printf("start send and receive data\n");  
  
    // while(1)
    // {      
        n = 0;  
        len = 0;  

        // memset(read_buf,sizeof(read_buf),0);
        // memset(write_buf,sizeof(read_buf),0);

        len = serial_read(fd);
                
        printf("Len: %d \n", len);  
        printf("%s \n", read_buf);  
   
        serial_write(fd,len);
          
        sleep(2);  
    // }  
    
    return 0;
}  