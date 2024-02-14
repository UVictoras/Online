#include "GameManager.h"
#include "Math.h"
#include <iostream>
#include <sstream>
#include <vector>
#define NOMINMAX
#include <winsock2.h>

using json = nlohmann::json;
using namespace std;
sf::Event pressed;
json jClient; //json message to send to server

GameManager* GameManager::pInstance = nullptr;

/*
-----------------------------------------------------------------------
|      Following are the functions used in the EventManager maps      |
-----------------------------------------------------------------------
*/

void EventCloseWindow()
{
    GameManager::Get()->CloseWindow();
}

void EventPlaceSign()
{
    GameManager::Get()->PlaceSign();
}

/*
-----------------------------------------------------------------------
|   Following are the methods corresponding to the GameManager Class  |
-----------------------------------------------------------------------
*/

GameManager::GameManager(SOCKET sSock) : oWindow(sf::VideoMode(920, 920), "Casse-Brique") // Calling RenderWindow constructor for our game window
{
    m_bWon1 = false;
    m_bWon2 = false;
    m_bDraw = false;
    IsEventInit = false;
    m_iTurn = 0;

    m_jServ["GameReady"] = false;

    m_rBackground = new GameObject(true, 0, 0, 920, 920, sf::Color::White);

    CreateGrid();
    CreateSign();

    m_pPlayers = new Player();

    sock = sSock;

    // create a JSON object
    
    std::string textj;

    // std::stringstream box_message;
    // box_message << "UwU " << j;
    // std::string currency = j["object"]["currency"];
    // int value = j["object"]["value"];
    // std::stringstream new_box_message;
    // new_box_message << "The value of " << currency << " is " << value;


    /*TextureManager::Get()->CreateTexture("img/blank.png", m_tTextureBlank);
    TextureManager::Get()->CreateTexture("img/x.png", m_tTextureX);
    TextureManager::Get()->CreateTexture("img/circle.png", m_tTextureCircle);*/
}

void GameManager::CloseWindow()
{
    oWindow.close();
}

void GameManager::GetJSON(json jServ) { 
    m_jServ = jServ;
    m_iTurn = m_jServ["PlayerTurn"];
    m_pPlayers->m_iId = m_jServ["Id"];
}

void GameManager::SendJSON(int cell)  
{
        jClient.clear();
        m_jClient["Name"] = m_pPlayers->m_sName;
        m_jClient["Cell"] = cell;
        m_jClient["Id"] = m_pPlayers->m_iId;
        std::string jtext = m_jClient.dump() + "\n";
        // send json to server
        //std::cout << "chaussette" << sock << std::endl;
        int bytesSent = send(sock, jtext.c_str(), static_cast<int>(strlen(jtext.c_str())), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            int error = WSAGetLastError();
            std::cout << "erreur:" << error;
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                closesocket(sock);
                WSACleanup();
            }
        }
    }


void GameManager::PlaceSign()
{
    for (Case* cCase : m_cCasesList)
    {
        if (Math::IsInsideInterval(static_cast<float>(vLocalPosition.x), cCase->m_fX, cCase->m_fX + cCase->m_fSizeL) == true)
        {
            if (Math::IsInsideInterval(static_cast<float>(vLocalPosition.y), cCase->m_fY, cCase->m_fY + cCase->m_fSizeH) == true)
            {
                //std::cout << "click click" << m_pPlayers->m_iId << "  turn" << m_iTurn<< std::endl;
                if (m_iTurn == 1)
                {
                    //std::cout << "suuui" << std::endl;
                    if (m_gCasesBack[cCase->m_iIndex] == nullptr) {
                        //std::cout << "suuui" << std::endl;
                        SendJSON(cCase->m_iIndex);
                    }
                }
                // reset json and fill it with the cell player interacted with
            }
        }
    }
}

void GameManager::UpdateGrid(json servJSON) {
    if (servJSON["ValidMove"]) {
        for (int i = 0; i < 9; i++) {
            switch (int(servJSON["Grid"][i])) {
            case 0:
                m_gCasesBack[i] = nullptr;
            case 1:
                m_gCasesBack[i] = new GameObject(false, static_cast<float>(i % 3 * (290 + 25)) , static_cast<float>(i / 3 * (290 + 25)), 290.0, 290.0, sf::Color::White);
            case 2:
                m_gCasesBack[i] = new GameObject(true, static_cast<float>(i + 290 / 2 - 100), static_cast<float>(i + 290 / 2 - 100), 200.0, 200.0, sf::Color::Red);
            default:
                break;
            }
        }
    }
}


void GameManager::CreateGrid()
{
    //150, 150, (640 + (i * 170)) - ((i / 4) * 170) * 4, 185 + ((i / 4) * 170)

    for (int i = 0; i < 9; i++)
    {
        m_cCasesList[i] = new Case(true, static_cast<float>(i % 3 * (290 + 25)), static_cast<float>(i / 3 * (290 + 25)), i, 290, 290, sf::Color::Black, m_tTextureBlank);
    }
}

void GameManager::CreateSign()
{
    for (int i = 0; i < 9; i++)
    {
        //m_gCasesBack[i] = new GameObject(true, i % 3 * (290 + 25), i / 3 * (290 + 25), 290, 290, sf::Color::White);
        m_gCasesBack[i] = nullptr;
    }
}


void GameManager::CheckWin()
{
    m_bWon1 = false;
    m_bWon2 = false;

    // VERIFICATION COLUMN
    for (int x = 0; x < 3; x++) {
        if (m_gCasesBack[x] != nullptr and m_gCasesBack[x + 3] != nullptr and m_gCasesBack[x + 6] != nullptr) {
            if (m_gCasesBack[x]->m_bType == m_gCasesBack[x + 3]->m_bType and m_gCasesBack[x + 6]->m_bType == m_gCasesBack[x]->m_bType) {
                if (m_gCasesBack[x]->m_bType == true)
                    m_bWon1 = true;
                else
                    m_bWon2 = true;
            }
        }
    }

    // VERIFICATION LINE
    for (int y = 0; y < 3; y++) {
        if (m_gCasesBack[y * 3] != nullptr and m_gCasesBack[y * 3 + 1] != nullptr and m_gCasesBack[y * 3 + 2] != nullptr) {
            if (m_gCasesBack[y * 3]->m_bType == m_gCasesBack[y * 3 + 1]->m_bType and m_gCasesBack[y * 3 + 2]->m_bType == m_gCasesBack[y * 3]->m_bType) {
                if (m_gCasesBack[y * 3]->m_bType == true)
                    m_bWon1 = true;
                else
                    m_bWon2 = true;
            }
        }
    }

    // VERIFICATION DIAGONAL
    if (m_gCasesBack[0] != nullptr and m_gCasesBack[4] != nullptr and m_gCasesBack[8] != nullptr) {
        if (m_gCasesBack[0]->m_bType == m_gCasesBack[4]->m_bType and m_gCasesBack[8]->m_bType == m_gCasesBack[0]->m_bType) {
            if (m_gCasesBack[0]->m_bType == true)
                m_bWon1 = true;
            else
                m_bWon2 = true;
        }
    }
    if (m_gCasesBack[2] != nullptr and m_gCasesBack[4] != nullptr and m_gCasesBack[6] != nullptr) {
        if (m_gCasesBack[2]->m_bType == m_gCasesBack[4]->m_bType and m_gCasesBack[6]->m_bType == m_gCasesBack[2]->m_bType) {
            if (m_gCasesBack[2]->m_bType == true)
                m_bWon1 = true;
            else
                m_bWon2 = true;
        }
    }

}

void GameManager::CheckDraw()
{
    for (GameObject* cCase : m_gCasesBack)
    {
        if (cCase == nullptr) {
            m_bDraw = false;
            return;
        }

    }
    m_bDraw = true;
}


bool GameManager::IsFullGrid()
{
    for (Case* cCase : m_cCasesList)
    {
        if (cCase->m_bIsFull == false)
        {
            return false;
        }
    }
    return true;
}

void GameManager::GetName() {
    // EventManager::Get()->AddComponent(sf::Event::EventType::KeyPressed, sf::Keyboard::Key::Enter, &EventCloseWindow);
    sf::RenderWindow window(sf::VideoMode(400, 200), "Entrer le nom d'utilisateur");

    sf::Font font;
    if (!font.loadFromFile("srcs/font/Roboto-Black.ttf")) {
        return;
    }

    sf::Text instruction("Entrez votre nom d'utilisateur :", font, 20);
    instruction.setPosition(10, 10);

    sf::Text inputText("", font, 20);
    inputText.setPosition(10, 50);

    std::string username;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char enteredChar = char(event.text.unicode);
                    if (enteredChar == '\b' && !username.empty()) {
                        // Backspace : supprimer le dernier caractère
                        username.pop_back();
                    }
                    else if (enteredChar != '\b') {
                        // Ajouter le caractère à la chaîne du nom d'utilisateur
                        username += enteredChar;
                    }

                }
                // Mettre à jour le texte affiché
                inputText.setString(username);
            }
            // Vérifier si la touche Entrée est pressée
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
                window.close();
        }

        window.clear();
        window.draw(instruction);
        window.draw(inputText);
        window.display();
    }
    m_pPlayers->ChangeName(username);
    SendJSON(-1);
}

void GameManager::InitGameEvent() {
    EventManager::Get()->AddComponent(sf::Event::EventType::KeyPressed, sf::Keyboard::Key::Escape, &EventCloseWindow);
    EventManager::Get()->AddComponent(sf::Event::EventType::MouseButtonPressed, sf::Mouse::Left, &EventPlaceSign);
    IsEventInit = true;
}

void GameManager::GameLoop()
{
        //EVENT
        EventManager::Get()->Update(&oWindow);

        vLocalPosition = sf::Mouse::getPosition(oWindow);

        //DRAW
        oWindow.clear();

        m_rBackground->Draw(&oWindow);
        for (Case* cCase : m_cCasesList)
        {
            cCase->Draw(&oWindow);
        }

        for (GameObject* cCase : m_gCasesBack)
        {
            if (cCase != nullptr) {
                cCase->Draw(&oWindow);
            }

        }

        

        oWindow.display();
}