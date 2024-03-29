#include <canvas/app.h>
#include <canvas/opengl.h>
#include <floats/intersection.h>
#include <hive/locator/physical_file_locator.h>
#include <legion/controllers/top_down_camera_controller.h>
#include <legion/resources/resource_manager.h>
#include <nucleus/file_path.h>
#include <nucleus/win/includes.h>

#include <legion/engine/engine.hpp>
#include <nucleus/optional.hpp>
#include <utility>

#include "ad/app/user_interface.h"
#include "ad/context.hpp"
#include "ad/world/construction_controller.h"
#include "ad/world/entity.h"
#include "ad/world/generator.hpp"
#include "ad/world/prefabs.h"
#include "ad/world/world.h"

#if 0
class AsteroidDefender : public ca::WindowDelegate {
  NU_DELETE_COPY_AND_MOVE(AsteroidDefender);

public:
  AsteroidDefender() : ca::WindowDelegate{"Asteroid Defender"} {}

  ~AsteroidDefender() override = default;

  bool onWindowCreated(ca::Window* window) override {
    prefabs_ = nu::makeScopedPtr<Prefabs>(resource_manager_.get());
    if (!create_prefabs(prefabs_.get())) {
      return false;
    }

    Generator::generate(&world_, prefabs_.get());

    world_camera_.moveTo({0.0f, 0.0f, 5.0f});
    world_camera_.setNearPlane(0.1f);
    world_camera_.setFarPlane(200.0f);

    construction_controller_ = nu::makeScopedPtr<ConstructionController>(&world_, prefabs_.get());

    user_interface_ =
        nu::makeScopedPtr<UserInterface>(construction_controller_.get(), world_.resources());
    if (!user_interface_->initialize(renderer)) {
      return false;
    }

    return true;
  }

  void onWindowResized(const fl::Size& size) override {
    WindowDelegate::onWindowResized(size);

    screen_size_ = size;

    auto aspect_ratio = le::Camera::aspectRatioFromScreenSize(screen_size_);

    world_camera_.setAspectRatio(aspect_ratio);

    //    user_interface_.ui().resize(size);
  }

  void on_mouse_moved(const ca::MouseEvent& event) override {
#if 0
    user_interface_.ui().on_mouse_moved(event);

    current_mouse_position_ = event.pos;

    current_camera_->onMouseMoved(
        le::Camera::convertScreenPositionToClipSpace(event.pos, screen_size_));
#endif  // 0
  }

  bool on_mouse_pressed(const ca::MouseEvent& event) override {
#if 0
    if (user_interface_.ui().on_mouse_pressed(event)) {
      return true;
    }

    if (construction_controller_.is_building() && event.button == ca::MouseEvent::Button::Left) {
      construction_controller_.build();
      return true;
    }

    if (event.button == ca::MouseEvent::Button::Left) {
      auto entity_under_cursor = world_.get_entity_under_cursor();
      if (entity_under_cursor.is_valid()) {
        LOG(Info) << "testing";
      }
    }

//    current_camera_->onMousePressed(
//        event.button, le::Camera::convertScreenPositionToClipSpace(event.pos, screen_size_));
#endif  // 0

    return false;
  }

  void on_mouse_released(const ca::MouseEvent& event) override {
#if 0
    user_interface_.ui().on_mouse_released(event);

    current_camera_->onMouseReleased(
        event.button, le::Camera::convertScreenPositionToClipSpace(event.pos, screen_size_));
#endif  // 0
  }

  void on_mouse_wheel(const ca::MouseWheelEvent& event) override {
#if 0
    user_interface_.ui().on_mouse_wheel(event);

    current_camera_->onMouseWheel(
        {static_cast<F32>(event.wheelOffset.x), static_cast<F32>(event.wheelOffset.y)});
#endif  // 0
  }

  void on_key_pressed(const ca::KeyEvent& event) override {
#if 0
    current_camera_->onKeyPressed(event.key);

    switch (event.key) {
      case ca::Key::LBracket:
        field_of_view_movement_ -= 1.0f;
        break;

      case ca::Key::RBracket:
        field_of_view_movement_ += 1.0f;
        break;

      default:
        break;
    }
#endif  // 0
  }

  void on_key_released(const ca::KeyEvent& event) override {
#if 0
    current_camera_->onKeyReleased(event.key);

    switch (event.key) {
      case ca::Key::LBracket:
        field_of_view_movement_ += 1.0f;
        break;

      case ca::Key::RBracket:
        field_of_view_movement_ -= 1.0f;
        break;

      default:
        break;
    }
#endif  // 0
  }

  void tick(F32 delta) override {
    current_camera_->tick(delta);

    {
      ray_ = world_camera_.createRayForMouse(
          le::Camera::convertScreenPositionToClipSpace(current_mouse_position_, screen_size_));

      fl::Plane world_plane{-fl::Vec3::forward, 0.0f};
      auto result = fl::intersection(world_plane, ray_);
      fl::Vec2 cursor_position{result.position.x, result.position.y};
      world_.set_cursor_position(cursor_position);
      //      construction_controller_.set_cursor_position(cursor_position);
    }

    world_.tick(delta);
    //    user_interface_.tick(delta);
  }

  void onRender(ca::Renderer* renderer) override {
    renderer->state().depth_test(true);

    le::Camera* camera = current_camera_->camera();

    {
      PROFILE("render world")
      world_.render(renderer, camera);
    }

    //    construction_controller_.render(renderer, camera);

    glDisable(GL_DEPTH_TEST);

    {
        PROFILE("render user interface")
        //      user_interface_.ui().render(renderer);
    }

    renderer->state()
        .depth_test(false);
  }

#if 0
    void drawModel(ca::Renderer* renderer, const ca::Vec3& position,
                   const ca::Quaternion& orientation, const ca::Mat4& finalMatrix) {
      ca::Mat4 modelTranslation = translationMatrix(position);
      ca::Mat4 modelRotation{orientation.toRotationMatrix()};
      ca::Mat4 modelScale = ca::scaleMatrix(0.5f);

      ca::Mat4 model = ca::createModelMatrix(modelTranslation, modelRotation, modelScale);

      ca::Mat4 final = finalMatrix * model;

      // renderModel(renderer, m_cube.model, final, m_cube.programId, m_cube.transformUniformId);

      le::renderModel(renderer, *m_model, final);
    }
#endif  // 0

#if 0
    void drawCamera(le::Camera* camera) {
      // drawModel(renderer, camera->position(), camera->orientation(), finalMatrix);

      // Draw the frustum.
      ca::Mat4 camProjection = ca::Mat4::identity;
      ca::Mat4 camView = ca::Mat4::identity;

      camera->updateProjectionMatrix(&camProjection);
      camera->updateViewMatrix(&camView);

      ca::Mat4 camInverse{ca::inverse(camProjection * camView)};

      static ca::Vec4 vertices[] = {
          {-1.0f, -1.0f, -1.0f, 1.0f},  //
          {+1.0f, -1.0f, -1.0f, 1.0f},  //
          {+1.0f, +1.0f, -1.0f, 1.0f},  //
          {-1.0f, +1.0f, -1.0f, 1.0f},  //
          {-1.0f, -1.0f, +1.0f, 1.0f},  //
          {+1.0f, -1.0f, +1.0f, 1.0f},  //
          {+1.0f, +1.0f, +1.0f, 1.0f},  //
          {-1.0f, +1.0f, +1.0f, 1.0f},  //
      };

      nu::DynamicArray<ca::Vec4> pos;
      for (ca::Vec4& v : vertices) {
        auto p = camInverse * v;
        p /= p.w;
        pos.emplaceBack(p);
      }
    }
#endif  // 0

private:
  nu::ScopedPtr<le::ResourceManager> resource_manager_;

  nu::ScopedPtr<Prefabs> prefabs_;
  World world_;

  nu::ScopedPtr<ConstructionController> construction_controller_;

  nu::ScopedPtr<UserInterface> user_interface_;

  F32 field_of_view_movement_ = 0.0f;

  le::Camera world_camera_{fl::degrees(45.0f), {0.0f, 0.0f, 1.0f}};
  le::TopDownCameraController world_camera_controller_{
      &world_camera_, {fl::Vec3::forward, 0.0f}, 25.0f};

  le::CameraController* current_camera_ = &world_camera_controller_;

  fl::Ray ray_{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

  // Client size of the area we're rendering the world into:
  fl::Size screen_size_;

  // Position of the cursor on the screen in range ([0..m_size.width], [0..m_size.height]}.
  fl::Pos current_mouse_position_;
};
#endif  // 0

namespace ad {

class WorldLayer : public le::EngineLayer {
public:
  explicit WorldLayer(nu::ScopedRefPtr<Context> context) : context_{std::move(context)} {}

protected:
  bool on_initialize() override {
    // Set up resource manager.
    auto assets_path = nu::getCurrentWorkingDirectory() / "assets";
    LOG(Info) << "Assets path: " << assets_path.getPath();
    resource_manager().set_locator(nu::make_scoped_ref_ptr<hi::PhysicalFileLocator>(assets_path));

    // Set up the prefabs.

    setup_prefabs(&context_->prefabs());

    // Load needed assets.

    context_->world().initialize(&resource_manager());

    cursor_model_ = resource_manager().get_render_model("cursor.obj");
    if (!cursor_model_) {
      LOG(Error) << "Could not load cursor model.";
      return false;
    }

    // Populate the world.

    if (!populate_world(&context_->world(), &context_->prefabs())) {
      return false;
    }

    // Set state of entities.

    world_camera_.moveTo({0.0f, 0.0f, 5.0f});
    world_camera_.setNearPlane(0.1f);
    world_camera_.setFarPlane(200.0f);

    return true;
  }

  void on_resize(const fl::Size& size) override {
    EngineLayer::on_resize(size);

    world_camera_controller_.set_screen_size(size);

    auto aspect_ratio = le::Camera::aspectRatioFromScreenSize(size);
    world_camera_.setAspectRatio(aspect_ratio);
  }

  void on_mouse_moved(const ca::MouseEvent& evt) override {
    current_mouse_position_ = evt.pos;
    world_camera_controller_.on_mouse_moved(evt);
  }

  bool on_mouse_pressed(const ca::MouseEvent& evt) override {
    return world_camera_controller_.on_mouse_pressed(evt);
  }

  void on_mouse_released(const ca::MouseEvent& evt) override {
    if (context_->construction_controller().is_building()) {
      if (evt.button == ca::MouseEvent::Button::Left) {
        context_->construction_controller().build();
        return;
      } else if (evt.button == ca::MouseEvent::Button::Right) {
        context_->construction_controller().cancel_building();
        // Don't return here, because the right mouse button is also used by the camera controller.
      }
    }

    world_camera_controller_.on_mouse_released(evt);
  }

  void on_mouse_wheel(const ca::MouseWheelEvent& evt) override {
    world_camera_controller_.on_mouse_wheel(evt);
  }

  void on_key_pressed(const ca::KeyEvent& evt) override {
    world_camera_controller_.on_key_pressed(evt);
  }

  void on_key_released(const ca::KeyEvent& evt) override {
    switch (evt.key) {
      case ca::Key::M:
        context_->construction_controller().start_building(EntityType::Miner);
        return;

      case ca::Key::T:
        context_->construction_controller().start_building(EntityType::Turret);
        return;

      case ca::Key::H:
        context_->construction_controller().start_building(EntityType::Hub);
        return;

      case ca::Key::Escape:
        context_->construction_controller().cancel_building();
        return;
    }

    world_camera_controller_.on_key_released(evt);
  }

  void update(F32 delta) override {
    world_camera_controller_.tick(delta);

    {
      fl::Vec2 mouse_position_in_clip_space =
          le::Camera::convertScreenPositionToClipSpace(current_mouse_position_, size_in_pixels());
      auto ray = world_camera_.createRayForMouse(mouse_position_in_clip_space);

      fl::Plane world_plane{{0.0f, 0.0f, 1.0f}, 0.0f};
      auto result = fl::intersection(world_plane, ray);

      // DCHECK(result.didIntersect);
      mouse_position_in_world_ = result.position;

      context_->world().set_cursor_position(mouse_position_in_world_.xy());
      context_->construction_controller().set_cursor_position(mouse_position_in_world_.xy());
    }

    context_->world().tick(delta);
  }

  void on_render() override {
    context_->world().render(&renderer(), &world_camera_, &context_->construction_controller());

    auto projection = fl::Mat4::identity;
    auto view = fl::Mat4::identity;

    world_camera_.updateProjectionMatrix(&projection);
    world_camera_.updateViewMatrix(&view);
    auto model = fl::translation_matrix(mouse_position_in_world_);

    auto mvp = projection * view * model;

    le::renderModel(&renderer(), *cursor_model_, mvp);
  }

private:
  static bool setup_prefabs(Prefabs* prefabs) {
    if (!prefabs->set(EntityType::CommandCenter,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->flags = ENTITY_FLAG_LINKABLE;

                        storage->electricity.electricity_delta = 100;
                        storage->building.selection_radius = 2.5f;

                        auto* model = resource_manager->get_render_model("command_center.obj");

                        storage->render.model = model;
                        if (!storage->render.model) {
                          return false;
                        }

                        return true;
                      })) {
      return false;
    }

    if (!prefabs->set(EntityType::Miner,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->flags = ENTITY_FLAG_NEEDS_LINK;

                        storage->electricity.electricity_delta = -5;

                        storage->building.selection_radius = 1.5f;

                        storage->mining.cycle_duration = 100.0f;
                        storage->mining.mineral_amount_per_cycle = 10;

                        storage->render.model = resource_manager->get_render_model("miner.obj");
                        if (!storage->render.model) {
                          return false;
                        }

                        return true;
                      })) {
      return false;
    }

    if (!prefabs->set(EntityType::Turret,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->flags = ENTITY_FLAG_NEEDS_LINK;

                        storage->electricity.electricity_delta = -5;

                        storage->building.selection_radius = 1.5f;

                        storage->render.model = resource_manager->get_render_model("turret.obj");
                        if (!storage->render.model) {
                          return false;
                        }

                        return true;
                      })) {
      return false;
    }

    if (!prefabs->set(EntityType::Hub,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->flags = ENTITY_FLAG_NEEDS_LINK | ENTITY_FLAG_LINKABLE;

                        storage->electricity.electricity_delta = -1;

                        storage->building.selection_radius = 1.5f;

                        storage->render.model = resource_manager->get_render_model("hub.obj");
                        if (!storage->render.model) {
                          return false;
                        }

                        return true;
                      })) {
      return false;
    }

    if (!prefabs->set(EntityType::Asteroid,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->flags = ENTITY_FLAG_MINABLE;

                        storage->render.model = resource_manager->get_render_model("asteroid.obj");
                        if (!storage->render.model) {
                          return false;
                        }

                        storage->building.selection_radius = 1.7f;

                        return true;
                      })) {
      return false;
    }

    if (!prefabs->set(EntityType::EnemyFighter,
                      [](le::ResourceManager* resource_manager, Entity* storage) -> bool {
                        storage->render.model = resource_manager->get_render_model("enemy.obj");
                        if (!storage->render.model) {
                          return false;
                        }

                        return true;
                      })) {
      return false;
    }

    return true;
  }

  nu::ScopedRefPtr<Context> context_;

  le::Camera world_camera_{fl::degrees(70.0f), {0.0f, 0.0f, 1.0f}};
  le::TopDownCameraController world_camera_controller_{
      &world_camera_, {fl::Vec3::forward, 0.0f}, 45.0f};

  le::RenderModel* cursor_model_ = nullptr;
  fl::Vec3 mouse_position_in_world_ = fl::Vec3::zero;

  fl::Pos current_mouse_position_ = {0, 0};
};

}  // namespace ad
