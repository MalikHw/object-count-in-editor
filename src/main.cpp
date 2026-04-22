#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        CCLabelBMFont* m_objectCountLabel = nullptr;
        CCLabelBMFont* m_lengthLabel = nullptr;
    };

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto objLabel = CCLabelBMFont::create("Objects: 0", "chatFont.fnt");
        objLabel->setScale(0.5f);
        objLabel->setOpacity(127);
        objLabel->setAnchorPoint({ 0.f, 1.f });
        objLabel->setPosition({ 5.f, winSize.height - 5.f });
        objLabel->setZOrder(999);
        this->addChild(objLabel);
        m_fields->m_objectCountLabel = objLabel;

        auto lenLabel = CCLabelBMFont::create("Length: -", "chatFont.fnt");
        lenLabel->setScale(0.5f);
        lenLabel->setOpacity(127);
        lenLabel->setAnchorPoint({ 0.f, 1.f });
        lenLabel->setPosition({ 5.f, winSize.height - 5.f - (objLabel->getContentSize().height * 0.5f + 2.f) });
        lenLabel->setZOrder(999);
        this->addChild(lenLabel);
        m_fields->m_lengthLabel = lenLabel;

        this->schedule(schedule_selector(MyEditorUI::updateOverlayLabels), 0.5f);

        return true;
    }

    void updateOverlayLabels(float dt) {
        auto el = m_editorLayer;
        if (!el || !m_fields->m_objectCountLabel || !m_fields->m_lengthLabel) return;

        int objCount = el->m_objectCount.value();
        m_fields->m_objectCountLabel->setString(
            fmt::format("Objects: {}", objCount).c_str()
        );

        static constexpr const char* lengthNames[] = {
            "Tiny", "Short", "Medium", "Long", "XL", "???"
        };
        int lenIdx = el->m_level->m_levelLength;
        if (lenIdx < 0 || lenIdx > 4) lenIdx = 5;
        m_fields->m_lengthLabel->setString(
            fmt::format("Length: {}", lengthNames[lenIdx]).c_str()
        );
    }
};
