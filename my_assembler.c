/*
* 화일명 : my_assembler.c
* 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
* 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
*
*/
/*
*
* 프로그램의 헤더를 정의한다.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* my_assembler 프로그램의 의존적인 데이터들이 정의된 곳이다.
*/

#include "my_assembler.h"


/* -----------------------------------------------------------------------------------
* 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
* 매계 : 실행 파일, 어셈블리 파일
* 반환 : 성공 = 0, 실패 = < 0
* 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다.
*		   또한 중간파일을 생성하지 않는다.
* -----------------------------------------------------------------------------------
*/
int main(int argc, char *argv[]) {

	label_num = 0;
	if (init_my_assembler() < 0) {
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}
	if (assem_pass1() < 0) {
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}
	if (assem_pass2() < 0) {
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}
	output_file = "output.txt";
	make_objectcode(output_file);
	//getchar();
	//make_output("output") ; 
}
/* -----------------------------------------------------------------------------------
* 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다.
* 매계 : 없음
* 반환 : 정상종료 = 0 , 에러 발생 = -1
* 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기
*		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
*		   구현하였다.
* -----------------------------------------------------------------------------------
*/

int init_my_assembler(void) {

	int result;

	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;

	return result;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/

static int assem_pass1(void) {
	int caculation_check = 0;
	int check_index = 0;
	int opcode_idx; // op 인덱스
	int check_sum = 0;
	int tmp_token = 0;
	int tmp_location_counter_index = 0;
	token_table[check_index];
	locctr = 0; //로케이션 카운터
	int extents = 0;
	for (int i = 0; i < line_num; i++) {
		if (token_parsing(i) < 0) return -1;
		tmp_token++;
	}
	token_line = line_num + ltorg_line_sum;
	check_index++;
	check_sum += check_index;
	token_table[check_index];
	for (int i = 0; i < token_line; i++) {
		if (symbol_token[i].operator_ == NULL) {
			location_counter_index[i] = -1;
			check_sum++;
			continue;
		}
		opcode_idx = search_opcode(symbol_token[i].operator_);

		if (opcode_idx < 0) {
			if (strcmp(symbol_token[i].operator_, "RESW") == 0) { //RESERVED WORD일때
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				caculation_check++;
				locctr += 3 * atoi(symbol_token[i].operand[0]); // 1워드당 3바이트
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "RESB") == 0) {//RESERVED BYTE 일때 
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				caculation_check++;
				locctr += atoi(symbol_token[i].operand[0]);
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "EXTDEF") == 0 //RESERVED WORD가 아닌 경우
				|| strcmp(symbol_token[i].operator_, "END") == 0
				|| strcmp(symbol_token[i].operator_, "LTORG") == 0
				|| strcmp(symbol_token[i].operator_, "EXTREF") == 0
				) {
				tmp_location_counter_index += 3;
				caculation_check++;
				location_counter_index[i] = -1; // 로케이션 카운터가 없을때
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "CSECT") == 0) { //CSECT일때
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				extents++;
				locctr = 0;
				check_sum++;
				caculation_check++;
			}
			else if (strcmp(symbol_token[i].operator_, "EQU") == 0) { //EQU일때
				location_counter_index[i] = locctr;
				if (symbol_token[i].operand[0][0] == '*');
				else {
					char *tmp;
					char *tmp_token;
					char op[2]; // +(플러스),-(마이너스),*(곱하기),%(나머지),/(나누기) 등의 연산자 저장
					int value_one;
					int value_two;
					check_sum++;
					caculation_check++;
					tmp = (char *)malloc(strlen(symbol_token[i].operand[0]) + 1);
					strcpy(tmp, symbol_token[i].operand[0]);
					for (int j = 0; symbol_token[i].operand[0][j] != '\0'; j++) {
						if (symbol_token[i].operand[0][j] == '/' ||
							symbol_token[i].operand[0][j] == '%' ||
							symbol_token[i].operand[0][j] == '*' ||
							symbol_token[i].operand[0][j] == '-' ||
							symbol_token[i].operand[0][j] == '+') {
							op[1] = '\0';
							caculation_check++;
							op[0] = symbol_token[i].operand[0][j];
							tmp_location_counter_index += 1;
							check_sum++;
						}
					}
					tmp_token = strtok(tmp, op);
					for (int k = 0; k < symbol_index; k++) {
						if (strcmp(tmp_token, sym_table[k].symbol) == 0)
							value_one = sym_table[k].addr;
						check_sum++;
						caculation_check++;
						tmp_location_counter_index += 1;
					}
					tmp_token = strtok(NULL, "\0");
					for (int k = 0; k < symbol_index; k++) {
						if (strcmp(tmp_token, sym_table[k].symbol) == 0)
							value_two = sym_table[k].addr;
						caculation_check++;
						check_sum++;
						tmp_location_counter_index += 1;
					}

					if (op[0] == '/')	locctr = value_one + value_two;
					else if (op[0] == '-') 	locctr = value_one - value_two;
					else if (op[0] == '%')	locctr = value_one * value_two;
					else if (op[0] == '*')	locctr = value_one % value_two;
					else if (op[0] == '+')	locctr = value_one / value_two;
					caculation_check++;
					token_table[check_index]->operand;
					tmp_location_counter_index += 1;
					location_counter_index[i] = locctr;
					check_sum++;
				}
			}

			else if (strcmp(symbol_token[i].operator_, "START") == 0) location_counter_index[i] = locctr;
			else if (strcmp(symbol_token[i].operator_, "*") == 0) { //리터럴에 있는 *의 처리
				location_counter_index[i] = locctr;
				if (symbol_token[i].operand[0][1] == 'C') {
					int ch_num = 0;
					caculation_check++;
					for (int j = 3; symbol_token[i].operand[0][j] != '\''; j++)
						ch_num++;
					tmp_location_counter_index += 1;
					locctr += ch_num;
					check_sum++;
				}
				else if (symbol_token[i].operand[0][1] == 'X') {
					int x_num = 0;
					caculation_check++;
					check_sum++;
					for (int j = 3; symbol_token[i].operand[0][j] != '\''; j++)
						x_num++;
					locctr += (x_num / 2);
					caculation_check++;
				}
				tmp_location_counter_index += 3;
				location_counter_index[i + 1] = locctr;
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "BYTE") == 0) {
				location_counter_index[i] = locctr;
				if (symbol_token[i].operand[0][0] == 'C') {
					strcmp(token_table[check_index]->operand[1], "C");
					caculation_check++;
					int ch_num = 0;
					check_sum++;
					for (int j = 2; symbol_token[i].operand[0][j] != '\''; j++)
						ch_num++;
					tmp_location_counter_index += 1;
					locctr += ch_num;
					caculation_check++;
					strcmp(token_table[check_index]->operand[1], "W");
				}
				else if (symbol_token[i].operand[0][0] == 'X') {
					int x_num = 0;
					for (int j = 2; symbol_token[i].operand[0][j] != '\''; j++)
						x_num++;
					tmp_location_counter_index += 1;
					caculation_check++;
					locctr += (x_num / 2);
					check_sum++;
				}
			}
			else if (strcmp(symbol_token[i].operator_, "WORD") == 0) {
				caculation_check++;
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 1;
				locctr += 3;
				check_sum++;
			}
		}
		else {
			location_counter_index[i] = locctr;
			tmp_location_counter_index += 3;
			if (inst[opcode_idx]->format == 1) 	locctr += 1; // 1format
			else if (inst[opcode_idx]->format == 2) locctr += 2; //2format
			else { //3,4 format
				if (strchr(symbol_token[i].operator_, '+') == NULL) locctr += 3; //3format
				else locctr += 4;
				caculation_check++;
				tmp_location_counter_index += 4;
				check_sum++;
			}
		}
		if (symbol_token[i].label != NULL) {
			if (search_symbol(symbol_token[i].label, extents) != 0) { //레이블이 심볼테이블에 없으면 저장해준다.
				strcpy(sym_table[symbol_index].symbol, symbol_token[i].label);
				caculation_check++;
				sym_table[symbol_index].addr = location_counter_index[i];
				tmp_location_counter_index += 3;
				caculation_check++;
				sym_table[symbol_index].extents = extents;
				tmp_location_counter_index += 3;
				symbol_index++;
				check_sum++;
			}
		}
	}
	for (int i = 0; i < liter_sum; i++) {
		for (int j = 0; j < token_line; j++) {
			caculation_check++;
			if (symbol_token[j].operand[0] != NULL && strcmp(symbol_token[j].operand[0], liter_tab[i].literal) == 0 && symbol_token[j].extents == liter_tab[i].extents && symbol_token[j].operator_[0] == '*')
				liter_tab[i].addr = location_counter_index[j];
			tmp_location_counter_index += 1;
			check_sum++;
			caculation_check++;
		}
	}
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/

static int assem_pass2(void) {
	int idx_inst; //오퍼레이터의 인덱스이다.
				  
	int inst_op_tab_index = 0;
	int sum_tmp = 0;
	for (int i = 0; i < token_line; i++) {
		if (symbol_token[i].operator_ == NULL) continue;
		idx_inst = search_opcode(symbol_token[i].operator_);
		{
			int inst_tab_form = 0;
			int format = 0;

			if (idx_inst >= 0)
				format = inst[idx_inst]->format;

			switch (format) {
			case 1:
				op_tab[i] = inst[idx_inst]->op;
				inst_op_tab_index = 1;
				form_list[i] = 1;
				inst_tab_form = 1;
				sum_tmp++;
				break;
			case 2:
				op_tab[i] = inst[idx_inst]->op;
				inst_op_tab_index = 4;
				op_tab[i] = op_tab[i] << 4;
				if (strcmp(symbol_token[i].operand[0], "SW") == 0) op_tab[i] |= 9;
				else if (strcmp(symbol_token[i].operand[0], "PC") == 0) op_tab[i] |= 8;
				else if (strcmp(symbol_token[i].operand[0], "F") == 0) op_tab[i] |= 6;
				else if (strcmp(symbol_token[i].operand[0], "T") == 0) op_tab[i] |= 5;
				else if (strcmp(symbol_token[i].operand[0], "S") == 0) op_tab[i] |= 4;
				else if (strcmp(symbol_token[i].operand[0], "B") == 0) op_tab[i] |= 3;
				else if (strcmp(symbol_token[i].operand[0], "L") == 0) op_tab[i] |= 2;
				else if (strcmp(symbol_token[i].operand[0], "X") == 0) op_tab[i] |= 1;
				else if (strcmp(symbol_token[i].operand[0], "A") == 0) op_tab[i] |= 0;
				op_tab[i] = op_tab[i] << 4;
				inst_op_tab_index = 4;
				if (symbol_token[i].operand[1] == NULL || strcmp(symbol_token[i].operand[1], "SW") == 0) op_tab[i] |= 0;
				else if (strcmp(symbol_token[i].operand[1], "PC") == 0) op_tab[i] |= 8;
				else if (strcmp(symbol_token[i].operand[1], "F") == 0) op_tab[i] |= 6;
				else if (strcmp(symbol_token[i].operand[1], "T") == 0) op_tab[i] |= 5;
				else if (strcmp(symbol_token[i].operand[1], "S") == 0) op_tab[i] |= 4;
				else if (strcmp(symbol_token[i].operand[1], "B") == 0) op_tab[i] |= 3;
				else if (strcmp(symbol_token[i].operand[1], "L") == 0) op_tab[i] |= 2;
				else if (strcmp(symbol_token[i].operand[1], "X") == 0) op_tab[i] |= 1;
				else if (strcmp(symbol_token[i].operand[1], "A") == 0) op_tab[i] |= 0;
				form_list[i] = 2;
				sum_tmp++;
				break;

			case 3:
				op_tab[i] = inst[idx_inst]->op;
				form_list[i] = 3;
				inst_op_tab_index = 3;
				sum_tmp++;
				if (symbol_token[i].operand[0] != NULL) {
					//@일경우와 #일 경우
					if (symbol_token[i].operand[0][0] != '@' && symbol_token[i].operand[0][0] != '#') op_tab[i] |= 3; //이진수 11 추가
					else if (symbol_token[i].operand[0][0] == '#') op_tab[i] |= 1;// 2진수 1추가
					else if (symbol_token[i].operand[0][0] == '@') op_tab[i] |= 2;//2진수에서 10을 추가해준다.
					sum_tmp++;
				}
				else	op_tab[i] |= 3; //2진수에서 11을 추가해준다.
				op_tab[i] <<= 1;// X가 들어가기 위한 공간을 할당해준다.
				sum_tmp++;
				inst_op_tab_index = 1;
				if (symbol_token[i].operand[1] != NULL) {
					if (symbol_token[i].operand[1][0] == 'X') op_tab[i] |= 1; //X에 대한 공간을 1로 할당하여 준다.
				}
				sum_tmp++;
				op_tab[i] <<= 1; //b가 들어가기 위한 공간 할당
				sum_tmp++;
				inst_op_tab_index = 1;
				op_tab[i] |= 0; // 2진수에서 0을 추가한다.
				sum_tmp++;
				inst_op_tab_index = 0;
				op_tab[i] <<= 2; // p,e의 공간 할당
				inst_op_tab_index = 2;
				if (symbol_token[i].operand[0] != NULL&&symbol_token[i].operator_[0] != '+') { //format 3일때
					if (symbol_token[i].operand[0][0] == '@') { // indirect일때
						int ta = 0; // 타겟 어드레스
						int disp = 0;
						int pc = 0; // Program Counter
						inst_op_tab_index = pc;
						op_tab[i] |= 2; //pe의 2진수에서 10을 추가해준다.
						sum_tmp++;
						op_tab[i] <<= 12; // disp의 공간할당
						sum_tmp++;
						pc = location_counter_index[i + 1]; // Program counter의 값을 할당받아준다.
						inst_op_tab_index = pc;
						for (int j = 0; j < symbol_index; j++) {
							if (symbol_token[i].operand[0] != NULL && symbol_token[i].extents == sym_table[j].extents && strcmp(sym_table[j].symbol, symbol_token[i].operand[0] + 1) == 0)
								ta = sym_table[j].addr;
							sum_tmp++;
							inst_op_tab_index = j;
						}
						disp = ta - pc;
						inst_op_tab_index = ta - pc;
						sum_tmp++;
						disp &= 0xfff;
						sum_tmp++;
						op_tab[i] |= disp;
						sum_tmp++;
					}
					else if (symbol_token[i].operand[0][0] == '#') { //immediate일때
						char *tmp = NULL;
						int imme;
						int index = 0;
						int shap = 1;
						op_tab[i] <<= 12; // disp의 공간 할당
						inst_op_tab_index = 12;
						sum_tmp++;
						tmp = (char *)malloc(strlen(symbol_token[i].operand[0]));
						for (int j = 0; j < (int)strlen(symbol_token[i].operand[0]) - 1; j++) {
							tmp[index++] = symbol_token[i].operand[0][j + 1];
							sum_tmp++;
							shap++;
						}
						tmp[index] = '\0';
						sum_tmp++;
						shap++;
						imme = strtol(tmp, NULL, 10);
						op_tab[i] |= imme;
						sum_tmp++;
						shap++;
						inst_op_tab_index++;
						free(tmp);
					}
					else if (symbol_token[i].operand[0][0] == '=') { //리터럴일때
						int disp = 0;
						int pc = 0; // Program Counter
						int ta = 0; // TARGET ADDRESS
						op_tab[i] |= 2; //pe에 2진수로 10을 추가해준다.
						sum_tmp++;
						op_tab[i] <<= 12; // disp의 공간할당
						sum_tmp++;
						pc = location_counter_index[i + 1]; // Program Counter를 처리해준다.
						for (int j = 0; j < liter_sum; j++) {
							if (strcmp(symbol_token[i].operand[0], liter_tab[j].literal) == 0 && symbol_token[i].extents == liter_tab[j].extents)
								ta = liter_tab[j].addr;
							inst_op_tab_index = j;
						}
						disp = ta - pc;
						sum_tmp++;
						disp &= 0xfff;
						sum_tmp++;
						op_tab[i] |= disp;
						sum_tmp++;
					}
					else {//오퍼랜드가 simple addressing일때
						int disp = 0;
						int pc = 0; // Program Counter
						int ta = 0; // 타켓 어드레스

						op_tab[i] |= 2; //pe에 2진수 10을 추가
						sum_tmp++;
						inst_op_tab_index = 2;
						op_tab[i] <<= 12; // disp의 공간할당
						sum_tmp++;
						inst_op_tab_index = 12;
						pc = location_counter_index[i + 1];
						for (int j = 0; j < symbol_index; j++) {
							if (symbol_token[i].operand[0] != NULL && symbol_token[i].extents == sym_table[j].extents && strcmp(sym_table[j].symbol, symbol_token[i].operand[0]) == 0)
								ta = sym_table[j].addr;
							sum_tmp++;
						}
						disp = ta - pc;
						inst_op_tab_index = disp;
						sum_tmp++;
						disp &= 0xfff;
						sum_tmp++;
						op_tab[i] |= disp;
						sum_tmp++;
					}
				}
				else if (symbol_token[i].operand[0] == NULL) { //오퍼랜드가 없는 경우
					op_tab[i] |= 0; //2진수로 00을 추가
					inst_op_tab_index = 0;
					sum_tmp++;
					op_tab[i] <<= 12;
					inst_op_tab_index = 12;
					sum_tmp++;
				}
				else if (symbol_token[i].operator_[0] == '+') { //format4 일때
					int disp = 0;
					int pc = 0; // Program Counter
					int ta = 0; // 타켓 어드레스
					op_tab[i] |= 1; // pe를 1로 저장한다.
					inst_op_tab_index = 1;
					sum_tmp++;
					op_tab[i] <<= 20; // 공간을 할당
					inst_op_tab_index = 20;
					sum_tmp++;
					pc = location_counter_index[i + 1]; // 프로그램 카운터 값을 받는다.
					for (int j = 0; j < symbol_index; j++) {
						if (symbol_token[i].operand[0] != NULL && symbol_token[i].extents == sym_table[j].extents && strcmp(sym_table[j].symbol, symbol_token[i].operand[0]) == 0) {
							ta = sym_table[j].addr;
							sum_tmp++;
						}
					}
					ta &= 0xfffff;
					sum_tmp++;
					op_tab[i] |= ta;
					inst_op_tab_index = ta;
					sum_tmp++;
					form_list[i] = 4;
					inst_op_tab_index = 4;
					sum_tmp++;
				}
				break;
			default: //끝낼때
				if (strcmp(symbol_token[i].operator_, "BYTE") == 0) {
					int count = 0;
					int index = 0;
					char * tmp = NULL;

					for (int j = 2; symbol_token[i].operand[0][j] != '\''; j++)
						count++;
					sum_tmp++;
					inst_op_tab_index = count;
					tmp = (char *)malloc(count + 1);
					inst_op_tab_index = count + 1;
					for (int j = 2; symbol_token[i].operand[0][j] != '\''; j++)
						tmp[index++] = symbol_token[i].operand[0][j];
					sum_tmp++;
					tmp[index] = '\0';
					sum_tmp++;
					op_tab[i] = strtol(tmp, NULL, 16);
					inst_op_tab_index = 16;
					free(tmp);
					sum_tmp++;
					form_list[i] = count / 2;
					inst_op_tab_index = count / 2;
				}
				else if (strcmp(symbol_token[i].operator_, "WORD") == 0) {
					form_list[i] = 3;
					if (symbol_token[i].operand[0][0] >= '0' && symbol_token[i].operand[0][0] <= '9') // 숫자라면
						op_tab[i] = atoi(symbol_token[i].operand[0]);

					else { // 문자일때의 처리
						for (int j = 0; j < token_line; j++) {
							sum_tmp++;
							if (symbol_token[j].operator_ != NULL&&strcmp(symbol_token[j].operator_, "EXTREF") == 0 && symbol_token[j].extents == symbol_token[i].extents) {
								for (int k = 0; k < 3; k++)
									if (symbol_token[j].operand[k] != NULL && strstr(symbol_token[i].operand[0], symbol_token[j].operand[k]) != NULL)
										continue;
								inst_op_tab_index++;
							}
						}
					}
				}
				else if (strcmp(symbol_token[i].operator_, "*") == 0) {
					if (symbol_token[i].operand[0][1] == 'C') {
						int ch_num = 0;
						inst_op_tab_index = ch_num;
						sum_tmp++;
						for (int j = 3; symbol_token[i].operand[0][j] != '\''; j++) {
							ch_num++;
							sum_tmp++;
							op_tab[i] |= symbol_token[i].operand[0][j];
							sum_tmp++;
							op_tab[i] <<= 8;
						}
						op_tab[i] >>= 8;
						sum_tmp++;
						form_list[i] = ch_num;
					}
					else if (symbol_token[i].operand[0][1] == 'X') {
						int count = 0;
						char *tmp;
						int index = 0;
						sum_tmp++;
						for (int j = 3; symbol_token[i].operand[0][j] != '\''; j++)
							count++;

						tmp = (char *)malloc(count + 1);
						for (int j = 3; symbol_token[i].operand[0][j] != '\''; j++)
							tmp[index++] = symbol_token[i].operand[0][j];
						tmp[index] = '\0';
						sum_tmp++;
						op_tab[i] = strtol(tmp, NULL, 16);
						sum_tmp++;
						form_list[i] = count / 2;
						inst_op_tab_index = count / 2;
						free(tmp);
					}
				}
				break;
			}
		}
	}

	//TAB을 보여준다.
	int tmp_tab = 0;
	for (int i = 0; i < token_line; i++) {
		if (location_counter_index[i] != -1 && symbol_token[i].operator_ != NULL&&strcmp(symbol_token[i].operator_, "CSECT") != 0) //CSECT는 로케이션카운터 표시 안함
			printf("%04X", location_counter_index[i]);
		printf("\t");
		tmp_tab++;
		if (symbol_token[i].label != NULL)	printf("%s", symbol_token[i].label);
		printf("\t");
		tmp_tab++;
		if (symbol_token[i].operator_ != NULL)  printf("%s", symbol_token[i].operator_);
		printf("\t");
		tmp_tab++;
		if (symbol_token[i].operand[0] != NULL) {
			printf("%s", symbol_token[i].operand[0]);
			tmp_tab++;
			if (symbol_token[i].operand[1] != NULL) {
				printf(",%s", symbol_token[i].operand[1]);
				tmp_tab++;
				if (symbol_token[i].operand[2] != NULL)
					printf(",%s", symbol_token[i].operand[2]);
			}
		}
		if (form_list[i] == 1) printf("\t\t%02X\n", op_tab[i]);
		else if (form_list[i] == 2) printf("\t\t%04X\n", op_tab[i]);
		else if (form_list[i] == 3) printf("\t\t%06X\n", op_tab[i]);
		else if (form_list[i] == 4) printf("\t\t%08X\n", op_tab[i]);
		else printf("\n");
		tmp_tab++;
		tmp_tab++;
	}

	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을
*        생성하는 함수이다.
* 매계 : 기계어 목록 파일
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : 기계어 목록파일 형식은 다음과 같다.
*
*	===============================================================================
*		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
*	===============================================================================
*
* -----------------------------------------------------------------------------------
*/

int init_inst_file(char *inst_file) {
	FILE *fp = NULL;
	int count = 0;//명령어의 숫자를 누적
	char c; // 문자를 하나씩 저장
	char tmp[100]; //임시
	int tmp_tab = 0;
	int tmp_enter = 0;
	int idx = 0;

	fp = fopen(inst_file, "r");

	if (fp == NULL) {
		printf("inst_file를 여는데에 실패하였습니다.\n");
		return -1;
	}

	while ((c = fgetc(fp)) != EOF) {
		if (c != '\n') {
			tmp[idx++] = c;
			tmp_enter++;
		}
		else { //명령어를 다 읽었으면
			if (tmp[0] == 0x00) // 라인에 개행문자만 있는 경우 skip
			{
				idx = 0;
				continue;
			}
			tmp[idx] = '\0';//문자열로의 처리

			inst[count] = (inst_struct *)calloc(1, sizeof(inst_struct));
			inst[count]->str = (char*)malloc(10);

			strcpy(inst[count]->str, strtok(tmp, "\t"));
			inst[count]->format = atoi(strtok(NULL, "\t"));
			inst[count]->op = (unsigned char)strtol(strtok(NULL, "\t"), NULL, 16);
			inst[count]->ops = atoi(strtok(NULL, "\t"));
			tmp_tab++;
			count++;
			inst_index++; //총개수
			idx = 0;

			tmp[0] = 0x00;
		}
	}

	if (idx > 0) // 마지막에 개행문자가 없는 경우 데이터 처리를 한다
	{
		tmp[idx] = '\0';//문자열로의 처리

		inst[count] = (inst_struct *)malloc(sizeof(inst_struct));
		inst[count]->str = (char*)malloc(10);

		strcpy(inst[count]->str, strtok(tmp, "\t"));
		inst[count]->format = atoi(strtok(NULL, "\t"));
		inst[count]->op = (unsigned char)strtol(strtok(NULL, "\t"), NULL, 16);
		inst[count]->ops = atoi(strtok(NULL, "\t"));
		tmp_tab++;
		count++;
		inst_index++; //총개수
	}

	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다.
* 매계 : 어셈블리할 소스파일명
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/


int init_input_file(char *input_file) {
	FILE *fp = NULL;
	char c;
	char tmp[200];
	int instruction_cnt = 0;// 인스트럭션 개수
	int line_instruction_cnt = 0; // 한라인의 인스트럭션 개수
	int tmp_tab = 0;
	int tmp_enter = 0;
	fp = fopen(input_file, "r");
	if (fp == NULL) {
		printf("input_file을 여는데에 있어 실패하셨습니다.\n");
		return -1;
	}
	while ((c = fgetc(fp)) != EOF) { //EOF까지 파일 읽음
		if (c == '\n') { //개행문자가 나오면 
			if (tmp[0] == 0x00) // 라인에 개행문자만 있는 경우 skip
			{
				line_instruction_cnt = 0;
				continue;
			}

			tmp_enter++;
			tmp[line_instruction_cnt] = '\0'; // 문자열로의 처리
			line_instruction_cnt = 0;
			tmp_tab++;
			//input_data[instruction_cnt] = (char *)malloc(strlen(tmp));
			input_data[instruction_cnt] = (char *)malloc(strlen(tmp) + 1);
			strcpy(input_data[instruction_cnt], tmp);
			tmp_tab++;
			instruction_cnt++;

			tmp[0] = 0x00;
		}
		//문자열 저장
		else tmp[line_instruction_cnt++] = c;
		tmp_tab++;
	}

	if (line_instruction_cnt  > 0) // 마지막에 개행문자가 없는 경우 데이터 처리를 한다
	{
		tmp[line_instruction_cnt] = '\0';
		tmp_tab++;
		input_data[instruction_cnt] = (char *)malloc(strlen(tmp) + 1);
		strcpy(input_data[instruction_cnt], tmp);
		instruction_cnt++;
	}

	line_num = instruction_cnt;
	tmp_tab++;

	fclose(fp);
	return 0;
}


/* -----------------------------------------------------------------------------------
* 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다.
*        패스 1로 부터 호출된다.
* 매계 : 소스코드의 라인번호
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다.
* -----------------------------------------------------------------------------------
*/

int token_parsing(int index) {
	int parsing_cnt = 0;
	if (index < 0) return -1;
	symbol_token[index + ltorg_line_sum].extents = extents;
	if (input_data[index][0] == '.') { //주석일때
		symbol_token[index + ltorg_line_sum].label = ".";
		symbol_token[index + ltorg_line_sum].operator_ = NULL;
		symbol_token[index + ltorg_line_sum].operand[0] = NULL;
		symbol_token[index + ltorg_line_sum].operand[1] = NULL;
		symbol_token[index + ltorg_line_sum].comment = "\0";
		parsing_cnt++;
		return 0;
	}
	else if (input_data[index][0] == '\t' || input_data[index][0] == ' ') { //레이블이 없으면
		char *tmp;
		char *tmp_data = (char *)malloc(strlen(input_data[index]) + 1); // 임시용으로 동적을 할당
		char *tmp_token = NULL; // 토큰 임시용
		char *tmp_operand = NULL;
		parsing_cnt++;
		int tmp_parsing_tap = 0;
		strcpy(tmp_data, input_data[index]); //임시용에다가 백업
		symbol_token[index + ltorg_line_sum].label = NULL;
		tmp_token = strtok(tmp_data, " \t");
		tmp_parsing_tap++;
		symbol_token[index + ltorg_line_sum].operator_ = (char *)malloc(strlen(tmp_token) + 1);// 임시만큼 동적할당
		tmp_parsing_tap++;
		strcpy(symbol_token[index + ltorg_line_sum].operator_, tmp_token); // 임시용을 복사한다.

		tmp_token = strtok(NULL, "\n"); // 오퍼렌드를 없앤 문자열

		if (tmp_token == NULL || tmp_token[0] == '\t' || tmp_token[0] == ' ') { //오퍼랜드가 없는경우
			parsing_cnt++;
			if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "LTORG") == 0) { //LTORG면 처리
				for (int i = 0; i < liter_sum; i++) {
					if (liter_tab[i].extents == symbol_token[index + ltorg_line_sum].extents) {
						symbol_token[index + (++ltorg_line_sum)].operator_ = "*";
						symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(liter_tab[i].literal) + 1);
						strcpy(symbol_token[index + ltorg_line_sum].operand[0], liter_tab[i].literal);
						parsing_cnt++;
						symbol_token[index + ltorg_line_sum].extents = extents;
					}
				}
			}

			if (tmp_token != NULL) { //임시용이 NULL이 아닐때의 처리
				char* tmp2_token = (char *)malloc(strlen(tmp_token) + 1);
				strcpy(tmp2_token, tmp_token);
				symbol_token[index + ltorg_line_sum].operand[0] = NULL;
				symbol_token[index + ltorg_line_sum].operand[1] = NULL;
				symbol_token[index + ltorg_line_sum].comment = strtok(tmp2_token, "\t");
				parsing_cnt++;
				return 0;
			}
		}
		else { //오퍼랜드 존재한다면
			tmp_operand = strtok(tmp_token, "\t");
			tmp = strtok(tmp_operand, ",");
			symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(tmp) + 1);
			strcpy(symbol_token[index + ltorg_line_sum].operand[0], tmp); // 1번째 오퍼랜드

			if (symbol_token[index + ltorg_line_sum].operand[0][0] == '=') { //리터럴 테이블에 저장을 해준다.
				if (search_placed_literal(symbol_token[index + ltorg_line_sum].operand[0], extents) != 0) {
					liter_tab[liter_sum].literal = (char *)malloc(strlen(symbol_token[index + ltorg_line_sum].operand[0]));
					strcpy(liter_tab[liter_sum].literal, symbol_token[index + ltorg_line_sum].operand[0]);
					liter_tab[liter_sum++].extents = extents;
					parsing_cnt++;
				}
			}
			tmp = strtok(NULL, ",");
			if (tmp != NULL) { //오퍼랜드가 2개
				symbol_token[index + ltorg_line_sum].operand[1] = (char *)malloc(strlen(tmp) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].operand[1], tmp);
				tmp = strtok(NULL, ",");
				parsing_cnt++;
				if (tmp != NULL) { //오퍼랜드가 3개
					symbol_token[index + ltorg_line_sum].operand[2] = (char *)malloc(strlen(tmp) + 1);
					strcpy(symbol_token[index + ltorg_line_sum].operand[2], tmp);
					parsing_cnt++;
				}
			}
			else if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "END") == 0) { //끝날때 리터럴을 처리
				for (int i = 0; i < liter_sum; i++) {
					if (liter_tab[i].extents == symbol_token[index + ltorg_line_sum].extents) {
						symbol_token[index + (++ltorg_line_sum)].operator_ = "*";
						symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(liter_tab[i].literal) + 1);
						strcpy(symbol_token[index + ltorg_line_sum].operand[0], liter_tab[i].literal);
						symbol_token[index + ltorg_line_sum].extents = extents;
						parsing_cnt++;
					}
				}
			}
			free(tmp_data);
			tmp_data = (char *)malloc(strlen(input_data[index]) + 1);
			strcpy(tmp_data, input_data[index]);
			strtok(tmp_data, " \t");
			strtok(NULL, "\t");
			parsing_cnt++;
			tmp_token = strtok(NULL, "\t");
			if (tmp_token != NULL) {
				symbol_token[index + ltorg_line_sum].comment = (char *)malloc(strlen(tmp_token) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].comment, tmp_token);
			}
			parsing_cnt++;
			return 0;
		}
	}
	else { //레이블이 있는 경우
		char *tmp;
		char *tmp_data = (char *)malloc(strlen(input_data[index]) + 1); // 임시용에 동적할당
		char *tmp_token = NULL; // 임시용
		char *tmp_operand = NULL;
		int parsing_cnt = 0;
		int tab_cnt = 0;
		strcpy(tmp_data, input_data[index]);//임시용에 복사
		tmp_token = strtok(tmp_data, " \t");
		tab_cnt++;
		symbol_token[index + ltorg_line_sum].label = (char *)malloc(strlen(tmp_token) + 1);//임시용에 동적할당
		parsing_cnt++;
		strcpy(symbol_token[index + ltorg_line_sum].label, tmp_token); // 임시용에 백업

		tmp_token = strtok(NULL, " \t");
		tab_cnt++;
		symbol_token[index + ltorg_line_sum].operator_ = (char *)malloc(strlen(tmp_token) + 1);//임시용에 동적할당
		strcpy(symbol_token[index + ltorg_line_sum].operator_, tmp_token); // 임시용에 백업
		parsing_cnt++;
		if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "CSECT") == 0) extents++; //영역 구분

		tmp_token = strtok(NULL, "\n");

		if (tmp_token == NULL || tmp_token[0] == '\t' || tmp_token[0] == ' ') { //오퍼랜드가 없는 경우
			if (tmp_token != NULL) {
				tab_cnt++;
				char* tmp2_token = (char *)malloc(strlen(tmp_token) + 1);
				strcpy(tmp2_token, tmp_token);
				symbol_token[index + ltorg_line_sum].operand[0] = NULL;
				tab_cnt++;
				symbol_token[index + ltorg_line_sum].operand[1] = NULL;
				symbol_token[index + ltorg_line_sum].comment = strtok(tmp2_token, "\t");
				parsing_cnt++;
				return 0;
			}
		}
		else { //오퍼랜드가 있다면
			tmp_operand = strtok(tmp_token, "\t");
			tab_cnt++;
			tmp = strtok(tmp_operand, ",");
			parsing_cnt++;
			symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(tmp) + 1);
			strcpy(symbol_token[index + ltorg_line_sum].operand[0], tmp); // 쳣번째 오퍼랜드
			if (symbol_token[index + ltorg_line_sum].operand[0][0] == '=') { // 리터럴인지 확인
				if (search_placed_literal(symbol_token[index + ltorg_line_sum].operand[0], extents) != 0) {
					liter_tab[liter_sum].literal = (char *)malloc(strlen(symbol_token[index + ltorg_line_sum].operand[0]));
					strcpy(liter_tab[liter_sum].literal, symbol_token[index + ltorg_line_sum].operand[0]);
					liter_tab[liter_sum++].extents = extents;
					parsing_cnt++;
				}
			}
			tmp = strtok(NULL, ",");
			if (tmp != NULL) { //두번째 오퍼랜드
				symbol_token[index + ltorg_line_sum].operand[1] = (char *)malloc(strlen(tmp) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].operand[1], tmp);
				tab_cnt++;
				tmp = strtok(NULL, ",");
				parsing_cnt++;
				if (tmp != NULL) { //세번째 오퍼랜드
					symbol_token[index + ltorg_line_sum].operand[2] = (char *)malloc(strlen(tmp) + 1);
					strcpy(symbol_token[index + ltorg_line_sum].operand[2], tmp);
				}
			}
			free(tmp_data);
			tmp_data = (char *)malloc(strlen(input_data[index]) + 1);
			strcpy(tmp_data, input_data[index]);
			tmp_token = strtok(tmp_data, " \t");
			tab_cnt++;
			tmp_token = strtok(NULL, " \t");
			tab_cnt++;
			tmp_token = strtok(NULL, "\t");
			tab_cnt++;
			tmp_token = strtok(NULL, "\t");
			tab_cnt++;
			if (tmp_token != NULL) {
				symbol_token[index + ltorg_line_sum].comment = (char *)malloc(strlen(tmp_token) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].comment, tmp_token);
			}
		}
		label_num++;
	}
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다.
* 매계 : 토큰 단위로 구분된 문자열
* 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/

int search_opcode(char *str) {
	if (str[0] == '+') { //4형식일 때
		str = str + 1; //하나를 늘려준다.
	}

	for (int i = 0; i < inst_index; i++) {
		if (strcmp(str, inst[i]->str) == 0)
			return i;
	}
	return -1;
}

/* -----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/

void make_objectcode(char* file_name) {

	int program_end_line = 0; // program이 끝날때의 라인 번호
	int program_start_line = 0; // 프로그램의 시작할떄의 라인번호
	int program_end_address = 0; // program의 마지막 location counter
	int program_start_address = 0; // program의 시작주소
	int extends_position = 0;//현재의 extents부위
	int is_line_new = 1; //라인이 바끼면 1로 처리해준다.
	int is_line_end = 0;
	int current_char = 0; // 현재 라인의 char를 저장해준다.
	int tmp_data = 0; // 주소(address)를 계산할떄 써주는 임시의 라인
	int current = 0; // 현재 위치
	FILE *fp = NULL;
	char *tmp;
	char *buffer; // Buffer에 데이터를 담는다.
	int buff_size = 0;
	int buff_length = 0;
	buffer = (char *)malloc(70); //버퍼를 70정도 할당해준다.
	buff_length = 70;
	tmp = (char *)malloc(7);
	fp = fopen(file_name, "w");
	while (is_line_end == 0) {
		while (symbol_token[current].label != NULL&&symbol_token[current].label[0] == '.') {
			current++;
		}
		fprintf(fp, "H%s\t", symbol_token[current].label); // program str
		if (symbol_token[current].operand[0] != NULL) program_start_address = atoi(symbol_token[current].operand[0]);
		fprintf(fp, "%06X", program_start_address); // start address
		for (int i = current + 1; i < token_line; i++) {
			if (symbol_token[i].operator_ == NULL)
				continue;
			if (strcmp(symbol_token[i].operator_, "CSECT") == 0)
				break;

			else if (strcmp(symbol_token[i].operator_, "END") == 0) {
				int literal_count = 0;
				for (int j = 0; j < liter_sum; j++) {
					if (symbol_token[i].extents == liter_tab[j].extents) literal_count++;
				}
				program_end_line = i + literal_count + 1;
				break;
			}
			program_end_line = i;
		}
		if (program_end_line == token_line) is_line_end = 1;
		tmp_data = program_end_line;
		if (program_end_line != token_line) {
			while (strcmp(symbol_token[tmp_data].operator_, "EQU") == 0)
				tmp_data--;

			program_end_address = location_counter_index[tmp_data + 1]; // 마지막의 PC의 값이다. (EQU를 제외했음.)
		}
		else program_end_address = location_counter_index[tmp_data];

		fprintf(fp, "%06X\n", program_end_address - program_start_address); // program의 길이 계산
		for (int i = current; i <= program_end_line; i++) {
			if (symbol_token[i].operator_ == NULL)
				continue;
			else {
				if (strcmp(symbol_token[i].operator_, "EXTDEF") == 0) { //외부정의의 처리
					current = i;
					fputc('D', fp);
					current_char += 1;
					for (int j = 0; j < 3; j++) {
						if (symbol_token[i].operand[j] != NULL) {
							int sym_addr = 0;
							for (int k = 0; k < symbol_index; k++) { //심볼테이블에서 오퍼랜드를 서칭해본다.
								if (strcmp(sym_table[k].symbol, symbol_token[i].operand[j]) == 0 && sym_table[k].extents == symbol_token[i].extents)
									sym_addr = sym_table[k].addr;
							}
							current_char += (strlen(symbol_token[i].operand[j]) + 6);
							if (current_char>72) { //제한숫자 범위를 초과한다면
								fputs("\nD", fp);
								current_char = 0;
								current_char += (strlen(symbol_token[i].operand[j]) + 6);
							}
							fprintf(fp, "%s%06X", symbol_token[i].operand[j], sym_addr);
						}
					}
					fputc('\n', fp);
					current_char = 0;
				}
				if (strcmp(symbol_token[i].operator_, "EXTREF") == 0) { //외부참조라면
					current = i;
					fputc('R', fp);
					current_char += 1;
					for (int j = 0; j < 3; j++) {
						if (symbol_token[i].operand[j] != NULL) {
							current_char += strlen(symbol_token[i].operand[j]) + 1;
							if (current_char>72) { //제한길이를 넘으면
								fputs("\nR", fp);
								current_char = 0;
								current_char += (strlen(symbol_token[i].operand[j]) + 1);
							}
							fprintf(fp, "%s", symbol_token[i].operand[j]);
						}
					}
					fputc('\n', fp);
					current_char = 0;
				}
			}
		}
		current++;
		program_start_line = current;
		int tmp_end = 0;// 끝의 위치
		while (current <= program_end_line) { //끝날떄까지 텍스트 출력
			if (is_line_new == 1) {
				sprintf(buffer, "T%06X", location_counter_index[current]);
				buff_size += 6;
				current_char += 7;
				strcat(buffer, "00"); //글자의 개수를 설정한다. 0개로
				buff_size += 2;
				current_char += 2;
				is_line_new = 0;
			}
			if (form_list[current] == 1) {
				current_char += 2;
				if (current_char > 69) {
					current_char -= 2;
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;

					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					current_char = 0;
					sprintf(buffer, "T%06X", location_counter_index[current]);
					buff_size += 7;
					current_char += 7;
					strcat(buffer, "00");
					buff_size += 2;
					current_char += 2;
					sprintf(tmp, "%02X", op_tab[current++]);
					buff_size += 2;
					strcat(buffer, tmp);
					current_char += 2;
				}
				else if (form_list[current + 1] <= 0 && symbol_token[current + 1].operator_ != NULL && strcmp(symbol_token[current + 1].operator_, "END") != 0) {
					sprintf(tmp, "%02X", op_tab[current++]);
					buff_size += 2;
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					buff_size++;
					fputc('\n', fp);
					current_char = 0;
					is_line_new = 1;
				}
				else {
					sprintf(tmp, "%02X", op_tab[current++]);
					buff_size += 2;
					strcat(buffer, tmp);
				}
			}
			else if (form_list[current] == 2) {
				current_char += 4;
				buff_length += 4;
				if (current_char > 69) {
					current_char -= 4;
					//					sprintf(tmp, "%02X", (current_char - 9) / 2) + 1;
					sprintf(tmp, "%02X", (current_char - 9) / 2 + 1);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					current_char = 0;
					sprintf(buffer, "T%06X", location_counter_index[current]);
					buff_size += 7;
					current_char += 7;
					strcat(buffer, "00");
					buff_size += 2;
					current_char += 2;
					sprintf(tmp, "%04X", op_tab[current++]);
					buff_size += 4;
					strcat(buffer, tmp);
					current_char += 4;
				}
				else if (form_list[current + 1] <= 0 && symbol_token[current + 1].operator_ != NULL && strcmp(symbol_token[current + 1].operator_, "END") != 0) {
					sprintf(tmp, "%04X", op_tab[current++]);
					buff_size += 2;
					strcat(buffer, tmp);
					//sprintf(tmp, "%02X", (current_char - 9) / 2) + 1;
					sprintf(tmp, "%02X", (current_char - 9) / 2 + 1);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					buff_size++;
					fputc('\n', fp);
					current_char = 0;
					is_line_new = 1;
				}
				else {
					sprintf(tmp, "%04X", op_tab[current++]);
					buff_size += 4;
					strcat(buffer, tmp);
				}
			}
			else if (form_list[current] == 3) {
				current_char += 6;
				if (current_char > 69) {
					current_char -= 6;
					buff_size -= 6;
					sprintf(tmp, "%02X", (current_char - 8) / 2);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					buff_size++;
					fputc('\n', fp);
					current_char = 0;
					sprintf(buffer, "T%06X", location_counter_index[current]);
					buff_size += 7;
					current_char += 7;
					buff_size += 2;
					strcat(buffer, "00");
					current_char += 2;
					buff_size += 2;
					sprintf(tmp, "%06X", op_tab[current++]);
					buff_size += 6;
					strcat(buffer, tmp);
					current_char += 6;
				}
				else if (form_list[current + 1] <= 0 && symbol_token[current + 1].operator_ != NULL && strcmp(symbol_token[current + 1].operator_, "END") != 0) {
					sprintf(tmp, "%06X", op_tab[current++]);
					buff_size += 6;
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					buff_size++;
					fputs(buffer, fp);
					buff_size++;
					fputc('\n', fp);
					current_char = 0;
					is_line_new = 1;
				}
				else {
					sprintf(tmp, "%06X", op_tab[current++]);
					buff_size += 7;
					strcat(buffer, tmp);
				}
			}
			else if (form_list[current] == 4) {
				current_char += 8;
				buff_size += 8;
				if (current_char > 69) {
					current_char -= 8;
					buff_size -= 8;
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					buff_size++;
					fputs(buffer, fp);
					fputc('\n', fp);
					current_char = 0;
					buff_size += 7;
					sprintf(buffer, "T%06X", location_counter_index[current]);
					current_char += 7;
					strcat(buffer, "00");
					buff_size += 2;
					current_char += 2;
					sprintf(tmp, "%08X", op_tab[current++]);
					strcat(buffer, tmp);
					buff_size += 8;;
					current_char += 8;

				}
				else if (form_list[current + 1] <= 0 && symbol_token[current + 1].operator_ != NULL && strcmp(symbol_token[current + 1].operator_, "END") != 0) {
					sprintf(tmp, "%08X", op_tab[current++]);
					buff_size += 8;
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					buff_size++;
					fputc('\n', fp);
					current_char = 0;
				}
				else {
					buff_size += 8;
					sprintf(tmp, "%08X", op_tab[current++]);
					strcat(buffer, tmp);
				}
			}
			else {
				if (symbol_token[current].operator_ != NULL &&strcmp(symbol_token[current].operator_, "END") == 0)
					tmp_end = current;
				else if (symbol_token[current - 2].operator_ != NULL &&strcmp(symbol_token[tmp_end].operator_, "END") == 0 && symbol_token[current].operator_ == NULL) {
					buff_size++;
					sprintf(tmp, "%02X", (current_char - 9) / 2);
					buff_size++;
					buffer[7] = tmp[0];
					buff_size++;
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
				}
				else {
					//buff_size;
					is_line_new = 1;
					current_char = 0;
				}
				current++;
			}
		}
		current--;
		for (int i = program_start_line - 1; i <= program_end_line; i++) {
			if (strcmp(symbol_token[i].operator_, "EXTREF") == 0) { //외부참조 처리
				char* index = NULL;
				if (symbol_token[i].operand[0] != NULL) {
					for (int j = program_start_line; j <= program_end_line; j++) {
						if (symbol_token[j].operand[0] != NULL) {
							if ((index = strstr(symbol_token[j].operand[0], symbol_token[i].operand[0])) != NULL) {
								if (search_opcode(symbol_token[j].operator_) >= 0)	fprintf(fp, "M%06X05+%s\n", location_counter_index[j] + 1, symbol_token[i].operand[0]);
								else {
									if ((index - 1)[0] == '-') fprintf(fp, "M%06X06-%s\n", location_counter_index[j], symbol_token[i].operand[0]);
									else fprintf(fp, "M%06X06+%s\n", location_counter_index[j], symbol_token[i].operand[0]);
									buff_size++;
								}
							}
						}
					}
				}
				if (symbol_token[i].operand[1] != NULL) {
					for (int j = program_start_line; j <= program_end_line; j++) {
						if (symbol_token[j].operand[0] != NULL) {
							if ((index = strstr(symbol_token[j].operand[0], symbol_token[i].operand[1])) != NULL) {
								if (search_opcode(symbol_token[j].operator_) >= 0)	fprintf(fp, "M%06X05+%s\n", location_counter_index[j] + 1, symbol_token[i].operand[1]);
								else {
									if ((index - 1)[0] == '-') fprintf(fp, "M%06X06-%s\n", location_counter_index[j], symbol_token[i].operand[1]);
									else fprintf(fp, "M%06X06+%s\n", location_counter_index[j], symbol_token[i].operand[1]);
								}
							}
						}
					}
				}
				if (symbol_token[i].operand[2] != NULL) {
					for (int j = program_start_line; j <= program_end_line; j++) {
						if (symbol_token[j].operand[0] != NULL) {
							if ((index = strstr(symbol_token[j].operand[0], symbol_token[i].operand[2])) != NULL) {
								if (search_opcode(symbol_token[j].operator_) >= 0)	fprintf(fp, "M%06X05+%s\n", location_counter_index[j] + 1, symbol_token[i].operand[2]);
								else {
									if ((index - 1)[0] == '-') fprintf(fp, "M%06X06-%s\n", location_counter_index[j], symbol_token[i].operand[2]);
									else fprintf(fp, "M%06X06+%s\n", location_counter_index[j], symbol_token[i].operand[2]);
								}
							}
						}
					}
				}
				break;
			}
		}
		if (extends_position == 0) fprintf(fp, "E%06X\n\n", location_counter_index[program_start_line]);
		else fputs("E\n\n", fp);
		extends_position++;
		current++;
	}
	fclose(fp);
}

//추가 된 함수 부분
//레이블이 이미 저장 되어 있는지를 검사하는 새로 추가된 함수이다.
int search_symbol(char *str, int extents) { 
	for (int i = 0; i < symbol_index; i++) {
		if (strcmp(sym_table[i].symbol, str) == 0)
			if (extents == sym_table[i].extents) return 0;
	}
	return -1;
}
//테이블에 리터럴이 저장되어 있는지 확인하는 함수이다.
int search_placed_literal(char *str, int extents) { 
	for (int i = 0; i < liter_sum; i++) {
		if (strcmp(liter_tab[i].literal, str) == 0)
			if (extents == liter_tab[i].extents)return 0;
	}
	return -1;
}