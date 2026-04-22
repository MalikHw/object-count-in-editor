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

        auto lenLabel = CCLabelBMFont::create("Length: 0:00", "chatFont.fnt");
        lenLabel->setScale(0.5f);
        lenLabel->setOpacity(127);
        lenLabel->setAnchorPoint({ 0.f, 1.f });
        lenLabel->setPosition({ 5.f, winSize.height - 5.f - (objLabel->getContentSize().height * 0.5f + 2.f) });
        lenLabel->setZOrder(999);
        this->addChild(lenLabel);
        m_fields->m_lengthLabel = lenLabel;

        this->schedule(schedule_selector(MyEditorUI::updateOverlayLabels), 0.25f);

        return true;
    }

    void updateOverlayLabels(float dt) {
        auto el = m_editorLayer;
        if (!el || !m_fields->m_objectCountLabel || !m_fields->m_lengthLabel) return;

        int objCount = el->m_objectCount.value();
        m_fields->m_objectCountLabel->setString(
            fmt::format("Objects: {}", objCount).c_str()
        );

        // m_levelLength on GJBaseGameLayer is the farthest X position in units
        // timeForPos converts that to seconds based on speed portals
        float totalSecs = el->timeForPos({ el->m_levelLength, 0.f }, 0, 0, false, 0);
        int mins = static_cast<int>(totalSecs) / 60;
        int secs = static_cast<int>(totalSecs) % 60;

        m_fields->m_lengthLabel->setString(
            fmt::format("Length: {}:{:02d}", mins, secs).c_str()
        );
    }
};
