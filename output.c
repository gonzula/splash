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
output_entry(Entry *entry, int i, int count, int *stop, void *context) {
    FILE *output = (FILE *)context;

    char *key = entry->key;
    Serializable *value = entry->obj;

    fprintf(output, "<key>%s</key>", key);

    switch (value->type) {
    case st_ht:    output_htable(output, value->ht); break;
    case st_list:  fprintf(stderr, "NOT IMPLEMENTED\n"); break;
    case st_str:   fprintf(output, "<string>%s</string>", value->str->string); break;
    case st_int:   fprintf(output, "<integer>%d</integer>", value->i); break;
    case st_float: fprintf(output, "<real>%f</real>", value->f); break;
    case st_bool:  fprintf(stderr, "NOT IMPLEMENTED\n"); break;
    case st_null:  fprintf(stderr, "NOT IMPLEMENTED\n"); break;
    }
}

void
output_htable(FILE *output, HashTable *htable) {
    fprintf(output, "<dict>");

    htable_iterate(htable, output, output_entry);

    fprintf(output, "</dict>");
}
