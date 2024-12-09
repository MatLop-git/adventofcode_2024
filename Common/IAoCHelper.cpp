#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class IAoCHelper
{
public:
    unsigned long long _firstPuzzleAnswer = 0;
    unsigned long long _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    unsigned long long getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
    unsigned long long getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
    std::vector<std::string> getFileInput() { return _fileInput; }

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

    virtual void calculateAnswers(std::string inputFileName)
    {
        this->readFileInput(inputFileName);

        this->prepareInput();
        this->calculateFirstPuzzleAnswer();
        this->calculateSecondPuzzleAnswer();
    }

    virtual void prepareInput() { }
    virtual void calculateFirstPuzzleAnswer() = 0;
    virtual void calculateSecondPuzzleAnswer() = 0;
};
