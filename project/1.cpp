#include <stdio.h>

int main() {
	int size1=16;
	int t=(size1/8+((size1%8)?1:0))*(size1/2);
	printf("%d",t);
	return 0;
}
