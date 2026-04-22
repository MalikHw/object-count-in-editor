#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto label = CCLabelBMFont::create("EditorUI", "chatFont.fnt");
        label->setScale(0.5f);
        label->setOpacity(127); // 0.5 opacity (255 x 0.5)
        label->setAnchorPoint({ 0.f, 1.f });
        label->setPosition({ 5.f, winSize.height - 5.f });
        label->setZOrder(999);
        this->addChild(label);

        return true;
    }
};
