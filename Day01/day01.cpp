#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Helper
{
private:
    static const int MAX_VALUE = 100000;

    int _leftListArray[MAX_VALUE] = {0};
    int _rightListArray[MAX_VALUE] = {0};

public:
    int _firstPuzzleAnswer = 0;
    int _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    std::vector<int> _leftList;
    std::vector<int> _rightList;

    void parseValues()
    {
        // parse values and put these into aux arrays
        for(int i=0; i<_fileInput.size(); ++i)
        {
            // get values
            std::string currentLine = _fileInput[i];
            auto pos = currentLine.find_first_of(' ');
            int value = std::stoi( currentLine.substr(0, pos) );
            _leftListArray[value]++;
            pos = currentLine.find_first_not_of(' ', pos);
            value = std::stoi( currentLine.substr(pos) );
            _rightListArray[value]++;
        }

        // go through the aux arrays and fill the ordered vectors
        for (int i = 0; i < MAX_VALUE; ++i)
        {
            for(int j=0; j<_leftListArray[i]; ++j)
            {
                _leftList.push_back(i);
            }
            for(int j=0; j<_rightListArray[i]; ++j)
            {
                _rightList.push_back(i);
            }
        }

//        std::cout << _leftList.size() << " _ " << _rightList.size() << std::endl;
//        for (int i = 0; i < _leftList.size(); ++i)
//        {
//            std::cout << _leftList[i] << " " << _rightList[i] << std::endl;
//        }
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        for (int i = 0; i < _leftList.size(); ++i)
        {
            this->_firstPuzzleAnswer += abs(_rightList[i] - _leftList[i]);
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        for (int i = 0; i < _leftList.size(); ++i)
        {
            int value = _leftList[ i ];
            this->_secondPuzzleAnswer += (value * _leftListArray[value] * _rightListArray[value]);
        }
    }

   void calculateAnswers(std::string inputFileName)
   {
        this->readFileInput(inputFileName);
        this->parseValues();
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
    std::cout << "Advent of Code 2024 - Day 01" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}