#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "splash_helper.h"

void
output_header(FILE *output) {
    char header[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
                     <!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n\
                     <plist version=\"1.0\">\n\
                     <dict>\n\
                     \t<key>WFWorkflowActions</key>\n\
                     \t<array>\n\
                     ";

    fwrite(header, sizeof(char), strlen(header), output);
}

void
output_footer(FILE *output) {
    char footer[] = "\t</array>\n\
                     <key>WFWorkflowClientRelease</key>\n\
                     <string>2.1.2</string>\n\
                     <key>WFWorkflowClientVersion</key>\n\
                     <string>754</string>\n\
                     <key>WFWorkflowIcon</key>\n\
                     <dict>\n\
                     <key>WFWorkflowIconGlyphNumber</key>\n\
                     <integer>59511</integer>\n\
                     <key>WFWorkflowIconImageData</key>\n\
                     <data>\n\
                     </data>\n\
                     <key>WFWorkflowIconStartColor</key>\n\
                     <integer>463140863</integer>\n\
                     </dict>\n\
                     <key>WFWorkflowImportQuestions</key>\n\
                     <array/>\n\
                     <key>WFWorkflowInputContentItemClasses</key>\n\
                     <array>\n\
                     <string>WFAppStoreAppContentItem</string>\n\
                     <string>WFArticleContentItem</string>\n\
                     <string>WFContactContentItem</string>\n\
                     <string>WFDateContentItem</string>\n\
                     <string>WFEmailAddressContentItem</string>\n\
                     <string>WFGenericFileContentItem</string>\n\
                     <string>WFImageContentItem</string>\n\
                     <string>WFiTunesProductContentItem</string>\n\
                     <string>WFLocationContentItem</string>\n\
                     <string>WFDCMapsLinkContentItem</string>\n\
                     <string>WFAVAssetContentItem</string>\n\
                     <string>WFPDFContentItem</string>\n\
                     <string>WFPhoneNumberContentItem</string>\n\
                     <string>WFRichTextContentItem</string>\n\
                     <string>WFSafariWebPageContentItem</string>\n\
                     <string>WFStringContentItem</string>\n\
                     <string>WFURLContentItem</string>\n\
                     </array>\n\
                     <key>WFWorkflowTypes</key>\n\
                     <array>\n\
                     <string>NCWidget</string>\n\
                     <string>WatchKit</string>\n\
                     </array>\n\
                     </dict>\n\
                     </plist>\n";

    fwrite(footer, sizeof(char), strlen(footer), output);
}

void
output_number(FILE *output, char *number) {
    char patt[] = "<dict>\n\
                   <key>WFWorkflowActionIdentifier</key>\n\
                   <string>is.workflow.actions.number</string>\n\
                   <key>WFWorkflowActionParameters</key>\n\
                   <dict>\n\
                   <key>WFNumberActionNumber</key>\n\
                   <real>%s</real>\n\
                   </dict>\n\
                   </dict>\n";

    fprintf(output, patt, number);
}

void
output_set_var(FILE *output, char *name) {
    char patt[] = "<dict>\n\
                   <key>WFWorkflowActionIdentifier</key>\n\
                   <string>is.workflow.actions.setvariable</string>\n\
                   <key>WFWorkflowActionParameters</key>\n\
                   <dict>\n\
                   <key>WFVariableName</key>\n\
                   <string>%s</string>\n\
                   </dict>\n\
                   </dict>\n";

    fprintf(output, patt, name);

}

void
output_math_simple_operation_parameters(FILE *output, char operator, Operand *operand) {
    char WFoperation[30];
    switch (operator) {
        case '+': strcpy(WFoperation, "+"); break;
        case '-': strcpy(WFoperation, "-"); break;
        case '*': strcpy(WFoperation, "×"); break;
        case '/': strcpy(WFoperation, "÷"); break;
    }

    fprintf(output, "   <key>WFMathOperand</key>\n");

    switch (operand->type) {
        case number:
            fprintf(output, "<real>%s</real>\n", operand->value);
            break;
        case variable:
            fprintf(output, "<dict>\n");
            fprintf(output, "<key>Value</key>\n");
            fprintf(output, "<dict>\n");
            fprintf(output, "   <key>Type</key>\n");
            fprintf(output, "   <string>Variable</string>\n");
            fprintf(output, "   <key>VariableName</key>\n");
            fprintf(output, "   <string>%s</string>\n", operand->value);
            fprintf(output, "   </dict>\n");
            fprintf(output, "<key>WFSerializationType</key>\n");
            fprintf(output, "<string>WFTextTokenAttachment</string>\n");
            fprintf(output, "</dict>\n");
            break;
    }

    fprintf(output, "   <key>WFMathOperation</key>\n");
    fprintf(output, "   <string>%s</string>\n", WFoperation);
}

void
output_math_scientific_operation_parameters(FILE *output, char operator, Operand *operand) {
    if (operator == '^') {
        fprintf(output, "<key>WFMathOperation</key>\n");
        fprintf(output, "<string>…</string>\n");
        fprintf(output, "<key>WFScientificMathOperand</key>\n");

        switch (operand->type) {
            case number:
                fprintf(output, "<real>%s</real>\n", operand->value);
                break;
            case variable:
                fprintf(output, "<dict>\n");
                fprintf(output, "   <key>Type</key>\n");
                fprintf(output, "   <string>Variable</string>\n");
                fprintf(output, "   <key>VariableName</key>\n");
                fprintf(output, "   <string>%s</string>\n", operand->value);
                fprintf(output, "   <key>WFSerializationType</key>\n");
                fprintf(output, "   <string>WFTextTokenAttachment</string>\n");
                fprintf(output, "</dict>\n");
                break;
        }

        fprintf(output, "<key>WFScientificMathOperation</key>\n");
        fprintf(output, "<string>x^y</string>\n");
    }
}

void
output_operation(FILE *output, char operator, Operand *operand) {
    fprintf(output, "<dict>\n");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>\n");
    fprintf(output, "<string>is.workflow.actions.math</string>\n");
    fprintf(output, "<key>WFWorkflowActionParameters</key>\n");
    fprintf(output, "<dict>\n");

    switch (operator) {
        case '+':
        case '-':
        case '*':
        case '/':
            output_math_simple_operation_parameters(output, operator, operand);
            break;
        case '^':
            output_math_scientific_operation_parameters(output, operator, operand);
            break;
    }

    fprintf(output, "</dict>\n");
    fprintf(output, "</dict>\n");
}

void
append_operand(Operand **stack, OpType type, const char *operand) {
    *stack = (Operand *)malloc(sizeof(Operand));
    (*stack)->type = type;
    (*stack)->value = (char *)malloc(sizeof(char) * (strlen(operand) + 1));
    strcpy((*stack)->value, operand);
}

void
append_operation(Operand **stack, char operator, Operand *op1, Operand *op2) {
    switch (op1->type) {
        case number: output_number(stdout, op1->value); break;
        case variable: break;
    }

    output_operation(stdout, operator, op2);

    output_set_var(stdout, "AUX");

    /*puts(op1);*/
    /*printf("# %c %s\n", operator, op2);*/
    /*puts("AUX = #");*/
    /*char aux[] = "AUX";*/
    /*strcpy(*stack, aux);*/
    Operand *new_stack = (Operand *)malloc(sizeof(Operand));
    new_stack->type = variable;

    new_stack->value = (char *)malloc(sizeof(char) * (strlen("AUX") + 1));
    strcpy(new_stack->value, "AUX");
    *stack = new_stack;
}
