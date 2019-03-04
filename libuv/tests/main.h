#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define RUN_TEST(func) \
	int main(int argc, char **argv) \
	{ \
		func(); \
		return 0; \
	}

#endif
