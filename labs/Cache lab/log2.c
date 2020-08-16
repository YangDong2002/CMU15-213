#include<math.h>
#include<assert.h>
#include<stdio.h>
int main() {
	for(int i = 0; i < 32; ++i)
		if((int)(log2(1<<i) + 0.5) != i)
			printf("log2(1<<%d) = %d\n", i, (int)(log2(1<<i) + 0.5));
}
