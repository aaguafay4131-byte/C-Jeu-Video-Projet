#include "AboutScreen.hpp"

AboutScreen::AboutScreen(sf::Font& gameFont) : font(gameFont) {
    background.setSize(sf::Vector2f(800, 600));
    background.setFillColor(sf::Color(40, 40, 60));
    
    title.setFont(font);
    title.setString("A PROPOS");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(300, 50);
    
    std::vector<std::string> lines = {
        "DINO RUNNER - Jeu d'evitement d'obstacles",
        "",
        "Methodologie du jeu:",
        "- Vous controlez un dinosaure qui doit courir",
        "- Evitez les obstacles (cactus et fleches volantes)",
        "- Utilisez ESPACE pour sauter",
        "- Utilisez FLECHE BAS pour vous baisser",
        "- Vous avez 3 vies pour terminer le parcours",
        "- Le temps est limite! 60 secondes pour survivre",
        "- La vitesse augmente avec le score",
        "",
        "Objectif: Survivre le plus longtemps possible",
        "et atteindre un score eleve!"
    };
    
    float yPos = 120;
    for (const auto& line : lines) {
        sf::Text text;
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        if (line.empty()) text.setString(" ");
        text.setPosition(100, yPos);
        aboutText.push_back(text);
        yPos += 25;
    }
    
    backInstruction.setFont(font);
    backInstruction.setString("Appuyez sur ECHAP pour retourner au menu principal");
    backInstruction.setCharacterSize(18);
    backInstruction.setFillColor(sf::Color(200, 200, 200));
    backInstruction.setPosition(250, 550);
}

void AboutScreen::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(title);
    for (auto& text : aboutText) {
        window.draw(text);
    }
    window.draw(backInstruction);
}