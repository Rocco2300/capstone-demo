#include "Mesh.hpp"

// clang-format off
#include <GL/glew.h>
#include <GL/freeglut.h>
// clang-format on

Mesh::Mesh(uint32_t width, uint32_t height) : m_width{width}, m_height{height} {
    generateMeshData();
    bindBufferData();
}

void Mesh::generate(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    generateMeshData();
    bindBufferData();
}

std::vector<uint32_t>& Mesh::getIndices() {
    return m_indices;
}

void Mesh::bind() {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void Mesh::unbind() {
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::bindBufferData() {
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                 &m_vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t),
                 &m_indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

uint32_t Mesh::indexFrom2D(int x, int y) { return y * m_width + x; }

void Mesh::generateMeshData() {
    for (int y = 0; y <= m_height; y++) {
        for (int x = 0; x <= m_width; x++) {
            m_vertices.push_back(x * 0.5f);
            m_vertices.push_back(y * 0.5f);
            m_vertices.push_back(0.0f);
        }
    }

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            m_indices.push_back(indexFrom2D(x, y));
            m_indices.push_back(indexFrom2D(x + 1, y));
            m_indices.push_back(indexFrom2D(x + 1, y + 1));

            m_indices.push_back(indexFrom2D(x, y));
            m_indices.push_back(indexFrom2D(x + 1, y + 1));
            m_indices.push_back(indexFrom2D(x, y + 1));
        }
    }
}