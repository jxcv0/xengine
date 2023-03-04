#ifndef DATABASE_H_
#define DATABASE_H_

#ifndef NUM_ENTITIES
#define NUM_ENTITIES 256
#endif

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct id_buffer {
  uint32_t *mp_data; 
  uint32_t m_head;
  uint32_t m_tail;
  uint32_t m_max_size;
  uint32_t m_size;
};

/**
 * @brief Initialize an id_buffer with memory for nmemb members.
 *
 * @param buf A pointer to the buffer to initialize.
 * @param nmemb The the number of members in the id buffer.
 */
void init_id_buffer(struct id_buffer *buf, const size_t nmemb);

/**
 * @breif Release resources in use by an id buffer.
 *
 * @param buf A pointer to the buffer to delete.
 */
void delete_id_buffer(struct id_buffer *buf);

/**
 * @brief Get the next id from the buffer.
 * 
 * @param buf A pointer to the buffer to fetch the id from.
 * @return The id.
 */
uint32_t pop_id(struct id_buffer *buf);

/**
 * @brief Push an id onto a buffer.
 *
 * @param buf A pointer to the buffer.
 * @param id The id to store.
 */
void push_id(struct id_buffer *buf, const uint32_t id);

/**
 * @brief Initialize the id free list;
 */
void db_init(void);

/**
 * @brief Create an id.
 */
uint32_t db_create_id(void);

/**
 * @brief Delete an id.
 */
void db_delete_id(const uint32_t entity_id);

/**
 * @brief Get the signature of the entity. A signature contains a mask of all
 * the table mask taht have an entry associated with the entity id.
 *
 * @param entity_id The id of the entity to query.
 * @return The the component mask of the id.
 */
uint32_t db_get_signature(const uint32_t entity_id);

/**
 * @brief Create a component table.
 *
 * @param nmemb The maximum number of components in the table.
 * @param size The stride of the components in the table in bytes.
 * @parm table_mask The unique bitmask of the component type.
 * @return The id of the component array.
 */
uint32_t db_create_table(const size_t nmemb, const size_t size,
                      uint32_t table_mask);

/**
 * @brief Insert a component into a table. Copies data into component
 * storage. Using this function will AND the associated table mask bit the
 * signature mask associated with the entity id.
 *
 * @param entity_id The id of the entity to assign the component to.
 * @param table_mask The bitmask of the table.
 * @param component A pointer to the memory to store.
 * @param size The size of the component to copy in bytes.
 *
 * @return 0 on success, -1 on error.
 */
int db_insert_component(uint32_t entity_id, uint32_t table_mask, void *component,
                     size_t size);

/**
 * @brief Combine tables into one so that each field contains contiguous
 * components corresponding to the same entity id. The tables will be put into
 * the result in the order they are specified.
 *
 * @param num_tables The number of tables to merge.
 * @param table_masks The masks of the tables to merge.
 * @param stride The stride each entry in the zipped table in bytes.
 * @return The zipped table.
 */
void *db_zip_tables(const uint32_t num_tables, uint32_t *table_masks,
                 uint32_t *stride);

/**
 * @brief Extract data from a zipped table and store it back in a table.
 *
 * @param table_mask The bitmask of the table to write back to.
 * @param stride The byte offset between concecutive fields.
 */
void db_unzip_table(uint32_t table_mask, uint32_t *stride);

#ifdef __cplusplus
}
#endif

#endif  // DATABASE_H_
