#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

static const char* lengthNames[] = { "Tiny", "Short", "Medium", "Long", "XL", "???" };

static CCPoint anchorForPos(std::string const& p) {
    if (p == "top-center" || p == "bottom-center") return { 0.5f, 1.f };
    if (p == "top-right" || p == "center-right" || p == "bottom-right") return { 1.f, 1.f };
    return { 0.f, 1.f };
}

static CCPoint basePos(std::string const& p, CCSize ws, float lh) {
    if (p == "top-left") return { 5.f, ws.height - 5.f };
    if (p == "top-center") return { ws.width / 2.f, ws.height - 5.f };
    if (p == "top-right") return { ws.width - 5.f, ws.height - 5.f };
    if (p == "center-left") return { 5.f, ws.height / 2.f + lh };
    if (p == "center-right") return { ws.width - 5.f, ws.height / 2.f + lh };
    if (p == "bottom-left") return { 5.f, 5.f + lh * 2.f };
    if (p == "bottom-center") return { ws.width / 2.f, 5.f + lh * 2.f };
    if (p == "bottom-right") return { ws.width - 5.f, 5.f + lh * 2.f };
    return { 5.f, ws.height - 5.f };
}

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        CCLabelBMFont* objLabel = nullptr;
        CCLabelBMFont* lenLabel = nullptr;
    };

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto mod = Mod::get();

        auto ol = CCLabelBMFont::create("Objects: 0", "chatFont.fnt");
        ol->setZOrder(999);
        this->addChild(ol);
        m_fields->objLabel = ol;

        auto ll = CCLabelBMFont::create("Length: -", "chatFont.fnt");
        ll->setZOrder(999);
        this->addChild(ll);
        m_fields->lenLabel = ll;

        applySettings();

        listenForSettingChanges<bool>("enabled", [this](bool) { applySettings(); });
        listenForSettingChanges<double>("size", [this](double) { applySettings(); });
        listenForSettingChanges<int64_t>("opacity", [this](int64_t) { applySettings(); });
        listenForSettingChanges<std::string>("show", [this](std::string const&) { applySettings(); });
        listenForSettingChanges<std::string>("position", [this](std::string const&) { applySettings(); });

        this->schedule(schedule_selector(MyEditorUI::tick), 0.5f);
        return true;
    }

    void applySettings() {
        auto mod = Mod::get();
        auto ol = m_fields->objLabel;
        auto ll = m_fields->lenLabel;
        if (!ol || !ll) return;

        bool enabled = mod->getSettingValue<bool>("enabled");
        float sz = static_cast<float>(mod->getSettingValue<double>("size"));
        int op = static_cast<int>(mod->getSettingValue<int64_t>("opacity"));
        auto show = mod->getSettingValue<std::string>("show");

        bool showObj = enabled && (show == "objects" || show == "both");
        bool showLen = enabled && (show == "length" || show == "both");

        ol->setVisible(showObj);
        ll->setVisible(showLen);
        ol->setScale(sz);
        ll->setScale(sz);
        ol->setOpacity(op);
        ll->setOpacity(op);

        auto ws = CCDirector::sharedDirector()->getWinSize();
        auto p = mod->getSettingValue<std::string>("position");
        float lh = ol->getContentSize().height * sz + 2.f;
        auto anchor = anchorForPos(p);
        auto bp = basePos(p, ws, lh);

        ol->setAnchorPoint(anchor);
        ol->setPosition(bp);
        ll->setAnchorPoint(anchor);
        ll->setPosition({ bp.x, bp.y - lh });
    }

    void tick(float) {
        auto el = m_editorLayer;
        if (!el) return;

        auto show = Mod::get()->getSettingValue<std::string>("show");

        if (show == "objects" || show == "both")
            m_fields->objLabel->setString(
                fmt::format("Objects: {}", el->m_objectCount.value()).c_str()
            );

        if (show == "length" || show == "both") {
            int idx = el->m_level->m_levelLength;
            if (idx < 0 || idx > 4) idx = 5;
            auto lenStr = el->m_isPlatformer ? "Plat." : lengthNames[idx];
            m_fields->lenLabel->setString(fmt::format("Length: {}", lenStr).c_str());
        }
    }
};
