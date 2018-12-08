#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "output.h"

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
output_operand(FILE *output, Operand op) {
    switch (op.type) {
        case number: output_number(output, op); break;
        case variable: output_get_variable(output, op.name); break;
        case magicVariable: output_get_magic_variable(output, op); break;
    }
}

void
output_number(FILE *output, Operand op) {
    char *escaped = xml_escape(op.value.value);

    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.number</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>UUID</key>");
    fprintf(output, "<string>%s</string>", op.uuid);

    fprintf(output, "<key>WFNumberActionNumber</key>");
    fprintf(output, "<real>%s</real>", escaped);
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");

    free(escaped);
}

void
output_set_variable(FILE *output, char100 name) {
    char *escaped = xml_escape(name.value);

    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.setvariable</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFVariableName</key>");
    fprintf(output, "<string>%s</string>", escaped);
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");

    free(escaped);
}

void
output_get_variable(FILE *output, char100 name) {
    char *escaped = xml_escape(name.value);

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
    fprintf(output, "<string>%s</string>", escaped);
    fprintf(output, "</dict>");
    fprintf(output, "<key>WFSerializationType</key>");
    fprintf(output, "<string>WFTextTokenAttachment</string>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");

    free(escaped);
}

void
output_get_magic_variable(FILE *output, Operand op) {
    char *escaped1 = xml_escape(op.name.value);
    char *escaped2 = xml_escape(op.uuid);

    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");
    fprintf(output, "<string>is.workflow.actions.getvariable</string>");
    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFVariable</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>Value</key>");
    fprintf(output, "<dict>");
    fprintf(output, "<key>OutputName</key>");
    fprintf(output, "<string>%s</string>", escaped1);
    fprintf(output, "<key>OutputUUID</key>");
    fprintf(output, "<string>%s</string>", escaped2);
    fprintf(output, "<key>Type</key>");
    fprintf(output, "<string>ActionOutput</string>");
    fprintf(output, "</dict>");
    fprintf(output, "<key>WFSerializationType</key>");
    fprintf(output, "<string>WFTextTokenAttachment</string>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");
    fprintf(output, "</dict>");

    free(escaped1);
    free(escaped2);
}

void
output_math_operand(FILE *output, Operand op) {
    switch (op.type) {
        case number: {
                         char *escaped = xml_escape(op.value.value);
                         fprintf(output, "<real>%s</real>", escaped);
                         free(escaped);
                         break;
                     }
        case magicVariable: {
                                char *escaped = xml_escape(op.name.value);
                                fprintf(output, "<dict>");
                                fprintf(output, "<key>Value</key>");
                                fprintf(output, "<dict>");
                                fprintf(output, "<key>OutputName</key>");
                                fprintf(output, "<string>%s</string>", escaped);
                                fprintf(output, "<key>OutputUUID</key>");
                                fprintf(output, "<string>%s</string>", op.uuid);
                                fprintf(output, "<key>Type</key>");
                                fprintf(output, "<string>ActionOutput</string>");
                                fprintf(output, "</dict>");
                                fprintf(output, "<key>WFSerializationType</key>");
                                fprintf(output, "<string>WFTextTokenAttachment</string>");
                                fprintf(output, "</dict>");
                                free(escaped);
                                break;
                            }
        case variable: {
                           char *escaped = xml_escape(op.name.value);
                           fprintf(output, "<dict>");
                           fprintf(output, "<key>Value</key>");
                           fprintf(output, "<dict>");
                           fprintf(output, "<key>Type</key>");
                           fprintf(output, "<string>Variable</string>");
                           fprintf(output, "<key>VariableName</key>");
                           fprintf(output, "<string>%s</string>", escaped);
                           fprintf(output, "</dict>");
                           fprintf(output, "<key>WFSerializationType</key>");
                           fprintf(output, "<string>WFTextTokenAttachment</string>");
                           fprintf(output, "</dict>");
                           free(escaped);
                           break;
                       }
    }
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

    output_math_operand(output, operand);

    fprintf(output, "<key>WFMathOperation</key>");
    fprintf(output, "<string>%s</string>", WFoperation);
}

void
output_math_scientific_operation_parameters(FILE *output, char operator, Operand operand) {
    if (operator == '^') {
        fprintf(output, "<key>WFMathOperation</key>");
        fprintf(output, "<string>…</string>");

        if (operand.type == number && strcmp("2", operand.value.value) == 0) {
            fprintf(output, "<key>WFScientificMathOperation</key>");
            fprintf(output, "<string>x^2</string>");
        } else if (operand.type == number && strcmp("3", operand.value.value) == 0) {
            fprintf(output, "<key>WFScientificMathOperation</key>");
            fprintf(output, "<string>x^3</string>");
        } else {
            fprintf(output, "<key>WFScientificMathOperand</key>");
            output_math_operand(output, operand);
            fprintf(output, "<key>WFScientificMathOperation</key>");
            fprintf(output, "<string>x^y</string>");
        }
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
