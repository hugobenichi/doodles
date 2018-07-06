#include <stdio.h>
#include <unistd.h>

int myexp(int x, int p) {
	if (p < 1) {
		return 1;
	}
	if (p == 1) {
		return x;
	}
	int y = 2 * myexp(x, p / 2);
	if ((p & 1) == 1) {
		y += x;
	}
	return y;
}

int mul2(int a, int b) {
	if (b == 0) {
		return 0;
	}
	int ab = a;
	int c = 1;
	while (c < b) {
		ab = ab + ab;
		c = c + c;
	}
	while (c - b) {
		ab -= a;
		c--;
	}
	return ab;
}

int main(int argc, char **args) {
	for (int a = 0; a <= 10; a++) {
		char *sep = "";
		for (int b = 0; b <= 10; b++) {
			int ab = mul2(a, b);
			fprintf(stdout, "%s%d", sep, ab);
			sep = "\t";
		}
		puts("");
	}
}
