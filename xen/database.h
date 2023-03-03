#ifndef DATABASE_H_
#define DATABASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create an id.
 */
uint32_t create_id(void);

/**
 * @brief Delete an id.
 */
void delete_id(const uint32_t id);

/**
 * @brief Create a component table.
 *
 * @param nmemb The maximum number of components in the table.
 * @param size The stride of the components in the table in bytes.
 * @parm mask The bit id of the new table.
 * @return The id of the component array.
 */
uint32_t create_table(const size_t nmemb, const size_t size, uint32_t id);

/**
 * @brief Insert a component into a table. Copies data into component
 * storage.
 *
 * @param entity_id The id of the entity to assign the component to.
 * @param table_id The bitmask of the table.
 * @param component A pointer to the memory to store.
 * @param size The size of the component to copy in bytes.
 *
 * @return 0 on success, -1 on error.
 */
int insert_component(uint32_t entity_id, uint32_t table_id, void *component,
                     size_t size);

/**
 * @brief Combine tables together.
 *
 * TODO pass in a buffer?
 *
 * @param num_tables The number of tables to merge.
 * @param mask A bitmask of the table_ids to merge.
 * @param stride A pointer to a size_t to store the stride of the new table.
 * @return A pointer to the new table.
 */
void *zip_tables(const uint32_t num_tables, uint32_t mask, size_t *stride);

// void update_positions() {
//   const uint32_t num_velocities = 0;
//   const vec3 *velocities =
//      copy_table(velocity_table_id, &num_velocities);
//
//   const uint32_t num_positions = 0;
//   vec3 *positions = copy_table(position_table_id, &num_positions);
//
//   assert(num_positions == num_velocities);
//   do_the_thing(velocities, positions, num_velocities);
//
//   set_table(position_table_id, positions);
// }

#ifdef __cplusplus
}
#endif

#endif  // DATABASE_H_
