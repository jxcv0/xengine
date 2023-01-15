#include <stdio.h>
#include <libxml/parser.h>

int main(int argc, char **argv) {
  (void) argv;
  if (argc != 2) {
    printf("usage:\n\tcollada <dae_file>\n");
    return 0;
  }

  xmlDocPtr doc = xmlParseFile(argv[1]);
  if (doc == NULL) {
    printf("unable to parse file\n");
    return 0;
  }

  xmlNodePtr cur = xmlDocGetRootElement(doc);
  printf("%s\n", cur->content);

  return 0;
}
