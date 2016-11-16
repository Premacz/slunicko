///* File: interpret.c         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 10.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "interpret.h"
#include "instrList.h"
#include "generator.h"

void VStackInit(VariableStack* s, unsigned int size) {
	s->top = -1;
	s->max = size;
	s->data = malloc(sizeof(T_variable)*size);

	if (s->data == NULL)
	{
		fprintf(stderr, "Error init allock stack \n"); // TODO
		return;
	}
}

int VStackEmpty(VariableStack* s) {
	return (s->top == -1) ? 1 : 0;
}

int VStackFull(VariableStack* s) {
	return (s->top == s->max - 1) ? 1 : 0;
}

void VStackTop(VariableStack* s, int* c) {
	*/
	if (stackEmpty(s))
	{
		fprintf(stderr, "Error empty stack - call stackTop\n"); // TODO
		return;
	}
	else
	{
		*c = s->data[s->top];
		return;
	}
}


void VStackPop(VariableStack* s) {
	if (stackEmpty(s))
	{
		fprintf(stderr, "Error empty stack - call stackpop\n"); // TODO
		return;
	}
	else
	{
		//s->arr[s->top]=0; // todo
		s->top -= 1;
		return;
	}
}

T_variable VStackGet(VariableStack* s, int offset) {
	return s->data[offset];
}

void VStackSet(VariableStack* s, int offset, T_variable data) {
	s->data[offset] = data;
}

void VStackPush(VariableStack* s, T_variable data) {
	if (stackFull(s))
	{
		//stackError(SERR_PUSH);
		s->data = (int*)realloc(s->data, sizeof(int)*(s->max + s->max));
		if (s == NULL)
		{
			fprintf(stderr, "Error realloc stack \n"); // TODO
			return;
		}
		s->max += s->max;
	}

	s->top += 1;
	s->data[s->top] = data;
	return;
}

void VStackDelete_and_free(VariableStack* s)
{
	if (stackEmpty(s))
	{
		//stackError(SERR_PUSH);
		;
	}
	else
	{
		while (!stackEmpty(s))
		{
			stackPop(s);
		}
	}
	free(s->data);
	s->data = NULL;
	return;
}

//Provadi samotnou interpretaci predanych instrukci - pocetni operaca a instrukce skoku
int interpret(T_instr_list *L, VariableStack vStack) {
	bool ZeroFlag = false;
	int frame = 0;
	IntStack* offset_stack;
	stackInit(offset_stack, 99);
	T_address_code T = malloc(sizeof(T_address_code));
	T_address_code S = malloc(sizeof(T_address_code));
	T_variable A1 = malloc(sizeof(T_variable));
	T_variable A2 = malloc(sizeof(T_variable));
	T_variable A3 = malloc(sizeof(T_variable));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			return -1;
		}
		switch (T.operation) {

			//Instrukce pro aritmeticko operace
			case T_ADD:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int + A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int + A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double + A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double + A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == STRING && A2.type == STRING) {
					strLoad(A3)
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_INC:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					A1.value.value_int = A1.value.value_int + 1;
					ZeroFlag = !(A3.value.value_int == 0)
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.address1, A1);
				break;

			case T_SUB:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int - A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int - A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double - A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double - A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_DEC:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					A1.value.value_int = A1.value.value_int - 1;
					ZeroFlag = !(A3.value.value_int == 0)
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.address1, A1);
				break;

			case T_MUL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int * A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int * A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double * A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double * A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_DIV:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int / A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int / A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double / A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double / A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Logicke operace
			case T_AND:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int && A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double && A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool && A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_OR:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int || A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double || A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool || A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_NOT:
				A1 = VStackGet(vStack, frame + T.address1);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					A3.value.value_int = !(A1.value.value_int);
					ZeroFlag = !(A3.value.value_int == 0);
				}
				else if (A1.type == DOUBLE) {
					A3.value.value_double = !(A1.value.value_double);
					ZeroFlag = !(A3.value.value_double == 0);
				}
				else if (A1.type == BOOLEAN) {
					A3.value.value_bool = !(A1.value.value_bool);
					ZeroFlag = !(A3.value.value_bool == 0);
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Porovnavaci operace
			case T_EQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int == A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double == A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool == A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_LEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_MEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_NEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int != A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double != A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool != A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_MORE:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int > A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double > A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool > A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_LESS:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int < A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double < A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool < A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Vstup a v�stup
			case T_IN:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					scanf("%d", &(A1.value.value_int));
				}
				else if (A1.type == DOUBLE) {
					scanf("%f", &(A1.value.value_double));
				}
				else if (A1.type == BOOLEAN) {
					scanf("%b", &(A1.value.value_bool));
				}
				else if (A1.type == STRING) {
					while (int c = getchar()) {
						strAddChar(A1.value.value_String, c);
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.addresss1, A1);
				break;

			case T_OUT:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					printf("%d", A1.value.value_int);
				}
				else if (A1.type == DOUBLE) {
					printf("%f", A1.value.value_double);
				}
				else if (A1.type == BOOLEAN) {
					if (A1.value.value_bool) {
						printf("true");
					}
					else {
						printf("false");
					}
				}
				else if (A1.type == STRING) {
					strPrintStr(A1.value.value_String);
				}
				else {
					return -1;
				}
				break;

			/*Nejd��v mus� b�t v�echny aritmetick� a podobn� dal�� v�echny operace, �ek� se na strukturu z parseru*/

			//Skokove instrukce pro if a cykly
			case T_LABEL:
				break;

			case T_JMPZD:
				if (!ZeroFlag) {
					break;
				}

			case T_JMPD:
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_LABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				ZeroFlag = false;
				break;

			//Instrukce pro funkcnost uzivatelskych funkci
			case T_FSTART:
				frame = T->address1;
				stackPush(offset_stack, T->address1);
				break;

			case T_FJMP:
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_FLABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				A1 = VStackGet(vStack, frame + T->result);
				break;

			case T_FLABEL:
				stackPop(offset_stack);
				if (stackEmpty(offset_stack)) {
					frame = 0;
				}
				else {
					frame = stackGet(offset_stack);
				}
				VStackSet(vStack, frame + T->address1, A1);
				break;

			case default:
				return -1;

		}
		if (L->Active->next_item == NULL) {
			free(T);
			free(S);
			free(A1);
			free(A2);
			free(A3);
			return 0;
		}
		listNext(L);
	}
}