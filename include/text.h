#ifndef TEXT_H_
#define TEXT_H_

/**
 * @brief
 *
 * @param filepath
 * @return int
 */
int init_text(const char *filepath);

/**
 * @brief
 *
 * @param x
 * @param y
 * @param scale
 * @param text
 * @param col
 */
void render_text(float x, float y, float scale, char *text, float col[4]);

#endif  // TEXT_H_