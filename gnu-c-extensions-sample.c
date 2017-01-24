#include <stdio.h>

/* Some GNU C Extension sample. */

/* Now you don't have to quote macros with "do {" and "} while (0)", just use "({"
 * and "})". And the last expression in "({" and "})" is the value of the entire
 * macro - yes, you can "return" a value in macro!
 */
#define swap(a, b) \
	/* Use __auto_type as a replacement of C++'s auto in C. */ \
	({ __auto_type t = (a); (a) = (b); (b) = t; })

#define max(a, b)			\
	({ /* To evaluate argument once - avoid side affect. */ \
	 __auto_type _a = (a);	\
	 __auto_type _b = (b);	\
	 /* The last expression is what macro quoted in "({" and "})" returns. */ \
	 _a > _b ? _a : _b;		\
	 })

#define lenof(array) \
	/* Don't forget ";". */ \
	({ sizeof(array) / sizeof(*array); })

/* Locally Declared Labels
 * which allow labels visible only in the block in which it is declared.
 * If a macro contains nested loops, a goto can be useful for breaking out of them.
 * However, if the macro can be expanded several times in one function,
 * the label is multiply defined in that function. local labels is useful in this case.
 */
#define search(array, len, key)		\
({									\
	/* declare local label here */	\
	__label__ found;				\
 	int i = 0;						\
	for (; i < len; i++)			\
		if (array[i] == key)		\
			goto found;				\
	i = -1;							\
/* The label itself is here. */		\
found:								\
	/* "return" the index */		\
	i;								\
})

/* Unlike __VA_ARGS__, this GNU C Extension gives variable arguments a name,
 * let it more readable. Note '##' is to avoid error when passing no arguments.
 */
#define print_error(fmt, args...) fprintf(stderr, fmt, ## args)

/* Help compiler to optimize branches. Just google it... */
#define likely(exp)		__builtin_expect(!!(exp), 1)
#define unlikely(exp)	__builtin_expect(!!(exp), 0)

int main()
{
	/* Nested function defining is allowed! If you want to
	 * call a nested function out the function where it is
	 * defined, keep the "father" function not exited.
	 */
	void nested_func() {
		puts("I'm in nested function.");
		float x = 3.142, y = 2.718;
		swap(x, y);
		printf("x: %g, y: %g\n", x, y);
	}
	/* x and y is not visible outside the nested function. */
	/* Call the nested function. */
	nested_func();

	int array[] = {2, 6, 8, 2, 7, 6, 5, 6, 6, 8};
	printf("Searching... ");
	printf("%d ", search(array, lenof(array), 2));
	printf("%d\n", search(array, lenof(array), 7));

	int sth = 5;
	switch (sth) {
		/* Use "a ... b" to express a range [a, b].
		 * Note no space around "..." will throw an error.
		 */
		case 1 ... 5:
			printf("%d in [1, 5]\n", sth);
			break;
		case 6 ... 10:
			printf("%d in [6, 10]\n", sth);
	}
	if (likely(max(12, 34) == 34))
		print_error("Everything goes well!");

	return 0;
}
