#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Helper
{
public:
    int _firstPuzzleAnswer = 0;
    int _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    enum Direction { Right = 0, Left = 1, Up = 2, Down = 3, RightUp = 4, RightDown = 5, LeftUp = 6, LeftDown = 7 };
    const std::string XMAS_WORD = "XMAS";

    bool isXMAS(int x, int y, Direction direction, int letterIndex)
    {
        char letter = XMAS_WORD[letterIndex];
        // determine the position to look for letterIndex
        int currentX = x;
        int currentY = y;
        switch (direction)
        {
            case Right:
            {
                currentX += letterIndex;
                break;
            }
            case Left:
            {
                currentX -= letterIndex;
                break;
            }
            case Up:
            {
                currentY -= letterIndex;
                break;
            }
            case Down:
            {
                currentY += letterIndex;
                break;
            }
            case RightUp:
            {
                currentX += letterIndex;
                currentY -= letterIndex;
                break;
            }
            case RightDown:
            {
                currentX += letterIndex;
                currentY += letterIndex;
                break;
            }
            case LeftUp:
            {
                currentX -= letterIndex;
                currentY -= letterIndex;
                break;
            }
            case LeftDown:
            {
                currentX -= letterIndex;
                currentY += letterIndex;
                break;
            }
            default:
            {
                return false;
            }
        }

        // check if the letter is correct
        if( _fileInput[currentY][currentX] == letter )
        {
            if( letterIndex == XMAS_WORD.size()-1 )
            {
                return true;
            }
            else
            {
                return isXMAS(x, y, direction, letterIndex+1);
            }
        }
        return false;
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // go through data searching for initial values 'X'
        int xmasWordSize = XMAS_WORD.size();
        int fileInputSize = _fileInput.size();
        for(int i=0; i<fileInputSize; ++i)
        {
            std::string currentLine = _fileInput[i];
            int currentLineSize = currentLine.size();
            for(int j=0; j<currentLineSize; ++j)
            {
                if( currentLine[j] == 'X')
                {
                    // search right
                    if( (j+xmasWordSize <= currentLineSize) && isXMAS(j, i, Direction::Right, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search left
                    if( (j-xmasWordSize >= -1) && isXMAS(j, i, Direction::Left, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search up
                    if( (i-xmasWordSize >= -1) && isXMAS(j, i, Direction::Up, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search down
                    if( (i+xmasWordSize <= fileInputSize) && isXMAS(j, i, Direction::Down, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search right up
                    if( (j+xmasWordSize <= currentLineSize) && (i-xmasWordSize >= -1) && isXMAS(j, i, Direction::RightUp, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search right down
                    if( (j+xmasWordSize <= currentLineSize) && (i+xmasWordSize <= fileInputSize) && isXMAS(j, i, Direction::RightDown, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search left up
                    if( (j-xmasWordSize >= -1) && (i-xmasWordSize >= -1) && isXMAS(j, i, Direction::LeftUp, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                    // search left down
                    if( (j-xmasWordSize >= -1) && (i+xmasWordSize <= fileInputSize) && isXMAS(j, i, Direction::LeftDown, 1) )
                    {
                        this->_firstPuzzleAnswer++;
                    }
                }
            }
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // go through data searching for 'MAS' in X
        int xmasWordSize = XMAS_WORD.size();
        int fileInputSize = _fileInput.size();
        for(int i=0; i<fileInputSize; ++i)
        {
            std::string currentLine = _fileInput[i];
            int currentLineSize = currentLine.size();
            for(int j=0; j<currentLineSize; ++j)
            {
                if( currentLine[j] == 'A')
                {
                    if( (j-1 > -1) && (j+1 < currentLineSize) && (i-1 > -1) && (i+1 < fileInputSize) 
                        && ((_fileInput[i-1][j-1] == 'M' && _fileInput[i+1][j+1] == 'S') || (_fileInput[i-1][j-1] == 'S' && _fileInput[i+1][j+1] == 'M')) 
                        && ((_fileInput[i-1][j+1] == 'M' && _fileInput[i+1][j-1] == 'S') || (_fileInput[i-1][j+1] == 'S' && _fileInput[i+1][j-1] == 'M')) )
                    {
                        this->_secondPuzzleAnswer++;
                    }
                }
            }
        }
    }

   void calculateAnswers(std::string inputFileName)
   {
        this->readFileInput(inputFileName);
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
    std::cout << "Advent of Code 2024 - Day 04" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}