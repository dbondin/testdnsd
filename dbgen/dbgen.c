#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static void oneline();

int main(int argc, char ** argv) {
    long i;
    long num;
    if(argc < 2) {
	fprintf(stderr, "Number of lines required\n");
	return 1;
    }
    srand(time(NULL));
    num = atol(argv[1]);
    if(num <=0) {
	fprintf(stderr, "Bad number of lines specified\n");
	return 1;
    }
    for(i=0; i<num; i++) {
	oneline();
    }

    return 0;
}

/* static */
void oneline() {
    int i;
    int num = 2 + rand() % 3;
    int j;
    int size;
    for(i=0; i <= num; i++) {
	if(i != 0) {
	    putchar('.');
	}
	size = 2 + rand() % 5;
	for(j=0; j<size; j++) {
	    putchar('a' + rand() % ('z' - 'a' + 1));
	}
    }
    putchar(' ');
    for(i=0; i<4; i++) {
	if(i != 0) {
	    putchar('.');
	}
	printf("%d", 1 + rand() % 255);
    }
    putchar('\n');
}
