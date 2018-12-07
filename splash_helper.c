#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "splash_helper.h"
#include "uuid.h"

void
output_header(FILE *output) {
    fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    fprintf(output, "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
    fprintf(output, "<plist version=\"1.0\">");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActions</key>");
    fprintf(output, "<array>");
}

void
output_footer(FILE *output) {
    fprintf(output, "</array>");
    fprintf(output, "<key>WFWorkflowClientRelease</key>");
    fprintf(output, "<string>2.1.2</string>");
    fprintf(output, "<key>WFWorkflowClientVersion</key>");
    fprintf(output, "<string>754</string>");
    fprintf(output, "<key>WFWorkflowIcon</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowIconGlyphNumber</key>");
    fprintf(output, "<integer>59511</integer>");
    fprintf(output, "<key>WFWorkflowIconImageData</key>");
    fprintf(output, "<data>");
    fprintf(output, "</data>");
    fprintf(output, "<key>WFWorkflowIconStartColor</key>");
    fprintf(output, "<integer>463140863</integer>");
    fprintf(output, "</dict>");
    fprintf(output, "<key>WFWorkflowImportQuestions</key>");
    fprintf(output, "<array/>");
    fprintf(output, "<key>WFWorkflowInputContentItemClasses</key>");
    fprintf(output, "<array>");
    fprintf(output, "<string>WFAppStoreAppContentItem</string>");
    fprintf(output, "<string>WFArticleContentItem</string>");
    fprintf(output, "<string>WFContactContentItem</string>");
    fprintf(output, "<string>WFDateContentItem</string>");
    fprintf(output, "<string>WFEmailAddressContentItem</string>");
    fprintf(output, "<string>WFGenericFileContentItem</string>");
    fprintf(output, "<string>WFImageContentItem</string>");
    fprintf(output, "<string>WFiTunesProductContentItem</string>");
    fprintf(output, "<string>WFLocationContentItem</string>");
    fprintf(output, "<string>WFDCMapsLinkContentItem</string>");
    fprintf(output, "<string>WFAVAssetContentItem</string>");
    fprintf(output, "<string>WFPDFContentItem</string>");
    fprintf(output, "<string>WFPhoneNumberContentItem</string>");
    fprintf(output, "<string>WFRichTextContentItem</string>");
    fprintf(output, "<string>WFSafariWebPageContentItem</string>");
    fprintf(output, "<string>WFStringContentItem</string>");
    fprintf(output, "<string>WFURLContentItem</string>");
    fprintf(output, "</array>");
    fprintf(output, "<key>WFWorkflowTypes</key>");
    fprintf(output, "<array>");
    fprintf(output, "<string>NCWidget</string>");
    fprintf(output, "<string>WatchKit</string>");
    fprintf(output, "</array>");
    fprintf(output, "</dict>");
    fprintf(output, "</plist>");
    fprintf(output, "\n");
}

void
output_number(FILE *output, char100 number) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.number</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFNumberActionNumber</key>");
    fprintf(output, "<real>%s</real>", number.value);
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
}

void
output_set_variable(FILE *output, char100 name) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.setvariable</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFVariableName</key>");
    fprintf(output, "<string>%s</string>", name.value);
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");

}

void
output_get_variable(FILE *output, char100 name) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.getvariable</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFVariable</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>Value</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>Type</key>");
    fprintf(output, "<string>Variable</string>");
    fprintf(output, "<key>VariableName</key>");
    fprintf(output, "<string>%s</string>", name.value);
    fprintf(output, "</dict>");
    fprintf(output, "<key>WFSerializationType</key>");
    fprintf(output, "<string>WFTextTokenAttachment</string>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
}

void
output_get_magic_variable(FILE *output, Operand op) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.getvariable</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFVariable</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>Value</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>%s</key>", op.name.value);
    fprintf(output, "<string>Calculation Result</string>");
    fprintf(output, "<key>OutputUUID</key>");
    fprintf(output, "<string>%s</string>", op.uuid);
    fprintf(output, "<key>Type</key>");
    fprintf(output, "<string>ActionOutput</string>");
    fprintf(output, "</dict>");
    fprintf(output, "<key>WFSerializationType</key>");
    fprintf(output, "<string>WFTextTokenAttachment</string>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
}

void
output_math_simple_operation_parameters(FILE *output, char operator, Operand operand) {
    char WFoperation[30];
    switch (operator) {
        case '+': strcpy(WFoperation, "+"); break;
        case '-': strcpy(WFoperation, "-"); break;
        case '*': strcpy(WFoperation, "×"); break;
        case '/': strcpy(WFoperation, "÷"); break;
    }

    fprintf(output, "<key>WFMathOperand</key>");

    switch (operand.type) {
        case number:
            fprintf(output, "<real>%s</real>", operand.value.value);
            break;
        case magicVariable:
            fprintf(output, "<dict>");
            fprintf(output, "<key>Value</key>");
            fprintf(output, "<dict>");
            fprintf(output, "<key>OutputName</key>");
            fprintf(output, "<string>%s</string>", operand.name.value);
            fprintf(output, "<key>OutputUUID</key>");
            fprintf(output, "<string>%s</string>", operand.uuid);
            fprintf(output, "<key>Type</key>");
            fprintf(output, "<string>ActionOutput</string>");
            fprintf(output, "</dict>");
            fprintf(output, "<key>WFSerializationType</key>");
            fprintf(output, "<string>WFTextTokenAttachment</string>");
            fprintf(output, "</dict>");
            break;
        case variable:
            fprintf(output, "<dict>");
            fprintf(output, "<key>Value</key>");
            fprintf(output, "<dict>");
            fprintf(output, "<key>Type</key>");
            fprintf(output, "<string>Variable</string>");
            fprintf(output, "<key>VariableName</key>");
            fprintf(output, "<string>%s</string>", operand.name.value);
            fprintf(output, "</dict>");
            fprintf(output, "<key>WFSerializationType</key>");
            fprintf(output, "<string>WFTextTokenAttachment</string>");
            fprintf(output, "</dict>");
            break;
    }

    fprintf(output, "<key>WFMathOperation</key>");
    fprintf(output, "<string>%s</string>", WFoperation);
}

void
output_math_scientific_operation_parameters(FILE *output, char operator, Operand operand) {
    if (operator == '^') {
        fprintf(output, "<key>WFMathOperation</key>");
        fprintf(output, "<string>…</string>");
        fprintf(output, "<key>WFScientificMathOperand</key>");

        switch (operand.type) {
            case number:
                fprintf(output, "<real>%s</real>", operand.value.value);
                break;
        case magicVariable:
            fprintf(output, "<dict>");
            fprintf(output, "<key>Value</key>");
            fprintf(output, "<dict>");
            fprintf(output, "<key>OutputName</key>");
            fprintf(output, "<string>%s</string>", operand.name.value);
            fprintf(output, "<key>OutputUUID</key>");
            fprintf(output, "<string>%s</string>", operand.uuid);
            fprintf(output, "<key>Type</key>");
            fprintf(output, "<string>ActionOutput</string>");
            fprintf(output, "</dict>");
            fprintf(output, "<key>WFSerializationType</key>");
            fprintf(output, "<string>WFTextTokenAttachment</string>");
            fprintf(output, "</dict>");
            break;
            case variable:
                fprintf(output, "<dict>");
                fprintf(output, "<key>Type</key>");
                fprintf(output, "<string>Variable</string>");
                fprintf(output, "<key>VariableName</key>");
                fprintf(output, "<string>%s</string>", operand.name.value);
                fprintf(output, "<key>WFSerializationType</key>");
                fprintf(output, "<string>WFTextTokenAttachment</string>");
                fprintf(output, "</dict>");
                break;
        }

        fprintf(output, "<key>WFScientificMathOperation</key>");
        fprintf(output, "<string>x^y</string>");
    }
}

void
output_operation(FILE *output, char operator, Operand operand, char *uuid) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.math</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>UUID</key>");
    fprintf(output, "<string>%s</string>", uuid);

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

    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
}

void
append_operand(Operand *stack, OpType type, char100 operand) {
    Operand temp;
    *stack = temp;
    (*stack).type = type;

    switch (type) {
        case number:   strcpy((*stack).value.value, operand.value); break;
        case variable: strcpy((*stack).name.value, operand.value); break;
        case magicVariable:
                       strcpy((*stack).name.value, operand.value); break;
                       uuid_gen((*stack).uuid);
                       break;
    }
}

void
append_operation(Operand *stack, char operator, Operand op1, Operand op2) {
    switch (op1.type) {
        case number: output_number(stdout, op1.value); break;
        case variable: output_get_variable(stdout, op1.name); break;
        case magicVariable: output_get_magic_variable(stdout, op1); break;
    }

    char uuid[37];
    uuid_gen(uuid);
    output_operation(stdout, operator, op2, uuid);

    Operand new_stack;
    new_stack.type = magicVariable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, uuid);

    *stack = new_stack;
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = number;
    char minus_one[] = "-1";
    strcpy(temp.value.value, minus_one);

    append_operation(stack, '*', temp, op);
}

void
set_variable(char100 id, Operand op) {
    switch (op.type) {
        case number: output_number(stdout, op.value); break;
        case variable: output_get_variable(stdout, op.name); break;
        case magicVariable: break;  // if it's a magic variable, its the flow
    }

    output_set_variable(stdout, id);
}
