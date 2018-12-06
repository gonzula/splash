typedef enum {
    number,
    variable
} OpType;

typedef struct {
    OpType type;
    char *value;
} Operand;

void append_operand(Operand **, OpType, const char *);
void append_operation(Operand **, char, Operand *, Operand *);

void output_header(FILE *output);
void output_footer(FILE *output);
