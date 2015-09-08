#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// #define DEBUG

int main(int argc, char *argv[])
{
	if(argc<2) {
		// TODO: add outfile cmdline option handling
		printf("usage: ov-to-unix infile [outfile]\n");
		return 0;
	}

	int fin;
	fin = open(argv[1], O_RDONLY);
	if(fin == -1) {
		printf("failed to open %s: %s\n", argv[1], strerror(errno)); 	
		return 1;
	}

	int lineno = 1;
	do {
		int linelen;
		ssize_t status;
		off_t pos;
		#define MAXLINELEN 256
		char line[MAXLINELEN+1];
		unsigned char ll0, ll1;

		status = read(fin, (void *)&ll0, 1);
		if(status == 0) // eof
			break;

		if(ll0 == 0) // throw this, might be word padding
			continue;

		// ll0 contain real line length, need to throw(?) next byte
		// is there any > 256 byte line?
		status = read(fin, (void *)&ll1, 1);
		if(status == 0) // eof
			break;
		linelen = (unsigned int)ll1*256 + ll0;
		
		if(linelen == 0)
			continue;

		if(linelen > MAXLINELEN) {
			pos = lseek(fin, 0, SEEK_CUR);
			printf("error at line %d, current file offset %ld, line length %04xh > %d\n", 
				lineno, pos, linelen, MAXLINELEN);
			//break;
			continue;
		}

		status = read(fin, (void *)line, linelen);
		if(status == 0)
			break;

		line[linelen] = 0;
#ifdef DEBUG
		printf("%03d %s\n", lineno, line);
#else
		printf("%s\n", line);
#endif
		lineno++;
	} while(1);

	close(fin);
	return 0;
}	
