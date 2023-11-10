#pragma once

#include <vector>
#include <stdint.h>

class Mesh {
private:
    uint32_t m_vao{};
    uint32_t m_vbo{};
    uint32_t m_ebo{};

    uint32_t m_width{};
    uint32_t m_height{};

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

public:
    Mesh() = default;
    Mesh(uint32_t width, uint32_t height);

    void bind();
    void unbind();

private:
    void generateMeshData();
    uint32_t indexFrom2D(int x, int y);
};