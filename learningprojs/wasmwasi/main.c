
#include<stdio.h>
#include<stdlib.h>

typedef enum {
	INST_PUSH,
	INST_ADD,
	INST_PRINT
} Inst_Type;

typedef struct {
	Inst_Type type;
	int operand;
} Inst; // single instruction

Inst program[] = {
	{.type = INST_PUSH, .operand = 35},
	{.type = INST_PUSH, .operand = 34},
	{.type = INST_ADD},
	{.type = INST_PRINT}
};

int main() {

	return 0;
}
