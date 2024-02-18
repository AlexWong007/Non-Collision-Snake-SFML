#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

void DrawBoard(sf::RenderWindow&);
void SnakeMovement(sf::RectangleShape&, sf::RenderWindow&, sf::Keyboard::Key&);
void SnakeTail(sf::RectangleShape&, std::vector<sf::RectangleShape>&, int&, sf::RenderWindow&, std::vector<sf::RectangleShape>&);
void GameOverScene(sf::RenderWindow&);
void CheckBorderCollision(sf::RectangleShape&, bool&);
void CheckSnakeCollision(sf::RectangleShape&, std::vector<sf::RectangleShape>&, bool&, std::vector<sf::RectangleShape>&);
void SpawnApple(sf::RectangleShape&, sf::RenderWindow&);
void CheckApple(bool&, sf::RectangleShape&, sf::RenderWindow&, sf::RectangleShape&, int&);
void AppleEat(bool&, sf::RectangleShape&, sf::RectangleShape&, int&);
void UpdatePoints(sf::Text&, int&);

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(900, 900), "Snake", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::RectangleShape Snake(sf::Vector2f(25, 25));
    Snake.setFillColor(sf::Color::Red);
    Snake.setOutlineColor(sf::Color::White);
    Snake.setOutlineThickness(5);
    Snake.setPosition(450, 450);
    sf::Keyboard::Key PreviousKey = sf::Keyboard::D;
    std::vector<sf::RectangleShape> TailVector;
    int TailLength = 0;
    //int TailLength = 10500; max length
    bool GameOver = false;
    std::vector<sf::RectangleShape> CollisionVector;
    sf::RectangleShape Apple(sf::Vector2f(20, 20));
    Apple.setFillColor(sf::Color::Magenta);
    Apple.setOutlineColor(sf::Color::Black);
    Apple.setOutlineThickness(2); 
    bool AppleSpawned = false;
    srand(time(0));
    sf::Font font;
    sf::Text Points;

    if (!font.loadFromFile("TRON.ttf"))
    {

    }

    Points.setFont(font);
    Points.setFillColor(sf::Color::Magenta);
    Points.setPosition(sf::Vector2f(25, 25));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        if (!GameOver)
        {
            DrawBoard(window);
            SnakeMovement(Snake, window, PreviousKey);
            SnakeTail(Snake, TailVector, TailLength, window, CollisionVector);
            CheckBorderCollision(Snake, GameOver);
            //CheckSnakeCollision(Snake, TailVector, GameOver, CollisionVector); 
            CheckApple(AppleSpawned, Apple, window, Snake, TailLength);
            UpdatePoints(Points, TailLength);
            window.draw(Points);
            window.draw(Apple);
        }
        else
        {
            GameOverScene(window);
        }

        window.display();
    }

    return 0;
}

void DrawBoard(sf::RenderWindow& window)
{
    bool GridColour = false;
    sf::RectangleShape GreenGrid(sf::Vector2f(50, 50));
    GreenGrid.setFillColor(sf::Color(50, 205, 50));

    sf::RectangleShape YellowGrid(sf::Vector2f(50, 50));
    YellowGrid.setFillColor(sf::Color(206, 250, 5));

    for (int y = 0; y < 900; y += 50)
    {
        for (int x = 0; x < 900; x += 50)
        {
            if (GridColour)
            {
                GreenGrid.setPosition(sf::Vector2f(x, y));
                window.draw(GreenGrid);
                GridColour = false;
            }
            else if (!GridColour)
            {
                YellowGrid.setPosition(sf::Vector2f(x, y));
                window.draw(YellowGrid);
                GridColour = true;
            }
        }
        GridColour = !GridColour;
    }
}

void SnakeMovement(sf::RectangleShape& Snake, sf::RenderWindow& window, sf::Keyboard::Key& PreviousKey)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            Snake.move(sf::Vector2f(0, -6));
            PreviousKey = sf::Keyboard::W;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            Snake.move(sf::Vector2f(-6, 0));
            PreviousKey = sf::Keyboard::A;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            Snake.move(sf::Vector2f(0, 6));
            PreviousKey = sf::Keyboard::S;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            Snake.move(sf::Vector2f(6, 0));
            PreviousKey = sf::Keyboard::D;
        }
    }
    else
    {
        if (PreviousKey == sf::Keyboard::W)
        {
            Snake.move(sf::Vector2f(0, -6));
        }
        else if (PreviousKey == sf::Keyboard::A)
        {
            Snake.move(sf::Vector2f(-6, 0));
        }
        else if (PreviousKey == sf::Keyboard::S)
        {
            Snake.move(sf::Vector2f(0, 6));
        }
        else if (PreviousKey == sf::Keyboard::D)
        {
            Snake.move(sf::Vector2f(6, 0));
        }
    }
    
    window.draw(Snake);
}

void SnakeTail(sf::RectangleShape& Snake, std::vector<sf::RectangleShape>& TailVector, int& TailLength, sf::RenderWindow& window, std::vector<sf::RectangleShape>& CollisionVector)
{
    int counter = 0;
    bool check = false;
    sf::RectangleShape Tail(sf::Vector2f(25, 25));
    Tail.setFillColor(sf::Color::Red);

    sf::Vector2f SnakePosition = Snake.getPosition();
    Tail.setPosition(SnakePosition);
    
    TailVector.push_back(Tail);

    // for (int i = 1; i < TailLength; i++)
    // {
    //     window.draw(TailVector[TailVector.size() - 1]);
    // }

    for (sf::RectangleShape Tail : TailVector)
    {
        if (counter == (TailVector.size() - TailLength))
        {
            window.draw(Tail);
            check = true;
        }

        if (check)
        {
            window.draw(Tail);
        }
        counter++;
    }
    check = false;

    CollisionVector.clear();

    if (TailLength > 30)
    {
        CollisionVector = TailVector;

        for (int i = 0; i < 30; i++)
        {
            CollisionVector.pop_back();
        }
    }
}

void GameOverScene(sf::RenderWindow& window)
{
    sf::Font font;
    sf::Text text;

    if (!font.loadFromFile("TRON.ttf"))
    {
        return;
    }

    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(2);
    text.setPosition(sf::Vector2f(200, 400));
    text.setString("GAME OVER");
    text.setCharacterSize(50);

    window.draw(text);
}

void CheckBorderCollision(sf::RectangleShape& Snake, bool& GameOver)
{
    sf::Vector2f SnakePosition = Snake.getPosition();

    if (SnakePosition.x <= 0 || SnakePosition.y <= 0 || SnakePosition.x >= 900 || SnakePosition.y >= 900)
    {
        GameOver = true;
    }
}

void CheckSnakeCollision(sf::RectangleShape& Snake, std::vector<sf::RectangleShape>& TailVector, bool& GameOver, std::vector<sf::RectangleShape>& CollisionVector)
{
    sf::Vector2f SnakePosition = Snake.getPosition();

    // std::vector<sf::RectangleShape> newCollisionVector;

    // CollisionVector.clear();

    // for (int i = TailVector.size() - TailLength; i < TailVector.size(); ++i)
    // {
    //     CollisionVector.push_back(TailVector[i]);
    // }

    // for (sf::RectangleShape& TailSegment : CollisionVector)
    // {
    //     if (TailSegment.getPosition() == SnakePosition)
    //     {
    //         continue;
    //     }
    //     newCollisionVector.push_back(TailSegment);
    // }

    // CollisionVector.clear();
    // CollisionVector = newCollisionVector;

    // for (sf::RectangleShape TailSegment : CollisionVector)
    // {
    //     if (TailSegment.getPosition() == SnakePosition)
    //     {
    //         GameOver = true;
    //     }
    // }

    // if (CollisionVector.size() > 30)
    // {
    //     for (sf::RectangleShape TailSegment : CollisionVector)
    //     {
    //         if (TailSegment.getGlobalBounds().intersects(Snake.getLocalBounds()) && (TailSegment.getGlobalBounds().getPosition() != SnakePosition));
    //         {
    //             GameOver = true;
    //         }
    //     }
    // }

}

void SpawnApple(sf::RectangleShape& Apple, sf::RenderWindow& window)
{
    int max = 875;

    int x = rand() % max;
    int y = rand() % max;

    Apple.setPosition(x, y);
}

void CheckApple(bool& AppleSpawned, sf::RectangleShape& Apple, sf::RenderWindow& window, sf::RectangleShape& Snake, int& TailLength)
{
    if (AppleSpawned)
    {
        AppleEat(AppleSpawned, Apple, Snake, TailLength);
        return;
    }
    else
    {
        SpawnApple(Apple, window);
        AppleSpawned = true;
    }
}

void AppleEat(bool& AppleSpawned , sf::RectangleShape& Apple, sf::RectangleShape& Snake, int& TailLength)
{
    if (Snake.getGlobalBounds().intersects(Apple.getGlobalBounds()))
    {
        AppleSpawned = false;
        TailLength += 10;
    }
    else
    {
        return;
    }
}

void UpdatePoints(sf::Text& Points, int& TailLength)
{
    Points.setString(std::to_string(TailLength));
}