#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <vector>
#include <string>

class Textures {

public:

    Textures(const std::vector<std::string>& tNames, unsigned int tLen){
        m_len = tLen;
        m_texture_id = new unsigned int[tLen];
        m_texture_data = new unsigned int[tLen];
        for(unsigned int i = 0; i < tLen; ++i){
            m_texture_id[i] = GL_TEXTURE0 + i;
        }
        initTextures(tNames, tLen);
    }

    ~Textures() noexcept {
        delete[] m_texture_id;
        delete[] m_texture_data;
    }

    unsigned int getTextureId(unsigned int idx){
        if (idx >= m_len) {
            std::cerr << idx + " out of range\n";
        } else {
            return m_texture_id[idx]; 
        }
    }

private:

    void initTextures(const std::vector<std::string>& tNames, unsigned int tLen){

        glGenTextures(tLen, m_texture_data);

        unsigned char* imgData;
        int width, height, nrChannels;
        for(unsigned int i = 0; i < tLen; ++i){

            glActiveTexture(m_texture_id[i]);
            glBindTexture(GL_TEXTURE_2D, m_texture_data[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            const char* imgFile = tNames[i].c_str();
            stbi_set_flip_vertically_on_load(true);
            unsigned int ok = stbi_info(imgFile, &width, &height, &nrChannels);

            if (!ok) {
                std::cerr << imgFile << " width, height, and # of channels not read\n"; 
            }

            unsigned char* imgData = stbi_load(imgFile, &width, &height, &nrChannels, 0);

            if (!imgData) {
                std::cerr << imgFile << " not loaded\n"; 
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(imgData);
        }
    }
    
    unsigned int m_len;
    unsigned int* m_texture_id;
    unsigned int* m_texture_data;
};