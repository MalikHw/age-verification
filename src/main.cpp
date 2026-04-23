#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

using namespace geode::prelude;

class AgeVerificationPopup;
class ManualInputPopup;
class NineAndTenPopup;

// nineandtenpopup
class NineAndTenPopup : public geode::Popup<NineAndTenPopup*> {
protected:
    CCTextInputNode* m_input = nullptr;

    bool setup() override {
        this->setTitle("One Last Check...");

        auto bg = CCScale9Sprite::create("GJ_square05.png", {0, 0, 80, 80});
        bg->setContentSize(m_mainLayer->getContentSize() - CCSize(20, 20));
        bg->setPosition(m_mainLayer->getContentSize() / 2);
        bg->setOpacity(80);
        m_mainLayer->addChild(bg, -1);

        auto windowSize = m_mainLayer->getContentSize();

        auto lbl = CCLabelBMFont::create("What is 9 + 10?\nPlease answer either 19 or 21.", "bigFont.fnt");
        lbl->setScale(0.45f);
        lbl->setPosition(windowSize / 2 + CCPoint(0, 40));
        lbl->setAlignment(kCCTextAlignmentCenter);
        m_mainLayer->addChild(lbl);

        auto inputBg = CCScale9Sprite::create("square02_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({130.f, 36.f});
        inputBg->setPosition(windowSize / 2);
        inputBg->setOpacity(100);
        m_mainLayer->addChild(inputBg);

        m_input = CCTextInputNode::create(120.f, 30.f, "Enter answer...", "bigFont.fnt", 24);
        m_input->setPosition(windowSize / 2);
        m_input->setMaxLabelWidth(120.f);
        m_input->setMaxLabelScale(0.7f);
        m_input->setZOrder(1);
        m_mainLayer->addChild(m_input);

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Submit", "bigFont.fnt", "GJ_button_01.png"),
            this, menu_selector(NineAndTenPopup::onSubmit)
        );
        btn->setScale(0.8f);

        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition(windowSize / 2 + CCPoint(0, -50));
        m_mainLayer->addChild(menu);

        return true;
    }

    void onSubmit(CCObject*) {
        std::string ans = m_input->getString();

        if (ans == "19") {
            FLAlertLayer::create("Hmm...", "We've detected you may be a minor.", "OK")->show();
            ParentalOptionsLayer::create()->show();
            FLAlertLayer::create("Notice", "Please invite a parent to verify this.", "OK")->show();
            this->onClose(nullptr);
        } else if (ans == "21") {
            FLAlertLayer::create("Hmm...", "We've detected you may be a teen.", "OK")->show();
            ParentalOptionsLayer::create()->show();
            this->onClose(nullptr);
        } else {
            FLAlertLayer::create("Invalid Answer", "Invalid answer. Please answer either 19 or 21.", "OK")->show();
        }
    }

public:
    static NineAndTenPopup* create() {
        auto ret = new NineAndTenPopup();
        if (ret->initAnchored(300.f, 200.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// manualinputpopup
class ManualInputPopup : public geode::Popup<ManualInputPopup*> {
protected:
    Slider* m_ageSlider = nullptr;
    Slider* m_yearSlider = nullptr;
    CCLabelBMFont* m_ageLbl = nullptr;
    CCLabelBMFont* m_yearLbl = nullptr;

    int getAge()  { return 1    + (int)roundf(m_ageSlider->getValue()  * 99.f);  }
    int getYear() { return 1924 + (int)roundf(m_yearSlider->getValue() * 101.f); }

    bool setup() override {
        this->setTitle("Manual Age Input");

        auto bg = CCScale9Sprite::create("GJ_square05.png", {0, 0, 80, 80});
        bg->setContentSize(m_mainLayer->getContentSize() - CCSize(20, 20));
        bg->setPosition(m_mainLayer->getContentSize() / 2);
        bg->setOpacity(80);
        m_mainLayer->addChild(bg, -1);

        auto windowSize = m_mainLayer->getContentSize();

        auto ageTitleLbl = CCLabelBMFont::create("Age:", "bigFont.fnt");
        ageTitleLbl->setScale(0.5f);
        ageTitleLbl->setPosition(windowSize / 2 + CCPoint(-90, 60));
        m_mainLayer->addChild(ageTitleLbl);

        m_ageLbl = CCLabelBMFont::create("50", "bigFont.fnt");
        m_ageLbl->setScale(0.5f);
        m_ageLbl->setPosition(windowSize / 2 + CCPoint(90, 60));
        m_mainLayer->addChild(m_ageLbl);

        m_ageSlider = Slider::create(this, menu_selector(ManualInputPopup::onAgeSlider), 0.8f);
        m_ageSlider->setValue(0.49f);
        m_ageSlider->setPosition(windowSize / 2 + CCPoint(0, 45));
        m_mainLayer->addChild(m_ageSlider);

        auto yearTitleLbl = CCLabelBMFont::create("Birth Year:", "bigFont.fnt");
        yearTitleLbl->setScale(0.5f);
        yearTitleLbl->setPosition(windowSize / 2 + CCPoint(-90, 10));
        m_mainLayer->addChild(yearTitleLbl);

        m_yearLbl = CCLabelBMFont::create("1974", "bigFont.fnt");
        m_yearLbl->setScale(0.5f);
        m_yearLbl->setPosition(windowSize / 2 + CCPoint(90, 10));
        m_mainLayer->addChild(m_yearLbl);

        m_yearSlider = Slider::create(this, menu_selector(ManualInputPopup::onYearSlider), 0.8f);
        m_yearSlider->setValue(0.49f);
        m_yearSlider->setPosition(windowSize / 2 + CCPoint(0, -5));
        m_mainLayer->addChild(m_yearSlider);

        auto confirmBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Confirm", "bigFont.fnt", "GJ_button_01.png"),
            this, menu_selector(ManualInputPopup::onConfirm)
        );
        confirmBtn->setScale(0.85f);

        auto menu = CCMenu::create();
        menu->addChild(confirmBtn);
        menu->setPosition(windowSize / 2 + CCPoint(0, -60));
        m_mainLayer->addChild(menu);

        return true;
    }

    void onAgeSlider(CCObject*) {
        m_ageLbl->setString(std::to_string(getAge()).c_str());
        if (getAge() > 65)
            Notification::create("what the", NotificationIcon::Warning)->show();
    }

    void onYearSlider(CCObject*) {
        m_yearLbl->setString(std::to_string(getYear()).c_str());
    }

    void onConfirm(CCObject*) {
        int age  = getAge();
        int year = getYear();
        int calc = 2026 - year;

        if (std::abs(calc - age) > 1) {
            FLAlertLayer::create("Mismatch!", "Values don't match. Try again.", "OK")->show();
            return;
        }

        if (age >= 18) {
            Mod::get()->setSavedValue("verified", true);
            this->onClose(nullptr);
            FLAlertLayer::create("Welcome!", "Welcome then! Adult.", "OK")->show();
        } else {
            this->onClose(nullptr);
            NineAndTenPopup::create()->show();
        }
    }

public:
    static ManualInputPopup* create() {
        auto ret = new ManualInputPopup();
        if (ret->initAnchored(340.f, 220.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ageverificationpopup
class AgeVerificationPopup : public geode::Popup<AgeVerificationPopup*> {
protected:
    bool setup() override {
        this->setTitle("Age Verification");

        auto bg = CCScale9Sprite::create("GJ_square05.png", {0, 0, 80, 80});
        bg->setContentSize(m_mainLayer->getContentSize() - CCSize(20, 20));
        bg->setPosition(m_mainLayer->getContentSize() / 2);
        bg->setOpacity(80);
        m_mainLayer->addChild(bg, -1);

        auto windowSize = m_mainLayer->getContentSize();

        auto desc = CCLabelBMFont::create("Please verify your age\nto continue.", "bigFont.fnt");
        desc->setScale(0.5f);
        desc->setAlignment(kCCTextAlignmentCenter);
        desc->setPosition(windowSize / 2 + CCPoint(0, 50));
        m_mainLayer->addChild(desc);

        auto selfieBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Selfie Check", "bigFont.fnt", "GJ_button_04.png"),
            this, menu_selector(AgeVerificationPopup::onSelfie)
        );
        selfieBtn->setScale(0.75f);

        auto govBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Gov ID Check", "bigFont.fnt", "GJ_button_04.png"),
            this, menu_selector(AgeVerificationPopup::onGovID)
        );
        govBtn->setScale(0.75f);

        auto manualBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Manual Input", "bigFont.fnt", "GJ_button_01.png"),
            this, menu_selector(AgeVerificationPopup::onManual)
        );
        manualBtn->setScale(0.75f);

        auto menu = CCMenu::create();
        menu->addChild(selfieBtn);
        menu->addChild(govBtn);
        menu->addChild(manualBtn);
        menu->alignItemsVerticallyWithPadding(8.f);
        menu->setPosition(windowSize / 2 + CCPoint(0, -10));
        m_mainLayer->addChild(menu);

        return true;
    }

    void onSelfie(CCObject*) {
        FLAlertLayer::create("Unavailable", "Sorry, Persona is hard to implement.", "OK")->show();
    }

    void onGovID(CCObject*) {
        // same thing lol
        FLAlertLayer::create("Unavailable", "Sorry, Persona is hard to implement.", "OK")->show();
    }

    void onManual(CCObject*) {
        this->onClose(nullptr);
        ManualInputPopup::create()->show();
    }

public:
    static AgeVerificationPopup* create() {
        auto ret = new AgeVerificationPopup();
        if (ret->initAnchored(300.f, 240.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// menulayer
class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSavedValue<bool>("verified", false))
            AgeVerificationPopup::create()->show();

        return true;
    }
};
