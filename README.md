# XENGINE
Game Engine

## TODO's
- uniform buffer objects
- deferred rendering
- animation (editor tool or export from blender)
- tangent/bitangent generation for normal maps

## DB Schema

### Attributes
| Attribute Name   | Value Type |
|------------------|------------|
| entity_id        | uint32_t   | 
| model_id         | uint32_t   | 
| texture_id       | uint32_t   |
| animation_set_id | uint32_t   |
| animation_id     | uint32_t   |
| position         | vec3       |
| velocity         | vec3       |
| bearing          | vec3       |
| health           | uint32_t   |

### Model Table
| entity_id | model_id |
|-----------|----------|

### Position Table
| entity_id | position |
|-----------|----------|

### Velocity Table
| entity_id | velocity |
|-----------|----------|

### Bearing Table
| entity_id | bearing |
|-----------|---------|

### Texture Table
| mesh_id | texture_id |
|---------|------------|

### Animation Group Table
| model_id | animation_set_id |
|----------|------------------|

## Animation Table
| animation_group | strafe_range | velocity | animation_id |
|-----------------|--------------|----------|--------------|

## Systems
| System Name | In        | In Mode | Out       | Out Mode |
|-------------|-----------|---------|-----------|----------|
| sys_input   | GLOBAL_IN | R       | velocity  | W        |
| sys_move    | velocity  | R       | position  | W        |

