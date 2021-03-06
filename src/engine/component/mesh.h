#ifndef MESH_H_
#define MESH_H_

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "../data/block_texture.h"
#include "../data/shader_layout.h"
#include "../utils/mat.h"
#include "../utils/mat_opengl.h"

struct BlockVertex {
    math::vec3f position;
    math::vec2f textpos;
    BlockTexture textid;
};

enum class FaceKind { Top, Bottom, Front, Back, Left, Right };

class MeshChunk {
   public:
    MeshChunk() { updateModelMatrix(); }

    static constexpr std::array layout{ 
        LayoutItem{"in_position", LayoutType::Float, 3},
        LayoutItem{"in_texposition", LayoutType::Float, 2},
        LayoutItem{"in_textid", LayoutType::Int, 1},
    };

    // position is left top corner
    void addBlockFace(math::vec3f coords, FaceKind kind,
                      BlockTexture texture_id) {
        // clang-format off
        switch (kind) {
            case FaceKind::Front:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {0, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 0}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 0}, {1, 1}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {0, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 0}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 0}, {1, 0}, texture_id});
                break;
            case FaceKind::Left:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 1}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 0}, {1, 0}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 1}, {0, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 1}, {0, 0}, texture_id});
                break;
            case FaceKind::Bottom:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 1}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 1}, {0, 1}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 0}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 0}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 1}, {1, 1}, texture_id});
                break;
            case FaceKind::Right:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 0}, {0, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 0}, {0, 0}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 1}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 0}, {0, 1}, texture_id});
                break;
            case FaceKind::Back:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 1}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 1}, {1, 1}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {0, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 0, 1}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 0, 1}, {0, 1}, texture_id});
                break;
            case FaceKind::Top:
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 0}, {0, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 1}, {0, 0}, texture_id});

                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 1}, {1, 0}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 1, 1, 0}, {1, 1}, texture_id});
                vertices.push_back(BlockVertex{coords + math::vec3f{ 0, 1, 0}, {0, 1}, texture_id});
                break;
        }
        // clang-format on
    }

    void updateModelMatrix() { model_matrix = math::scale(scaling_factor); }
    [[nodiscard]] const math::mat4f& getModelMatrix() const { return model_matrix; }
    [[nodiscard]] const BlockVertex* data() const { return vertices.data(); }
    [[nodiscard]] std::size_t size() const { return vertices.size(); }
    [[nodiscard]] std::size_t bytes() const { return sizeof(BlockVertex) * size(); }

   private:
    std::vector<BlockVertex> vertices{};
    float scaling_factor = 1;
    math::mat4f model_matrix = math::identity<float, 4>();
};

#endif  // !MESH_H_
