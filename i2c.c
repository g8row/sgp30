#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct identificator{
	int file_desc;
	__u8 dev_addr;
};

int i2c_open(int* file_desc, __u8 dev_addr, int adapter_nr){
	char filename[20];
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	*file_desc = open(filename, O_RDWR);
	if (*file_desc < 0) {
        	printf("Failed to open the i2c bus\n");
		return -1;
	}
  
	if(ioctl(*file_desc, I2C_SLAVE,dev_addr)<0) {
		printf("Failed to open the i2c bus]\n");
		return -1;
	}
	return 0;
}

int sgp30_iaq_init(int *file_desc){
	__u8 buf [2];
 	sleep(0.1);
  	buf[0]=0x20;
  	buf[1]=0x03;
	if(write(*file_desc, buf, 2) != 2){
		printf("failed to init sgp30\n");
		return -1;
	}
	return 0;
}

int sgp30_get_meas(int* file_desc, __u8* res, int res_len){	
	__u8 buf [2];
  	buf[0]=0x20;
  	buf[1]=0x08;
  	if(write(*file_desc,buf,2) != 2){
		printf("error writing to device\n");
		return -1;
	}
	sleep(1);
  	if(read(*file_desc,res,res_len) != res_len){
		printf("error reading from device\n");
		return -1;
	}
	return 0;
}

int main(){
	struct identificator id;
	id.dev_addr = 0x58;
	printf("%d\n",i2c_open(&id.file_desc,id.dev_addr,1));

	printf("%d\n",sgp30_iaq_init(&id.file_desc));

	for(int i=0;i<30;i++){
		__u8 buf[6];
		sgp30_get_meas(&id.file_desc, buf, 6);
		printf("%d CO2  %d TVOC\n", (buf[0]<<8)|buf[1], (buf[3]<<8)|buf[4]);
  	}
}
