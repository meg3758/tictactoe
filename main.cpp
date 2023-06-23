#include <iostream>
#include <SFML/Graphics.hpp>
#define window_size 450
#define grid_size 150
#define char_size 100

using namespace sf;
Text circle(int row,int column,Font& font){
    Text text;
    text.setFont(font);
    text.setString("O");
    text.setFillColor(Color::Yellow);
    text.setCharacterSize(char_size);
    text.setPosition(Vector2f(grid_size*row+(grid_size-char_size)/2,grid_size*column+(grid_size-char_size)/2));
    return text;
}
Text ex(int row,int column,Font& font){
    Text text;
    text.setFont(font);
    text.setString("X");
    text.setFillColor(Color::Blue);
    text.setCharacterSize(char_size);
    text.setPosition(Vector2f(grid_size*row+(grid_size-char_size)/2,grid_size*column+(grid_size-char_size)/2));
    return text;
}
unsigned int mousePos(Event event){
    unsigned int value=0;
    if(event.type==Event::MouseButtonPressed){
        if(event.mouseButton.button==Mouse::Left){
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;
            if(x<grid_size){
                value+=10;
            }
            else if(x<grid_size*2){
                value+=20;
            }
            else{
                value+=30;
            }
            if(y<grid_size){
                value+=1;
            }
            else if(y<grid_size*2){
                value+=2;
            }
            else{
                value+=3;
            }
        }
    }
    //std::cout<<value<<std::endl;
    return value;
}
bool checkWinCondition(const std::vector<Text>& positions)
{
    static const std::vector<std::vector<unsigned int>> winConditions{
            {11, 12, 13}, {21, 22, 23}, {31, 32, 33},  // horizontal
            {11, 21, 31}, {12, 22, 32}, {13, 23, 33},  // vertical
            {11, 22, 33}, {13, 22, 31}                 // diagonal
    };

    for (const auto& condition : winConditions)
    {
        bool win = true;
        for (const auto& pos : condition)
        {
            bool found = false;
            for (const auto& text : positions)
            {
                Vector2f position = text.getPosition();
                unsigned int row = static_cast<unsigned int>((position.x - 10) / grid_size) + 1;
                unsigned int column = static_cast<unsigned int>((position.y - 10) / grid_size) + 1;
                unsigned int positionValue = row * 10 + column;

                if (positionValue == pos)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                win = false;
                break;
            }
        }

        if (win)
        {
            return true;
        }
    }

    return false;
}


int main() {
    srand(time(NULL));
    RenderWindow appWindow(sf::VideoMode(window_size,window_size),"Tic Tac Toe");
    //grid
    RectangleShape grid(Vector2f(grid_size,grid_size));
    grid.setFillColor(Color::Transparent);
    grid.setOutlineThickness(1.0f);
    grid.setOutlineColor(Color::White);

    Font font;
    if(!font.loadFromFile("../others/font.ttf")){
        std::cout << "failed to load font" << std::endl;
    }
    unsigned int turns = 0;
    std::vector<Text> circles;
    std::vector<Text> exes;
    std::vector<unsigned int> covered_positions;
    int random = (rand()%3+1)*10+(rand()%3+1);
    bool gameWon = false;
    bool gameLost = false;
    bool gameOver = false;
    Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(40);
    resultText.setPosition(window_size / 2 - 100, window_size / 2 - 20);

    Event appEvent;
    while(appWindow.isOpen()&& !gameWon && !gameLost){
        while(appWindow.pollEvent(appEvent)){
            if(appEvent.type == Event::Closed){
                appWindow.close();
            }
        }
        if(covered_positions.size()!=9){
            if(std::find(covered_positions.begin(),covered_positions.end(), mousePos(appEvent))==covered_positions.end()){
                while(std::find(covered_positions.begin(),covered_positions.end(), random)!=covered_positions.end()){
                    random = (rand()%3+1)*10+(rand()%3+1);
                }
                if(appEvent.type==Event::MouseButtonPressed && appEvent.mouseButton.button == Mouse::Left){
                    if(turns%2==0){
                        circles.push_back(circle(mousePos(appEvent)/10-1, mousePos(appEvent)%10-1,font));
                        turns++;
                        covered_positions.push_back(mousePos(appEvent));
                    }
                }
                else{
                    if(turns%2==1){
                        exes.push_back(ex(random/10-1, random%10-1,font));
                        turns++;
                        covered_positions.push_back(random);
                    }
                }
//                if (checkWinCondition(exes))
//                {
//                    gameLost = true;
//                    break;
//                }
//                else if (checkWinCondition(circles))
//                {
//                    gameWon = true;
//                    break;
//                }
//
//                if (covered_positions.size() == 9)
//                {
//                    break;
//                }
                if (circles.size() >= 3 || exes.size() >= 3)
                {
                    if (checkWinCondition(circles))
                    {
                        resultText.setString("Congatulations!!");
                        gameOver = true;
                    }
                    else if (checkWinCondition(exes))
                    {
                        resultText.setString("You lost!");
                        gameOver = true;
                    }
                    else if (covered_positions.size() == 9)
                    {
                        resultText.setString("Remis!");
                        gameOver = true;
                    }
                }

                if (gameOver)
                {
                    appWindow.clear(Color::Black);
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            grid.setPosition(Vector2f(grid_size * i, grid_size * j));
                            appWindow.draw(grid);
                        }
                    }
                    for (unsigned int i = 0; i < circles.size(); i++)
                    {
                        appWindow.draw(circles.at(i));
                    }
                    for (unsigned int i = 0; i < exes.size(); i++)
                    {
                        appWindow.draw(exes.at(i));
                    }
                    appWindow.draw(resultText);
                    appWindow.display();

                    sleep(seconds(3));  // Oczekiwanie 3 sekundy przed zamknięciem okna
                    appWindow.close();
                }


            }
        }
        else{
            sleep(seconds(1));
            circles.clear();
            exes.clear();
            covered_positions.clear();
        }

        appWindow.clear(Color::Black);
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                grid.setPosition(Vector2f(grid_size*i,grid_size*j));
                appWindow.draw(grid);
            }
        }
        for(unsigned int i=0;i<circles.size();i++){
            appWindow.draw(circles.at(i));
        }
        for(unsigned int i=0;i<exes.size();i++){
            appWindow.draw(exes.at(i));
        }
        appWindow.display();
    }
    return 0;
}
