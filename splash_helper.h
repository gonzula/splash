typedef enum {
    number,
    magicVariable,
    variable
} OpType;

typedef struct {
    OpType type;
    char *value;
    char *name;
    char uuid[37];
} Operand;


void append_operand(Operand **, OpType, const char *);
void append_operation(Operand **, char, Operand *, Operand *);
void append_minus_op(Operand **, Operand *);
void set_variable(const char *id, Operand *op);

void output_header(FILE *output);
void output_footer(FILE *output);
