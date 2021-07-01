#pragma once
#include <TGUI/TGUI.hpp>
#include <string>

bool paused = false;

void unpauseGame(tgui::GuiSFML& gui);
void pauseGame(tgui::GuiSFML& gui);

void initTGUI(tgui::GuiSFML &gui)
{

	tgui::Panel::Ptr pausePanel = tgui::Panel::create({ 320, 255 });
	tgui::Button::Ptr resumeButton = tgui::Button::create("Resume Game");
	tgui::Label::Ptr pausedText = tgui::Label::create("Game Paused");
	tgui::Label::Ptr creditText = tgui::Label::create("Made by debug420\n"
		"Discord: CEO of Africa#0591\n"
		"Made using C++, SFML and TGUI\n"
		"You may use the source in however way you wish.\n\n"
		"Github Rep Link: \nhttps:\/\/github.com\/debug420\/Pong"
	);

	resumeButton->setPosition("50%", 220);
	resumeButton->setSize("90%", 40);
	resumeButton->setOrigin(0.5, 0.5);
	resumeButton->onMouseRelease([&]() { unpauseGame(gui); });
	
	pausedText->setPosition("50%", 35);
	pausedText->setSize(100, 20);
	pausedText->setOrigin(0.5, 0.5);
	pausedText->getRenderer()->setTextColor({ 200, 200, 200 });

	creditText->setPosition("50%", 160);
	creditText->setSize("90%", 200);
	creditText->setOrigin(0.5, 0.5);
	creditText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	creditText->getRenderer()->setTextColor({ 190, 190, 190 });

	pausePanel->setPosition("50%", "50%");
	pausePanel->setOrigin(0.5, 0.5);
	pausePanel->getRenderer()->setBackgroundColor({ 20, 20, 20 });
	pausePanel->add(creditText);
	pausePanel->add(pausedText);
	pausePanel->add(resumeButton);

	gui.add(pausePanel, "panel");
}

void pauseGame(tgui::GuiSFML& gui)
{
	paused = true;
	gui.get<tgui::Panel>("panel")->setVisible(true);
}

void unpauseGame(tgui::GuiSFML& gui)
{
	paused = false;
	gui.get<tgui::Panel>("panel")->setVisible(false);
}
