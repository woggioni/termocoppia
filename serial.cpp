#include "serial.hpp"

void swap_endian(char* pt, unsigned char byte_size)
{
    char buffer;
    unsigned char i;
    for (i=0; i<byte_size/2; i++)
    {
        buffer=pt[i];
        pt[i]=pt[byte_size-i-1];
        pt[byte_size-i-1]=buffer;
    }
}

int serialport::open(const char* port_adress)
{
    fd = ::open(port_adress, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
    {		
		/*
		* Could not open the port.
		*/
        printf("Error for %s\n", port_adress);
		perror("open_port: Unable to open port - ");
	}
	else
	{
        fcntl(fd, F_SETFL, 0);
	}

	return (fd);
}

void serialport::set_rate(int baudrate)
{
	switch (baudrate)
	{
	case 9600:
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		break;
	case 57600:
		cfsetispeed(&options, B57600);
		cfsetospeed(&options, B57600);
		break;
	case 19200:
		cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
		break;
	default:
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
	}
}

int serialport::read( void* destination, unsigned bytesize)
{
	short n;
	if( (n = ::read(fd, destination, bytesize)) <0)
	{
		perror("Error: impossible to read from port - ");
	}
return n;
}

int serialport::write(void* data, unsigned bytesize)
{
	return ::write(fd, data, bytesize);
}

int serialport::close()
{
	if (::close(fd))
	{
		perror("serialport::close: unable to close the port - ");
		return -1;
	}
	else
	{
		return 0;
	}
}

serialport::serialport(){};

serialport::serialport(const char *port_adress, int baudrate)
{
	fd=open(port_adress);
	tcgetattr(fd, &options);
	set_rate(baudrate);
	
	/*
	* Enable the receiver and set local mode...
	*/
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
		
	options.c_cflag |= (CLOCAL | CREAD | CS8);
	
	// canonical mode
	//options.c_lflag |= (ICANON | ECHO | ECHOE);
	// raw mode
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    options.c_cc[VMIN]     = 2;
	/*
	* Set the new options for the port...
	*/
	tcsetattr(fd, TCSAFLUSH, &options);
}

int serialport::init(const char *port_adress, int baudrate)
{
    int res=open(port_adress);
    tcgetattr(fd, &options);
    set_rate(baudrate);

    /*
    * Enable the receiver and set local mode...
    */
options.c_cflag &= ~PARENB;
options.c_cflag &= ~CSTOPB;
options.c_cflag &= ~CSIZE;
//options.c_cflag |= CS8;

    //options.c_cflag |= CSTOPB;

    //options.c_cflag &= ~CSIZE;
    //options.c_cflag |= CS8;

    options.c_cflag |= (CLOCAL | CREAD | CS8);

    // canonical mode
    options.c_lflag |= (ICANON | ECHO | ECHOE);
    //options.c_lflag |= (ICANON );
    //raw mode
    //options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    //options.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    //options.c_cc[VMIN]     = 2;
    /*
    * Set the new options for the port...
    */
    tcsetattr(fd, TCSAFLUSH, &options);
    return res;
}

int serialport::get_fd()
{
	return fd;
}

int serialport::flush()
{
	if(tcflush(fd,TCIFLUSH))
	{
		perror("impossible to flush the port - ");
		return -1;
	}
	else
	{
		return 0;
	}
}
