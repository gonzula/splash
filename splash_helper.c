#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "splash_helper.h"
#include "uuid.h"

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
output_number(FILE *output, char100 number) {
    char patt[] = "<dict>\n\
                   <key>WFWorkflowActionIdentifier</key>\n\
                   <string>is.workflow.actions.number</string>\n\
                   <key>WFWorkflowActionParameters</key>\n\
                   <dict>\n\
                   <key>WFNumberActionNumber</key>\n\
                   <real>%s</real>\n\
                   </dict>\n\
                   </dict>\n";

    fprintf(output, patt, number.value);
}

void
output_set_variable(FILE *output, char100 name) {
    char patt[] = "<dict>\n\
                   <key>WFWorkflowActionIdentifier</key>\n\
                   <string>is.workflow.actions.setvariable</string>\n\
                   <key>WFWorkflowActionParameters</key>\n\
                   <dict>\n\
                   <key>WFVariableName</key>\n\
                   <string>%s</string>\n\
                   </dict>\n\
                   </dict>\n";

    fprintf(output, patt, name.value);
}

void
output_get_variable(FILE *output, char100 name) {
        fprintf(output, "<dict>\n");
        fprintf(output, "   <key>WFWorkflowActionIdentifier</key>\n");
        fprintf(output, "   <string>is.workflow.actions.getvariable</string>\n");
        fprintf(output, "   <key>WFWorkflowActionParameters</key>\n");
        fprintf(output, "   <dict>\n");
        fprintf(output, "       <key>WFVariable</key>\n");
        fprintf(output, "       <dict>\n");
        fprintf(output, "           <key>Value</key>\n");
        fprintf(output, "           <dict>\n");
        fprintf(output, "               <key>Type</key>\n");
        fprintf(output, "               <string>Variable</string>\n");
        fprintf(output, "               <key>VariableName</key>\n");
        fprintf(output, "               <string>%s</string>\n", name.value);
        fprintf(output, "           </dict>\n");
        fprintf(output, "           <key>WFSerializationType</key>\n");
        fprintf(output, "           <string>WFTextTokenAttachment</string>\n");
        fprintf(output, "       </dict>\n");
        fprintf(output, "   </dict>\n");
        fprintf(output, "</dict>\n");
}

void
output_get_magic_variable(FILE *output, Operand op) {
    fprintf(output, "<dict>\n");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>\n");
    fprintf(output, "<string>is.workflow.actions.getvariable</string>\n");
    fprintf(output, "<key>WFWorkflowActionParameters</key>\n");
    fprintf(output, "<dict>\n");
    fprintf(output, "   <key>WFVariable</key>\n");
    fprintf(output, "   <dict>\n");
    fprintf(output, "       <key>Value</key>\n");
    fprintf(output, "       <dict>\n");
    fprintf(output, "           <key>%s</key>\n", op.name.value);
    fprintf(output, "           <string>Calculation Result</string>\n");
    fprintf(output, "           <key>OutputUUID</key>\n");
    fprintf(output, "           <string>%s</string>\n", op.uuid);
    fprintf(output, "           <key>Type</key>\n");
    fprintf(output, "           <string>ActionOutput</string>\n");
    fprintf(output, "       </dict>\n");
    fprintf(output, "       <key>WFSerializationType</key>\n");
    fprintf(output, "       <string>WFTextTokenAttachment</string>\n");
    fprintf(output, "   </dict>\n");
    fprintf(output, "</dict>\n");
    fprintf(output, "</dict>\n");
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

    fprintf(output, "   <key>WFMathOperand</key>\n");

    switch (operand.type) {
        case number:
            fprintf(output, "<real>%s</real>\n", operand.value.value);
            break;
        case magicVariable:
            fprintf(output, "<dict>\n");
            fprintf(output, "<key>Value</key>\n");
            fprintf(output, "<dict>\n");
            fprintf(output, "    <key>OutputName</key>\n");
            fprintf(output, "    <string>%s</string>\n", operand.name.value);
            fprintf(output, "    <key>OutputUUID</key>\n");
            fprintf(output, "    <string>%s</string>\n", operand.uuid);
            fprintf(output, "    <key>Type</key>\n");
            fprintf(output, "    <string>ActionOutput</string>\n");
            fprintf(output, "    </dict>\n");
            fprintf(output, "<key>WFSerializationType</key>\n");
            fprintf(output, "<string>WFTextTokenAttachment</string>\n");
            fprintf(output, "</dict>\n");
            break;
        case variable:
            fprintf(output, "<dict>\n");
            fprintf(output, "<key>Value</key>\n");
            fprintf(output, "<dict>\n");
            fprintf(output, "   <key>Type</key>\n");
            fprintf(output, "   <string>Variable</string>\n");
            fprintf(output, "   <key>VariableName</key>\n");
            fprintf(output, "   <string>%s</string>\n", operand.name.value);
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
output_math_scientific_operation_parameters(FILE *output, char operator, Operand operand) {
    if (operator == '^') {
        fprintf(output, "<key>WFMathOperation</key>\n");
        fprintf(output, "<string>…</string>\n");
        fprintf(output, "<key>WFScientificMathOperand</key>\n");

        switch (operand.type) {
            case number:
                fprintf(output, "<real>%s</real>\n", operand.value.value);
                break;
        case magicVariable:
            fprintf(output, "<dict>\n");
            fprintf(output, "<key>Value</key>\n");
            fprintf(output, "<dict>\n");
            fprintf(output, "    <key>OutputName</key>\n");
            fprintf(output, "    <string>%s</string>\n", operand.name.value);
            fprintf(output, "    <key>OutputUUID</key>\n");
            fprintf(output, "    <string>%s</string>\n", operand.uuid);
            fprintf(output, "    <key>Type</key>\n");
            fprintf(output, "    <string>ActionOutput</string>\n");
            fprintf(output, "</dict>\n");
            fprintf(output, "<key>WFSerializationType</key>\n");
            fprintf(output, "<string>WFTextTokenAttachment</string>\n");
            fprintf(output, "</dict>\n");
            break;
            case variable:
                fprintf(output, "<dict>\n");
                fprintf(output, "   <key>Type</key>\n");
                fprintf(output, "   <string>Variable</string>\n");
                fprintf(output, "   <key>VariableName</key>\n");
                fprintf(output, "   <string>%s</string>\n", operand.name.value);
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
output_operation(FILE *output, char operator, Operand operand, char *uuid) {
    fprintf(output, "<dict>\n");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>\n");
    fprintf(output, "<string>is.workflow.actions.math</string>\n");
    fprintf(output, "<key>WFWorkflowActionParameters</key>\n");
    fprintf(output, "<dict>\n");
    fprintf(output, "   <key>UUID</key>\n");
    fprintf(output, "   <string>%s</string>\n", uuid);

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
