/*
* my_assembler �Լ��� ���� ���� ���� �� ��ũ�θ� ��� �ִ� ��� �����̴�.
*
*/
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3

/*
* ���� ��� ���Ϸ� ���� ������ �޾ƿ� �����ϴ� ���� ��ȯ ���̺��̴�.
* �ش� ������ ������ ��ū���� ���� ����ϰ� �ִ�.
*/
struct inst_struct {
	char *str;
	unsigned char op;
	int format;
	int ops;
};
typedef struct inst_struct inst_struct;
inst_struct *inst[MAX_INST];
int inst_index;

inst_struct inst_table[MAX_INST];
/*
* ����� �� �ҽ��ڵ带 ��ū ������ �����ϴ� ���̺��̴�.
* ���� ������ �ҽ� ���� ������ �����Ǿ�����.
*/
char *input_data[MAX_LINES];
static int line_num;

int label_num;

struct token_unit {
	char *label;
	char *operator_;
	char *operand[MAX_OPERAND];
	char *comment;
	int extents; // �߰�
};

typedef struct token_unit token;
token *token_table[MAX_LINES];
static int token_line;



/*
* �ɺ��� �����ϴ� ����ü�̴�.
* �ɺ� ���̺��� �ɺ� �̸�, �ɺ��� ��ġ�� �����ȴ�.
*/
struct symbol_unit {
	char symbol[10];
	int addr;
	int extents; //�߰�
};

typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];

static int locctr;
//--------------

static char *input_file;
static char *output_file;

int init_my_assembler(void);
static int assem_pass1(void);
static int assem_pass2(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int token_parsing(int index);
int search_opcode(char *str);
void make_objectcode(char *file_name);

/*�߰��� ��� ���*/
int symbol_index; // �ɺ����̺� ����Ǿ� �ִ� �ɺ��� �ε���
int op_tab[MAX_LINES];//OPCODE�� ���� �������ִ� ���̺�
int location_counter_index[MAX_LINES]; // �����̼� ī���Ϳ� ���� �ε����� ó��

int form_list[MAX_LINES]; //���Ŀ� ���� ���� �������ִ� ����Ʈ
int extents;

typedef struct literal_unit {
	char *literal;
	int addr;
	int extents;
}literal; //���ͷ� ����ü
int ltorg_line_sum;  // LTORG�� �߰��� ���� ����
literal liter_tab[MAX_LINES]; // ���ͷ� table
int liter_sum; // ���ͷ� table�� �ִ� ���ͷ� ����
token symbol_token[MAX_LINES];//�ɺ� ��ū
int search_symbol(char *str, int extents);
int search_placed_literal(char *str, int extents);
