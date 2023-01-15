#include <asm-generic/errno-base.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "libxml/tree.h"
#include "lin.h"
#include "mesh.h"

struct source {
  float *mp_array; // only float supported
  uint32_t m_count;
  uint32_t m_accessor_count;
  uint32_t m_accessor_stride;
};

/**
 * @brief Creates a directory with RW USR mode unless the directory already
 * exists.
 *
 * @param path The relative path to create the directory in.
 * @return 0 on success. -1 on error.
 */
int make_dir(const char *path);

/**
 * @brief Find the next occurrence of a node within the childeren of curr.
 *
 * @param node The node to search.
 * @param name The name of the child node to search for.
 * @return The child node if found. Otherwise null.
 */
xmlNodePtr find_child(xmlNodePtr node, const char *name);

/**
 * @brief Parse all geometry elements and generate .geom files. Created files
 * are added to ${PWD}/assets/geometries. They will share the same file name as
 * the id property of the geometry tag.
 *
 * @param node The <geometries> node.
 */
void parse_geometries(xmlNodePtr node);

/**
 * @brief Parse a single geometry element.
 *
 * @param id The string id property of the geometry.
 * @param node The <geometry> node.
 */
void parse_geometry(const xmlChar *id, xmlNodePtr node);

/**
 * @brief Parse a single mesh element.
 *
 * @param node The <mesh> node.
 * @return A mesh structure containing the data from the node.
 */
struct mesh parse_mesh(xmlNodePtr node);

/**
 * ----------------------------------------------------------------------------
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage:\n\tcollada <dae_file>\n");
    return 0;
  }

  printf("opening file: %s\n", argv[1]);
  xmlDocPtr doc = xmlParseFile(argv[1]);
  if (doc == NULL) {
    fprintf(stderr, "unable to parse file\n");
    return 0;
  }

  xmlNodePtr curr = xmlDocGetRootElement(doc);
  if (curr == NULL || xmlStrcmp(curr->name, (const xmlChar *)"COLLADA") != 0) {
    fprintf(stderr, "COLLADA root node not found\n");
  }

  make_dir("assets");

  curr = curr->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)"library_geometries") == 0) {
      printf("parsing geometries\n");
      parse_geometries(curr);
    }
    curr = curr->next;
  }

  xmlFreeDoc(doc);
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
xmlNodePtr find_child(xmlNodePtr node, const char *name) {
  xmlNodePtr curr = node->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)name) == 0) {
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
}

/**
 * ----------------------------------------------------------------------------
 */
int make_dir(const char *path) {
  const int mode = S_ISUID | S_IROTH | S_IWOTH | S_IEXEC | S_IRUSR | S_IWUSR |
                   S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP;
  printf("creating: %s\n", path);
  if (mkdir(path, mode) == -1) {
    if (errno == EEXIST) {
      printf("\tdirectory already exists\n\tcontinuing\n");
      return 0;
    } else {
      perror("mkdir");
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_geometries(xmlNodePtr node) {
  // there must be at least 1 geometry element so we mkdir here
  make_dir("assets/geometries");

  xmlNodePtr curr = node->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)"geometry") == 0) {
      xmlChar *id = xmlGetProp(curr, (const xmlChar *)"id");
      printf("found geometry: %s\n", id);
      parse_geometry(id, curr);
      free(id);
    }
    curr = curr->next;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_geometry(const xmlChar *id, xmlNodePtr node) {
  char buf[128];
  sprintf(buf, "assets/geometries/%s.geom", id);
  printf("writing to file %s\n", buf);
  FILE *file = fopen(buf, "w+");  // overwrite file
  if (file == NULL) {
    perror("fopen");
    return;
  }

  xmlNodePtr curr = node->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)"mesh") == 0) {
      parse_mesh(curr);
    }
    curr = curr->next;
  }

  // TODO other elements within geometry if required

  fwrite(buf, strlen(buf), 1, file);

  fclose(file);
}

/**
 * ----------------------------------------------------------------------------
 */
struct mesh parse_mesh(xmlNodePtr node) {
  struct mesh m = {0};

  xmlNodePtr curr = node->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)"source") == 0) {
      // struct source parse_source();
      // source tags
      xmlNodePtr array_node = curr->xmlChildrenNode;
      while (array_node != NULL) {
        // get array

        // get accessor
        array_node = array_node->next;
      }
    } else if (xmlStrcmp(curr->name, (const xmlChar *)"vertices") == 0) {
      // vertex tags
    }
    curr = curr->next;
  }
  return m;
}
