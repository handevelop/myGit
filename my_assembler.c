/*
* ȭ�ϸ� : my_assembler.c
* ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
* �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
*
*/
/*
*
* ���α׷��� ����� �����Ѵ�.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* my_assembler ���α׷��� �������� �����͵��� ���ǵ� ���̴�.
*/

#include "my_assembler.h"


/* -----------------------------------------------------------------------------------
* ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
* �Ű� : ���� ����, ����� ����
* ��ȯ : ���� = 0, ���� = < 0
* ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
*		   ���� �߰������� �������� �ʴ´�.
* -----------------------------------------------------------------------------------
*/
int main(int argc, char *argv[]) {

	label_num = 0;
	if (init_my_assembler() < 0) {
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}
	if (assem_pass1() < 0) {
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	if (assem_pass2() < 0) {
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	output_file = "output.txt";
	make_objectcode(output_file);
	//getchar();
	//make_output("output") ; 
}
/* -----------------------------------------------------------------------------------
* ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
* �Ű� : ����
* ��ȯ : �������� = 0 , ���� �߻� = -1
* ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
*		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
*		   �����Ͽ���.
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
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/

static int assem_pass1(void) {
	int caculation_check = 0;
	int check_index = 0;
	int opcode_idx; // op �ε���
	int check_sum = 0;
	int tmp_token = 0;
	int tmp_location_counter_index = 0;
	token_table[check_index];
	locctr = 0; //�����̼� ī����
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
			if (strcmp(symbol_token[i].operator_, "RESW") == 0) { //RESERVED WORD�϶�
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				caculation_check++;
				locctr += 3 * atoi(symbol_token[i].operand[0]); // 1����� 3����Ʈ
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "RESB") == 0) {//RESERVED BYTE �϶� 
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				caculation_check++;
				locctr += atoi(symbol_token[i].operand[0]);
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "EXTDEF") == 0 //RESERVED WORD�� �ƴ� ���
				|| strcmp(symbol_token[i].operator_, "END") == 0
				|| strcmp(symbol_token[i].operator_, "LTORG") == 0
				|| strcmp(symbol_token[i].operator_, "EXTREF") == 0
				) {
				tmp_location_counter_index += 3;
				caculation_check++;
				location_counter_index[i] = -1; // �����̼� ī���Ͱ� ������
				check_sum++;
			}
			else if (strcmp(symbol_token[i].operator_, "CSECT") == 0) { //CSECT�϶�
				location_counter_index[i] = locctr;
				tmp_location_counter_index += 3;
				extents++;
				locctr = 0;
				check_sum++;
				caculation_check++;
			}
			else if (strcmp(symbol_token[i].operator_, "EQU") == 0) { //EQU�϶�
				location_counter_index[i] = locctr;
				if (symbol_token[i].operand[0][0] == '*');
				else {
					char *tmp;
					char *tmp_token;
					char op[2]; // +(�÷���),-(���̳ʽ�),*(���ϱ�),%(������),/(������) ���� ������ ����
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
			else if (strcmp(symbol_token[i].operator_, "*") == 0) { //���ͷ��� �ִ� *�� ó��
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
			if (search_symbol(symbol_token[i].label, extents) != 0) { //���̺��� �ɺ����̺� ������ �������ش�.
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
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/

static int assem_pass2(void) {
	int idx_inst; //���۷������� �ε����̴�.
				  
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
					//@�ϰ��� #�� ���
					if (symbol_token[i].operand[0][0] != '@' && symbol_token[i].operand[0][0] != '#') op_tab[i] |= 3; //������ 11 �߰�
					else if (symbol_token[i].operand[0][0] == '#') op_tab[i] |= 1;// 2���� 1�߰�
					else if (symbol_token[i].operand[0][0] == '@') op_tab[i] |= 2;//2�������� 10�� �߰����ش�.
					sum_tmp++;
				}
				else	op_tab[i] |= 3; //2�������� 11�� �߰����ش�.
				op_tab[i] <<= 1;// X�� ���� ���� ������ �Ҵ����ش�.
				sum_tmp++;
				inst_op_tab_index = 1;
				if (symbol_token[i].operand[1] != NULL) {
					if (symbol_token[i].operand[1][0] == 'X') op_tab[i] |= 1; //X�� ���� ������ 1�� �Ҵ��Ͽ� �ش�.
				}
				sum_tmp++;
				op_tab[i] <<= 1; //b�� ���� ���� ���� �Ҵ�
				sum_tmp++;
				inst_op_tab_index = 1;
				op_tab[i] |= 0; // 2�������� 0�� �߰��Ѵ�.
				sum_tmp++;
				inst_op_tab_index = 0;
				op_tab[i] <<= 2; // p,e�� ���� �Ҵ�
				inst_op_tab_index = 2;
				if (symbol_token[i].operand[0] != NULL&&symbol_token[i].operator_[0] != '+') { //format 3�϶�
					if (symbol_token[i].operand[0][0] == '@') { // indirect�϶�
						int ta = 0; // Ÿ�� ��巹��
						int disp = 0;
						int pc = 0; // Program Counter
						inst_op_tab_index = pc;
						op_tab[i] |= 2; //pe�� 2�������� 10�� �߰����ش�.
						sum_tmp++;
						op_tab[i] <<= 12; // disp�� �����Ҵ�
						sum_tmp++;
						pc = location_counter_index[i + 1]; // Program counter�� ���� �Ҵ�޾��ش�.
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
					else if (symbol_token[i].operand[0][0] == '#') { //immediate�϶�
						char *tmp = NULL;
						int imme;
						int index = 0;
						int shap = 1;
						op_tab[i] <<= 12; // disp�� ���� �Ҵ�
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
					else if (symbol_token[i].operand[0][0] == '=') { //���ͷ��϶�
						int disp = 0;
						int pc = 0; // Program Counter
						int ta = 0; // TARGET ADDRESS
						op_tab[i] |= 2; //pe�� 2������ 10�� �߰����ش�.
						sum_tmp++;
						op_tab[i] <<= 12; // disp�� �����Ҵ�
						sum_tmp++;
						pc = location_counter_index[i + 1]; // Program Counter�� ó�����ش�.
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
					else {//���۷��尡 simple addressing�϶�
						int disp = 0;
						int pc = 0; // Program Counter
						int ta = 0; // Ÿ�� ��巹��

						op_tab[i] |= 2; //pe�� 2���� 10�� �߰�
						sum_tmp++;
						inst_op_tab_index = 2;
						op_tab[i] <<= 12; // disp�� �����Ҵ�
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
				else if (symbol_token[i].operand[0] == NULL) { //���۷��尡 ���� ���
					op_tab[i] |= 0; //2������ 00�� �߰�
					inst_op_tab_index = 0;
					sum_tmp++;
					op_tab[i] <<= 12;
					inst_op_tab_index = 12;
					sum_tmp++;
				}
				else if (symbol_token[i].operator_[0] == '+') { //format4 �϶�
					int disp = 0;
					int pc = 0; // Program Counter
					int ta = 0; // Ÿ�� ��巹��
					op_tab[i] |= 1; // pe�� 1�� �����Ѵ�.
					inst_op_tab_index = 1;
					sum_tmp++;
					op_tab[i] <<= 20; // ������ �Ҵ�
					inst_op_tab_index = 20;
					sum_tmp++;
					pc = location_counter_index[i + 1]; // ���α׷� ī���� ���� �޴´�.
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
			default: //������
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
					if (symbol_token[i].operand[0][0] >= '0' && symbol_token[i].operand[0][0] <= '9') // ���ڶ��
						op_tab[i] = atoi(symbol_token[i].operand[0]);

					else { // �����϶��� ó��
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

	//TAB�� �����ش�.
	int tmp_tab = 0;
	for (int i = 0; i < token_line; i++) {
		if (location_counter_index[i] != -1 && symbol_token[i].operator_ != NULL&&strcmp(symbol_token[i].operator_, "CSECT") != 0) //CSECT�� �����̼�ī���� ǥ�� ����
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
* ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)��
*        �����ϴ� �Լ��̴�.
* �Ű� : ���� ��� ����
* ��ȯ : �������� = 0 , ���� < 0
* ���� : ���� ������� ������ ������ ����.
*
*	===============================================================================
*		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
*	===============================================================================
*
* -----------------------------------------------------------------------------------
*/

int init_inst_file(char *inst_file) {
	FILE *fp = NULL;
	int count = 0;//��ɾ��� ���ڸ� ����
	char c; // ���ڸ� �ϳ��� ����
	char tmp[100]; //�ӽ�
	int tmp_tab = 0;
	int tmp_enter = 0;
	int idx = 0;

	fp = fopen(inst_file, "r");

	if (fp == NULL) {
		printf("inst_file�� ���µ��� �����Ͽ����ϴ�.\n");
		return -1;
	}

	while ((c = fgetc(fp)) != EOF) {
		if (c != '\n') {
			tmp[idx++] = c;
			tmp_enter++;
		}
		else { //��ɾ �� �о�����
			if (tmp[0] == 0x00) // ���ο� ���๮�ڸ� �ִ� ��� skip
			{
				idx = 0;
				continue;
			}
			tmp[idx] = '\0';//���ڿ����� ó��

			inst[count] = (inst_struct *)calloc(1, sizeof(inst_struct));
			inst[count]->str = (char*)malloc(10);

			strcpy(inst[count]->str, strtok(tmp, "\t"));
			inst[count]->format = atoi(strtok(NULL, "\t"));
			inst[count]->op = (unsigned char)strtol(strtok(NULL, "\t"), NULL, 16);
			inst[count]->ops = atoi(strtok(NULL, "\t"));
			tmp_tab++;
			count++;
			inst_index++; //�Ѱ���
			idx = 0;

			tmp[0] = 0x00;
		}
	}

	if (idx > 0) // �������� ���๮�ڰ� ���� ��� ������ ó���� �Ѵ�
	{
		tmp[idx] = '\0';//���ڿ����� ó��

		inst[count] = (inst_struct *)malloc(sizeof(inst_struct));
		inst[count]->str = (char*)malloc(10);

		strcpy(inst[count]->str, strtok(tmp, "\t"));
		inst[count]->format = atoi(strtok(NULL, "\t"));
		inst[count]->op = (unsigned char)strtol(strtok(NULL, "\t"), NULL, 16);
		inst[count]->ops = atoi(strtok(NULL, "\t"));
		tmp_tab++;
		count++;
		inst_index++; //�Ѱ���
	}

	return 0;
}

/* -----------------------------------------------------------------------------------
* ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�.
* �Ű� : ������� �ҽ����ϸ�
* ��ȯ : �������� = 0 , ���� < 0
* ���� :
*
* -----------------------------------------------------------------------------------
*/


int init_input_file(char *input_file) {
	FILE *fp = NULL;
	char c;
	char tmp[200];
	int instruction_cnt = 0;// �ν�Ʈ���� ����
	int line_instruction_cnt = 0; // �Ѷ����� �ν�Ʈ���� ����
	int tmp_tab = 0;
	int tmp_enter = 0;
	fp = fopen(input_file, "r");
	if (fp == NULL) {
		printf("input_file�� ���µ��� �־� �����ϼ̽��ϴ�.\n");
		return -1;
	}
	while ((c = fgetc(fp)) != EOF) { //EOF���� ���� ����
		if (c == '\n') { //���๮�ڰ� ������ 
			if (tmp[0] == 0x00) // ���ο� ���๮�ڸ� �ִ� ��� skip
			{
				line_instruction_cnt = 0;
				continue;
			}

			tmp_enter++;
			tmp[line_instruction_cnt] = '\0'; // ���ڿ����� ó��
			line_instruction_cnt = 0;
			tmp_tab++;
			//input_data[instruction_cnt] = (char *)malloc(strlen(tmp));
			input_data[instruction_cnt] = (char *)malloc(strlen(tmp) + 1);
			strcpy(input_data[instruction_cnt], tmp);
			tmp_tab++;
			instruction_cnt++;

			tmp[0] = 0x00;
		}
		//���ڿ� ����
		else tmp[line_instruction_cnt++] = c;
		tmp_tab++;
	}

	if (line_instruction_cnt  > 0) // �������� ���๮�ڰ� ���� ��� ������ ó���� �Ѵ�
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
* ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
*        �н� 1�� ���� ȣ��ȴ�.
* �Ű� : �ҽ��ڵ��� ���ι�ȣ
* ��ȯ : �������� = 0 , ���� < 0
* ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
* -----------------------------------------------------------------------------------
*/

int token_parsing(int index) {
	int parsing_cnt = 0;
	if (index < 0) return -1;
	symbol_token[index + ltorg_line_sum].extents = extents;
	if (input_data[index][0] == '.') { //�ּ��϶�
		symbol_token[index + ltorg_line_sum].label = ".";
		symbol_token[index + ltorg_line_sum].operator_ = NULL;
		symbol_token[index + ltorg_line_sum].operand[0] = NULL;
		symbol_token[index + ltorg_line_sum].operand[1] = NULL;
		symbol_token[index + ltorg_line_sum].comment = "\0";
		parsing_cnt++;
		return 0;
	}
	else if (input_data[index][0] == '\t' || input_data[index][0] == ' ') { //���̺��� ������
		char *tmp;
		char *tmp_data = (char *)malloc(strlen(input_data[index]) + 1); // �ӽÿ����� ������ �Ҵ�
		char *tmp_token = NULL; // ��ū �ӽÿ�
		char *tmp_operand = NULL;
		parsing_cnt++;
		int tmp_parsing_tap = 0;
		strcpy(tmp_data, input_data[index]); //�ӽÿ뿡�ٰ� ���
		symbol_token[index + ltorg_line_sum].label = NULL;
		tmp_token = strtok(tmp_data, " \t");
		tmp_parsing_tap++;
		symbol_token[index + ltorg_line_sum].operator_ = (char *)malloc(strlen(tmp_token) + 1);// �ӽø�ŭ �����Ҵ�
		tmp_parsing_tap++;
		strcpy(symbol_token[index + ltorg_line_sum].operator_, tmp_token); // �ӽÿ��� �����Ѵ�.

		tmp_token = strtok(NULL, "\n"); // ���۷��带 ���� ���ڿ�

		if (tmp_token == NULL || tmp_token[0] == '\t' || tmp_token[0] == ' ') { //���۷��尡 ���°��
			parsing_cnt++;
			if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "LTORG") == 0) { //LTORG�� ó��
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

			if (tmp_token != NULL) { //�ӽÿ��� NULL�� �ƴҶ��� ó��
				char* tmp2_token = (char *)malloc(strlen(tmp_token) + 1);
				strcpy(tmp2_token, tmp_token);
				symbol_token[index + ltorg_line_sum].operand[0] = NULL;
				symbol_token[index + ltorg_line_sum].operand[1] = NULL;
				symbol_token[index + ltorg_line_sum].comment = strtok(tmp2_token, "\t");
				parsing_cnt++;
				return 0;
			}
		}
		else { //���۷��� �����Ѵٸ�
			tmp_operand = strtok(tmp_token, "\t");
			tmp = strtok(tmp_operand, ",");
			symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(tmp) + 1);
			strcpy(symbol_token[index + ltorg_line_sum].operand[0], tmp); // 1��° ���۷���

			if (symbol_token[index + ltorg_line_sum].operand[0][0] == '=') { //���ͷ� ���̺� ������ ���ش�.
				if (search_placed_literal(symbol_token[index + ltorg_line_sum].operand[0], extents) != 0) {
					liter_tab[liter_sum].literal = (char *)malloc(strlen(symbol_token[index + ltorg_line_sum].operand[0]));
					strcpy(liter_tab[liter_sum].literal, symbol_token[index + ltorg_line_sum].operand[0]);
					liter_tab[liter_sum++].extents = extents;
					parsing_cnt++;
				}
			}
			tmp = strtok(NULL, ",");
			if (tmp != NULL) { //���۷��尡 2��
				symbol_token[index + ltorg_line_sum].operand[1] = (char *)malloc(strlen(tmp) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].operand[1], tmp);
				tmp = strtok(NULL, ",");
				parsing_cnt++;
				if (tmp != NULL) { //���۷��尡 3��
					symbol_token[index + ltorg_line_sum].operand[2] = (char *)malloc(strlen(tmp) + 1);
					strcpy(symbol_token[index + ltorg_line_sum].operand[2], tmp);
					parsing_cnt++;
				}
			}
			else if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "END") == 0) { //������ ���ͷ��� ó��
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
	else { //���̺��� �ִ� ���
		char *tmp;
		char *tmp_data = (char *)malloc(strlen(input_data[index]) + 1); // �ӽÿ뿡 �����Ҵ�
		char *tmp_token = NULL; // �ӽÿ�
		char *tmp_operand = NULL;
		int parsing_cnt = 0;
		int tab_cnt = 0;
		strcpy(tmp_data, input_data[index]);//�ӽÿ뿡 ����
		tmp_token = strtok(tmp_data, " \t");
		tab_cnt++;
		symbol_token[index + ltorg_line_sum].label = (char *)malloc(strlen(tmp_token) + 1);//�ӽÿ뿡 �����Ҵ�
		parsing_cnt++;
		strcpy(symbol_token[index + ltorg_line_sum].label, tmp_token); // �ӽÿ뿡 ���

		tmp_token = strtok(NULL, " \t");
		tab_cnt++;
		symbol_token[index + ltorg_line_sum].operator_ = (char *)malloc(strlen(tmp_token) + 1);//�ӽÿ뿡 �����Ҵ�
		strcpy(symbol_token[index + ltorg_line_sum].operator_, tmp_token); // �ӽÿ뿡 ���
		parsing_cnt++;
		if (strcmp(symbol_token[index + ltorg_line_sum].operator_, "CSECT") == 0) extents++; //���� ����

		tmp_token = strtok(NULL, "\n");

		if (tmp_token == NULL || tmp_token[0] == '\t' || tmp_token[0] == ' ') { //���۷��尡 ���� ���
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
		else { //���۷��尡 �ִٸ�
			tmp_operand = strtok(tmp_token, "\t");
			tab_cnt++;
			tmp = strtok(tmp_operand, ",");
			parsing_cnt++;
			symbol_token[index + ltorg_line_sum].operand[0] = (char *)malloc(strlen(tmp) + 1);
			strcpy(symbol_token[index + ltorg_line_sum].operand[0], tmp); // ����° ���۷���
			if (symbol_token[index + ltorg_line_sum].operand[0][0] == '=') { // ���ͷ����� Ȯ��
				if (search_placed_literal(symbol_token[index + ltorg_line_sum].operand[0], extents) != 0) {
					liter_tab[liter_sum].literal = (char *)malloc(strlen(symbol_token[index + ltorg_line_sum].operand[0]));
					strcpy(liter_tab[liter_sum].literal, symbol_token[index + ltorg_line_sum].operand[0]);
					liter_tab[liter_sum++].extents = extents;
					parsing_cnt++;
				}
			}
			tmp = strtok(NULL, ",");
			if (tmp != NULL) { //�ι�° ���۷���
				symbol_token[index + ltorg_line_sum].operand[1] = (char *)malloc(strlen(tmp) + 1);
				strcpy(symbol_token[index + ltorg_line_sum].operand[1], tmp);
				tab_cnt++;
				tmp = strtok(NULL, ",");
				parsing_cnt++;
				if (tmp != NULL) { //����° ���۷���
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
* ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
* �Ű� : ��ū ������ ���е� ���ڿ�
* ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0
* ���� :
*
* -----------------------------------------------------------------------------------
*/

int search_opcode(char *str) {
	if (str[0] == '+') { //4������ ��
		str = str + 1; //�ϳ��� �÷��ش�.
	}

	for (int i = 0; i < inst_index; i++) {
		if (strcmp(str, inst[i]->str) == 0)
			return i;
	}
	return -1;
}

/* -----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/

void make_objectcode(char* file_name) {

	int program_end_line = 0; // program�� �������� ���� ��ȣ
	int program_start_line = 0; // ���α׷��� �����ҋ��� ���ι�ȣ
	int program_end_address = 0; // program�� ������ location counter
	int program_start_address = 0; // program�� �����ּ�
	int extends_position = 0;//������ extents����
	int is_line_new = 1; //������ �ٳ��� 1�� ó�����ش�.
	int is_line_end = 0;
	int current_char = 0; // ���� ������ char�� �������ش�.
	int tmp_data = 0; // �ּ�(address)�� ����ҋ� ���ִ� �ӽ��� ����
	int current = 0; // ���� ��ġ
	FILE *fp = NULL;
	char *tmp;
	char *buffer; // Buffer�� �����͸� ��´�.
	int buff_size = 0;
	int buff_length = 0;
	buffer = (char *)malloc(70); //���۸� 70���� �Ҵ����ش�.
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

			program_end_address = location_counter_index[tmp_data + 1]; // �������� PC�� ���̴�. (EQU�� ��������.)
		}
		else program_end_address = location_counter_index[tmp_data];

		fprintf(fp, "%06X\n", program_end_address - program_start_address); // program�� ���� ���
		for (int i = current; i <= program_end_line; i++) {
			if (symbol_token[i].operator_ == NULL)
				continue;
			else {
				if (strcmp(symbol_token[i].operator_, "EXTDEF") == 0) { //�ܺ������� ó��
					current = i;
					fputc('D', fp);
					current_char += 1;
					for (int j = 0; j < 3; j++) {
						if (symbol_token[i].operand[j] != NULL) {
							int sym_addr = 0;
							for (int k = 0; k < symbol_index; k++) { //�ɺ����̺��� ���۷��带 ��Ī�غ���.
								if (strcmp(sym_table[k].symbol, symbol_token[i].operand[j]) == 0 && sym_table[k].extents == symbol_token[i].extents)
									sym_addr = sym_table[k].addr;
							}
							current_char += (strlen(symbol_token[i].operand[j]) + 6);
							if (current_char>72) { //���Ѽ��� ������ �ʰ��Ѵٸ�
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
				if (strcmp(symbol_token[i].operator_, "EXTREF") == 0) { //�ܺ��������
					current = i;
					fputc('R', fp);
					current_char += 1;
					for (int j = 0; j < 3; j++) {
						if (symbol_token[i].operand[j] != NULL) {
							current_char += strlen(symbol_token[i].operand[j]) + 1;
							if (current_char>72) { //���ѱ��̸� ������
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
		int tmp_end = 0;// ���� ��ġ
		while (current <= program_end_line) { //���������� �ؽ�Ʈ ���
			if (is_line_new == 1) {
				sprintf(buffer, "T%06X", location_counter_index[current]);
				buff_size += 6;
				current_char += 7;
				strcat(buffer, "00"); //������ ������ �����Ѵ�. 0����
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
			if (strcmp(symbol_token[i].operator_, "EXTREF") == 0) { //�ܺ����� ó��
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

//�߰� �� �Լ� �κ�
//���̺��� �̹� ���� �Ǿ� �ִ����� �˻��ϴ� ���� �߰��� �Լ��̴�.
int search_symbol(char *str, int extents) { 
	for (int i = 0; i < symbol_index; i++) {
		if (strcmp(sym_table[i].symbol, str) == 0)
			if (extents == sym_table[i].extents) return 0;
	}
	return -1;
}
//���̺� ���ͷ��� ����Ǿ� �ִ��� Ȯ���ϴ� �Լ��̴�.
int search_placed_literal(char *str, int extents) { 
	for (int i = 0; i < liter_sum; i++) {
		if (strcmp(liter_tab[i].literal, str) == 0)
			if (extents == liter_tab[i].extents)return 0;
	}
	return -1;
}