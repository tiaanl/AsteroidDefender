#pragma once

#include "ad/world/entity.h"
#include "ad/world/entity_list.hpp"
#include "ad/world/resources.h"

namespace ad {

struct ResourceSystem {
  Resources* resources;

  explicit ResourceSystem(Resources* resources) : resources{resources} {}

  auto tick(EntityList& entities, F32 delta) -> void {
    I32 totalMinerals = 0;
    I32 totalElectricity = 0;

    for (auto& entity : entities) {
      // Electricity

      totalElectricity += entity.electricity.electricity_delta;

      // Mining

      entity.mining.time_since_last_cycle += delta;
      if (entity.mining.time_since_last_cycle >= entity.mining.cycle_duration) {
        entity.mining.time_since_last_cycle -= entity.mining.cycle_duration;

        totalMinerals += entity.mining.mineral_amount_per_cycle;
      }
    }

    resources->set_electricity(totalElectricity);
    resources->add_minerals(totalMinerals);
  }
};

}  //
