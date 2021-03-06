#pragma once

#include <ad/world/Systems/movement_system.h>
#include <ad/world/Systems/resource_system.h>
#include <ad/world/entity.h>
#include <ad/world/resources.h>
#include <legion/world/camera.h>
#include <nucleus/containers/dynamic_array.h>
#include <nucleus/macros.h>

namespace ca {
class Renderer;
}

namespace hi {
class ResourceManager;
}

class World {
public:
  NU_DELETE_COPY_AND_MOVE(World);

  World() = default;
  ~World() = default;

  Resources* resources() {
    return &resources_;
  }

  void clear();
  EntityId add_entity_from_prefab(Entity* prefab, const fl::Vec2& position);

  void set_cursor_position(const fl::Vec2& position);
  NU_NO_DISCARD EntityId get_entity_under_cursor() const;

  void tick(F32 delta);
  void render(ca::Renderer* renderer, le::Camera* camera);

private:
  fl::Vec2 cursor_position_ = fl::Vec2::zero;

  EntityList entities_;

  Resources resources_;

  MovementSystem movement_system_;
  ResourceSystem resource_system_{&resources_};
};
