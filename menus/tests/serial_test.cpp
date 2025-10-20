#define _GNU_SOURCE                // enable BOTHER and struct termios2
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm-generic/termbits.h>
//#include <linux/termios.h>        // for struct termios2, TCGETS2, TCSETS2
#include <cstring>
#include <iostream>

int main() {
    const char* port = "/dev/ttyS0";
    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // 1. Fetch current settings
    struct termios2 tio2;
    if (ioctl(fd, TCGETS2, &tio2) < 0) {
        perror("ioctl TCGETS2");
        close(fd);
        return 1;
    }

    // 2. Configure raw mode
    tio2.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                     | INLCR  | IGNCR  | ICRNL  | IXON);
    tio2.c_oflag &= ~OPOST;
    tio2.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tio2.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
    tio2.c_cflag |= CS8 | CREAD | CLOCAL;

    // 3. Set VMIN/VTIME for blocking read of at least 1 byte
    tio2.c_cc[VMIN]  = 1;
    tio2.c_cc[VTIME] = 0;

    // 4. Select custom baud rate
    //    BOTHER tells the driver to use c_ispeed / c_ospeed directly
    tio2.c_cflag |= BOTHER;
    tio2.c_ispeed = 921600;   // e.g. 250k baud
    tio2.c_ospeed = 921600;

    // 5. Apply settings
    if (ioctl(fd, TCSETS2, &tio2) < 0) {
        perror("ioctl TCSETS2");
        close(fd);
        return 1;
    }

    printf("Listening on %s at 250000 baud (termios2)\n", port);

    // 6. Read loop
    char buf[10];
    for (int i=0; i <10; i++)
	    buf[i]=i;
    while (true) {
        int n = ::write(fd, buf, sizeof(buf));
        if (1) {
		for (int i=0; i < 100000; i++);

	//		std::cout << static_cast<uint8_t>(buf[i]) << "\t";
        }
    }

    close(fd);
    return 0;
}
