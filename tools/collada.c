#include <assert.h>
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

#define POSITIONS_INDEX 0
#define NORMALS_INDEX 1
#define TEXCOORDS_INDEX 2

struct mesh dest_mesh;
// struct animation dest_anim[MAX_NUM_ANIMATIONS];
// struct texture dest_tex;

/**
 *-----------------------------------------------------------------------------
 */

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
 */
void parse_mesh(xmlNodePtr node);

void parse_source(xmlNodePtr node, float *sources[3]);

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
  assert(curr);
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
  printf("attempting to make directory: %s\n", path);
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
void parse_mesh(xmlNodePtr node) {
  struct mesh mesh = {0};
  float *sources[3];

  xmlNodePtr curr = node->xmlChildrenNode;
  while (curr != NULL) {
    if (xmlStrcmp(curr->name, (const xmlChar *)"source") == 0) {
      // source
      parse_source(curr, sources);
    }
    curr = curr->next;
  }

  // find vertices;
  xmlNodePtr vertices_node = find_child(node, "vertices");
  if (vertices_node == NULL) {
    fprintf(stderr, "unable to load mesh - no vertices element found\n");
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_source(xmlNodePtr node, float *sources[3]) {
  (void)sources;
  // Take semantic from id instead of from <input>. This could be problematic
  // when using different exporter.
  
  uint32_t dest = 0;
  xmlChar *semantic = xmlGetProp(node, (const xmlChar *)"id");
  if (xmlStrstr(semantic, (const xmlChar *)"positions") == 0) {
    dest = POSITIONS_INDEX;
  } else if (xmlStrstr(semantic, (const xmlChar *)"normals") == 0) {
    dest = NORMALS_INDEX;
  } else if (xmlStrstr(semantic, (const xmlChar *)"map") == 0) {
    dest = TEXCOORDS_INDEX;
  }

  xmlNodePtr float_array = find_child(node, "float_array");
  xmlChar *count = xmlGetProp(float_array, (const xmlChar *)"count");

  sources[dest] = malloc(sizeof(float) * atoi((const char *)count));

  uint32_t offset = 0;
  uint32_t stride = 1;

  xmlNodePtr technique_node = find_child(node, "technique_common");
  xmlNodePtr accessor_node = find_child(technique_node, "accessor");
  xmlChar *accessor_offset =
      xmlGetProp(accessor_node, (const xmlChar *)"offset");
  if (accessor_offset != NULL) {
    offset = atoi((const char *)accessor_offset);
  }

  xmlChar *accessor_stride =
      xmlGetProp(accessor_node, (const xmlChar *)"stride");
  if (accessor_stride != NULL) {
    stride = atoi((const char *)accessor_stride);
  }

  free(accessor_offset);
  free(accessor_stride);
  free(count);
}
