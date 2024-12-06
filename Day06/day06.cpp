#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Helper
{
public:
    int _firstPuzzleAnswer = 0;
    int _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    enum Direction { Up = 0, Right = 1, Down = 2, Left = 3 };
    std::map<int, std::set<int> > _guardArea;
    std::pair<int, int> _guardInitialPosition;

    void calculateGuardPositions()
    {
        Direction currentDirection = Up;
        // find guard starting position
        int x = -1;
        int y = -1;
        do
        {
            y++;
            std::string currentLine = _fileInput[y];
            x = currentLine.find_first_of('^');
        }
        while( x < 0 );

        _guardInitialPosition = std::pair<int, int>(x, y);
//        std::cout << "starting pos: " << x << "," << y << std::endl;

        // find guard positions
        _guardArea[y].insert(x);
        bool exited = false;
        do
        {
            switch (currentDirection)
            {
                case Up:
                    if( y < 1 )
                    {
                        exited = true;
                    }
                    else if( _fileInput[y-1][x] == '#' )
                    {
                        currentDirection = Right;
                    }
                    else
                    {
                        y--;
                        _guardArea[y].insert(x);
                    }
                    break;
                case Right:
                    if( x >= _fileInput[y].size()-1 )
                    {
                        exited = true;
                    }
                    else if( _fileInput[y][x+1] == '#' )
                    {
                        currentDirection = Down;
                    }
                    else
                    {
                        x++;
                        _guardArea[y].insert(x);
                    }
                    break;
                case Down:
                    if( y >= _fileInput.size()-1 )
                    {
                        exited = true;
                    }
                    else if( _fileInput[y+1][x] == '#' )
                    {
                        currentDirection = Left;
                    }
                    else
                    {
                        y++;
                        _guardArea[y].insert(x);
                    }
                    break;
                case Left:
                    if( x < 1 )
                    {
                        exited = true;
                    }
                    else if( _fileInput[y][x-1] == '#' )
                    {
                        currentDirection = Up;
                    }
                    else
                    {
                        x--;
                        _guardArea[y].insert(x);
                    }
                    break;
                default:
                    exited = true;
                    break;
            }
        }
        while( exited == false );
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        for (auto const& [y, xs] : _guardArea)
        {
            this->_firstPuzzleAnswer += xs.size();
//            std::cout << y << ':' << xs.size() << std::endl;
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // put an obstacle on each point of the guard positions and check loop
        for (auto const& [guardAreaY, guardAreaXs] : _guardArea)
        {
            for( auto const& guardAreaX : guardAreaXs )
            {
                // skip initial position
                int x = _guardInitialPosition.first;
                int y = _guardInitialPosition.second;
                if( guardAreaX != x || guardAreaY != y )
                {
                    _fileInput[guardAreaY][guardAreaX] = '#';

                    Direction currentDirection = Up;
                    std::map< int, std::map<int, int> > currentGuardArea;
                    bool exited = false;
                    do
                    {
                        switch (currentDirection)
                        {
                            case Up:
                                if( y < 1 )
                                {
                                    exited = true;
                                }
                                else if( _fileInput[y-1][x] == '#' )
                                {
                                    currentDirection = Right;
                                }
                                else
                                {
                                    y--;
                                    currentGuardArea[y][x]++;
                                    if( currentGuardArea[y][x] > 4 )
                                    {
                                        this->_secondPuzzleAnswer++;
                                        exited = true;
                                    }
                                }
                                break;
                            case Right:
                                if( x >= _fileInput[y].size()-1 )
                                {
                                    exited = true;
                                }
                                else if( _fileInput[y][x+1] == '#' )
                                {
                                    currentDirection = Down;
                                }
                                else
                                {
                                    x++;
                                    currentGuardArea[y][x]++;
                                    if( currentGuardArea[y][x] > 4 )
                                    {
                                        this->_secondPuzzleAnswer++;
                                        exited = true;
                                    }
                                }
                                break;
                            case Down:
                                if( y >= _fileInput.size()-1 )
                                {
                                    exited = true;
                                }
                                else if( _fileInput[y+1][x] == '#' )
                                {
                                    currentDirection = Left;
                                }
                                else
                                {
                                    y++;
                                    currentGuardArea[y][x]++;
                                    if( currentGuardArea[y][x] > 4 )
                                    {
                                        this->_secondPuzzleAnswer++;
                                        exited = true;
                                    }
                                }
                                break;
                            case Left:
                                if( x < 1 )
                                {
                                    exited = true;
                                }
                                else if( _fileInput[y][x-1] == '#' )
                                {
                                    currentDirection = Up;
                                }
                                else
                                {
                                    x--;
                                    currentGuardArea[y][x]++;
                                    if( currentGuardArea[y][x] > 4 )
                                    {
                                        this->_secondPuzzleAnswer++;
                                        exited = true;
                                    }
                                }
                                break;
                            default:
                                exited = true;
                                break;
                        }
                    }
                    while( exited == false );

                    _fileInput[guardAreaY][guardAreaX] = '.';
                }
            }
        }
    }

   void calculateAnswers(std::string inputFileName)
   {
        this->readFileInput(inputFileName);
        this->calculateGuardPositions();
        this->calculateFirstPuzzleAnswer();
        this->calculateSecondPuzzleAnswer();
   }

   void readFileInput(std::string inputFileName)
   {
        std::string currentLine;
        std::ifstream inputFile(inputFileName);
        if ( inputFile.is_open() )
        {
            while ( std::getline(inputFile, currentLine) )
            {
                _fileInput.push_back(currentLine);
            }
        }
   }

   int getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   int getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};


int main()
{
    std::cout << "Advent of Code 2024 - Day 06" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}