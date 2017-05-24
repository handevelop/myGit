/*
* my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다.
*
*/
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3

/*
* 기계어 목록 파일로 부터 정보를 받아와 생성하는 기계어 변환 테이블이다.
* 해당 기계어의 정보를 토큰으로 나눠 기록하고 있다.
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
* 어셈블리 할 소스코드를 토큰 단위로 관리하는 테이블이다.
* 관리 정보는 소스 라인 단위로 관리되어진다.
*/
char *input_data[MAX_LINES];
static int line_num;

int label_num;

struct token_unit {
	char *label;
	char *operator_;
	char *operand[MAX_OPERAND];
	char *comment;
	int extents; // 추가
};

typedef struct token_unit token;
token *token_table[MAX_LINES];
static int token_line;



/*
* 심볼을 관리하는 구조체이다.
* 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
*/
struct symbol_unit {
	char symbol[10];
	int addr;
	int extents; //추가
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

/*추가된 헤더 목록*/
int symbol_index; // 심볼테이블에 저장되어 있는 심볼의 인덱스
int op_tab[MAX_LINES];//OPCODE에 대해 관리해주는 테이블
int location_counter_index[MAX_LINES]; // 로케이션 카운터에 대한 인덱스를 처리

int form_list[MAX_LINES]; //형식에 대한 것을 관리해주는 리스트
int extents;

typedef struct literal_unit {
	char *literal;
	int addr;
	int extents;
}literal; //리터럴 구조체
int ltorg_line_sum;  // LTORG의 추가된 라인 개수
literal liter_tab[MAX_LINES]; // 리터럴 table
int liter_sum; // 리터럴 table의 있는 리터럴 개수
token symbol_token[MAX_LINES];//심볼 토큰
int search_symbol(char *str, int extents);
int search_placed_literal(char *str, int extents);
