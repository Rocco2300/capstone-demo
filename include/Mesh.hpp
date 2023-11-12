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
    std::vector<uint32_t> m_indices;

public:
    Mesh() = default;
    Mesh(uint32_t width, uint32_t height);

    void generate(uint32_t width, uint32_t height);

    std::vector<uint32_t>& getIndices();

    void bind();
    void unbind();

private:
    void bindBufferData();
    void generateMeshData();
    uint32_t indexFrom2D(int x, int y);
};