#ifndef __SERIAL__
#define __SERIAL__

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/poll.h>

    /*
     * 'open_port()' - Open serial port 1.
     *
     * Returns the file descriptor on success or -1 on error.
     */

class serialport
{
	int fd;
	struct termios options;
	
	public:
	serialport();
    serialport(const char* port_adress, int baudrate);
    int init(const char* port_adress, int baudrate);
    int open(const char* port_adress);
	void set_rate(int baudrate);
	//void set_parity();
	int read( void* destination, unsigned bytesize);
	int write(void* data, unsigned bytesize);
	int close();
	int get_fd();
	int flush();
};

void swap_endian(char* pt, unsigned char byte_size);

#endif
