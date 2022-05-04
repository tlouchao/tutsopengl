#include <GLFW/glfw3.h>
#include <iostream>
#include <limits>

class VAOWrapper {

public:
    VAOWrapper(float dist = 0.5f, unsigned int nVerts = 4, unsigned int nTotalVerts = 6, unsigned int nFaces = 6){
        initVAOWrapper(dist, nVerts, nTotalVerts, nFaces);
    }

    ~VAOWrapper() noexcept {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    unsigned int getVAO() { return m_VAO; }
    unsigned int getVBO() { return m_VBO; }
    unsigned int getIndices() { return m_indices; }
    unsigned int getStride() { return m_stride; }

private:

    static const int POS3_LEN = 3;
    static const int COLOR3_LEN = 3;
    static const int TEXCOORD2_LEN = 2;

    void initVAOWrapper(float dist, unsigned int nVerts = 4, unsigned int nTotalVerts = 6, unsigned int nFaces = 6){
        m_indices = nFaces * nTotalVerts;
        m_stride = POS3_LEN + COLOR3_LEN + TEXCOORD2_LEN;
        float vertices[m_indices * m_stride];
        int fIdx = 3;
        float dists[2]{-dist, -dist};
        float coords[2]{0.f, 0.f};
        float colors[COLOR3_LEN * nTotalVerts]{ 1.f, 0.f, 0.f,
                                                0.f, 1.f, 0.f,
                                                0.f, 0.f, 1.f,
                                                1.f, 1.f, 0.f,
                                                0.f, 1.f, 1.f,
                                                1.f, 0.f, 1.f};
        for (unsigned int i = 0; i < nFaces; ++i){
            for (unsigned int j = 0; j < nTotalVerts; ++j){
                unsigned int offset = (i * nTotalVerts * m_stride) + j * m_stride;

                // config vertex coords and texture coords
                if (j == 0 || j == nTotalVerts - 1){
                    dists[0] = -dist; dists[1] = -dist;
                    coords[0] = 0.f; coords[1] = 0.f;
                } else if (j == 1){
                    dists[0] = dist; dists[1] = -dist;
                    coords[0] = 1.f; coords[1] = 0.f;
                } else if (j == 2 || j == 3){
                    dists[0] = dist; dists[1] = dist;
                    coords[0] = 1.f; coords[1] = 1.f;
                } else if (j == 4){
                    dists[0] = -dist; dists[1] = dist;
                    coords[0] = 0.f; coords[1] = 1.f;
                }
                float fDist = dist;
                if (fIdx % 2 == 0){
                    // negative X, Y, or Z axis. corresponds to left, bottom, back faces of cube
                    fDist = -dist;
                    // mirror texture coordinates
                    if (std::fabs(coords[0] - 1.f) <= std::numeric_limits<float>::epsilon()) {
                        coords[0] = 0.f;
                    } else {
                        coords[0] = 1.f;
                    }
                }
                // set vertex coords
                if (fIdx < 2) {
                    vertices[offset + 0] = fDist;
                    vertices[offset + 1] = dists[0];
                    vertices[offset + 2] = dists[1];
                } else if (fIdx < 4) {
                    vertices[offset + 0] = dists[1];
                    vertices[offset + 1] = fDist;
                    vertices[offset + 2] = dists[0];
                } else { // fIdx < 6
                    vertices[offset + 0] = dists[0];
                    vertices[offset + 1] = dists[1];
                    vertices[offset + 2] = fDist;
                }

                // set colors
                vertices[offset + 3] = colors[j * COLOR3_LEN + 0];
                vertices[offset + 4] = colors[j * COLOR3_LEN + 1];
                vertices[offset + 5] = colors[j * COLOR3_LEN + 2];

                // set tex coords
                vertices[offset + 6] = coords[0];
                vertices[offset + 7] = coords[1];
            }
            --fIdx;
            fIdx = (fIdx < 0) ? nFaces - 1 : fIdx % nFaces;
        }
        
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, POS3_LEN, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, COLOR3_LEN, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)(POS3_LEN * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, TEXCOORD2_LEN, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)((POS3_LEN + COLOR3_LEN) * sizeof(float)));
        glEnableVertexAttribArray(2);
        /*
        // Debug //
        for (int i = 0; i < nFaces; ++i){
            for (int j = 0; j < nTotalVerts; ++j){
                std::cout << "Row " << i * nFaces + j << ": ";
                for (int k = 0; k < m_stride; ++k){
                    std::cout << vertices[(i * nTotalVerts * m_stride) + (j * m_stride) + k] << ' ';
                }
                std::cout << '\n';
            }
        }
        */
    }

    // member variables
    unsigned int m_indices;
    unsigned int m_stride;
    unsigned int m_VBO; 
    unsigned int m_VAO;
};