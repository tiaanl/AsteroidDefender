#pragma once

#include <legion/resources/resource_manager.h>
#include <nucleus/containers/hash_map.h>
#include <nucleus/function.h>

#include "ad/world/entity.h"

class Prefabs {
public:
  explicit Prefabs(le::ResourceManager* resource_manager) : resource_manager_{resource_manager} {}

  Entity* get(EntityType entity_type) {
    auto result = prefabs_.find(entity_type);
    if (!result.was_found()) {
      return nullptr;
    }

    return &result.value();
  }

  bool set(EntityType entity_type, nu::Function<bool(le::ResourceManager*, Entity*)>&& func) {
    auto result = prefabs_.insert(entity_type, {});
    Entity* storage = &result.value();

    storage->type = entity_type;

    return func(resource_manager_, storage);
  }

private:
  le::ResourceManager* resource_manager_;
  nu::HashMap<EntityType, Entity> prefabs_;
};
