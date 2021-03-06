#include "ad/App/user_interface.h"

#include <silhouette/image.h>

#include <cstdio>

#include "ad/World/construction_controller.h"
#include "ad/World/world.h"
#include "elastic/Views/button_view.h"
#include "elastic/Views/label_view.h"
#include "elastic/Views/linear_sizer_view.h"
#include "nucleus/Streams/file_input_stream.h"
#include "nucleus/file_path.h"

namespace {

struct BuildClickListener : public el::ButtonView::OnClickListener {
  ConstructionController* construction_controller;
  EntityType entity_type;

  BuildClickListener(ConstructionController* construction_controller, EntityType entity_type)
    : construction_controller{construction_controller}, entity_type{entity_type} {}

  ~BuildClickListener() override = default;

  void onButtonClicked(el::ButtonView* NU_UNUSED(sender)) override {
    construction_controller->start_building(entity_type);
  }
};

}  // namespace

UserInterface::UserInterface(ConstructionController* construction_controller, Resources* resources)
  : resources_{resources}, construction_controller_{construction_controller} {}

auto UserInterface::initialize(ca::Renderer* renderer) -> bool {
  if (!ui_.initialize(renderer)) {
    return false;
  }

#if OS(POSIX)
  // nu::FileInputStream fontStream{nu::FilePath{"/Library/Fonts/Arial Unicode.ttf"}};
  nu::FileInputStream font_stream{nu::FilePath{"/usr/share/fonts/TTF/DejaVuSans.ttf"}};
#elif OS(MACOSX)
  nu::FileInputStream font_stream{nu::FilePath{R"(/Library/Fonts/Arial.ttf)"}};
#else
  nu::FileInputStream font_stream{nu::FilePath{R"(C:\Windows\Fonts\Arial.ttf)"}};
#endif
  font_.load(&font_stream, renderer, 30);

  {
    nu::FileInputStream stream{nu::FilePath{R"(C:\Code\elastic\assets\button_bg.png)"}};
    si::Image background;
    if (!background.load_from_png(&stream)) {
      return false;
    }

    auto texture_id =
        renderer->createTexture(ca::TextureFormat::RGBA, background.size(),
                                background.data().data(), background.data().size(), false);
    if (!texture_id) {
      return false;
    }

    button_background_ = el::Image{texture_id, background.size()};
  }

  return create_ui(&ui_, &font_);
}

auto UserInterface::tick(F32 delta) -> void {
  ui_.tick(delta);

  Char buf[128];

#if COMPILER(MSVC)
  sprintf_s(buf, sizeof(buf), "electricity: %d", resources_->electricity());
#else
  sprintf(buf, "electricity: %d", resources_->electricity());
#endif
  electricity_label_->setLabel(buf);

#if COMPILER(MSVC)
  sprintf_s(buf, sizeof(buf), "minerals: %d", resources_->minerals());
#else
  sprintf(buf, "minerals: %d", resources_->minerals());
#endif
  minerals_label_->setLabel(buf);
}

auto UserInterface::create_ui(el::Context* context, el::Font* NU_UNUSED(font)) -> bool {
  el::ContextView* root_view = context->getRootView();

  auto resources_container = new el::LinearSizerView{&ui_};
  root_view->addChild(resources_container);
  resources_container->setOrientation(el::Orientation::Vertical);
  resources_container->setHorizontalAlignment(el::Alignment::Left);
  resources_container->setVerticalAlignment(el::Alignment::Top);

  electricity_label_ = new el::LabelView{&ui_, "0", &font_};
  //  resourcesContainer->addChild(m_electricityLabel);

  minerals_label_ = new el::LabelView{&ui_, "0", &font_};
  //  resourcesContainer->addChild(m_mineralsLabel);

  auto button_container = new el::LinearSizerView{&ui_};
  root_view->addChild(button_container);
  button_container->setOrientation(el::Orientation::Vertical);
  button_container->setHorizontalAlignment(el::Alignment::Left);
  button_container->setVerticalAlignment(el::Alignment::Bottom);
  button_container->setMinSize(fl::Size{250, 0});

  add_build_button(button_container, EntityType::CommandCenter, "Command Center");
  add_build_button(button_container, EntityType::Miner, "Miner");

  return true;
}

auto UserInterface::add_build_button(el::GroupView* container, EntityType entity_type,
                                     const nu::StringView& label) -> void {
  auto click_listener = new BuildClickListener{construction_controller_, entity_type};
  auto button = new el::ButtonView{&ui_, label, click_listener};
  button->setFont(&font_);
  button->setMinSize(fl::Size{0, 45});
  button->setExpansion(el::Expansion::Horizontal);
  button->background(button_background_);
  container->addChild(button);
}
