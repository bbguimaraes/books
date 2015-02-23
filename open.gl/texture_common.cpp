#include "texture_common.h"

#include <SOIL/SOIL.h>

GLint create_texture_from_image(const char * filename, GLenum object) {
    GLuint tex;
    glActiveTexture(object);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char * image = SOIL_load_image(
        filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

