#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

class Helper
{
public:
    int _firstPuzzleAnswer = 0;
    int _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    std::vector<std::pair<int, int>> _mulValues;

    bool isNumber(const std::string& s)
    {
        static const std::regex number_regex(
            R"(^[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?$)"
        );
        return std::regex_match(s, number_regex);
    }

    std::pair<int, int> parseMulValue(std::string str, int beginPos, int endPos)
    {
        std::pair<int, int> mulValue;


        return mulValue;
    }

    void parseMulValues()
    {
        // parse values and put these into aux arrays
        std::string currentLine;
        for(int i=0; i<_fileInput.size(); ++i)
        {
            // get mul values
            std::string currentLine = _fileInput[i];
            auto beginPos = 0;
            auto endPos = 0;
            int value = 0;
            do
            {
                // find "mul("
                beginPos = currentLine.find("mul(", beginPos);
                if( beginPos > -1 )
                {
                    std::pair<int, int> mulValue;
                    // discard "mul(" from search
                    beginPos += 4;
                    // find ','
                    endPos = currentLine.find_first_of(',', beginPos);
                    if( endPos > -1 )
                    {
                        // validate is number
                        std::string value = currentLine.substr(beginPos, endPos-beginPos);
                        if( isNumber(value) )
                        {
                            mulValue.first = std::stoi( value );
                            beginPos = endPos + 1;
                            // find ')'
                            endPos = currentLine.find_first_of(')', beginPos);
                            if( endPos > -1 )
                            {
                                // validate is number
                                value = currentLine.substr(beginPos, endPos-beginPos);
                                if( isNumber(value) )
                                {
                                    // mul value is correct, add to list and continue searching from the end of this mul(x,y)
                                    mulValue.second = std::stoi( value );
                                    _mulValues.push_back( mulValue );
                                    beginPos = endPos + 1;
                                }
                            }
                        }
                    }
                }
            }
            while(beginPos > -1 && endPos > -1);
        }
    }

    void parseMulValuesWithConditionals()
    {
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        this->parseMulValues();

        for(int i=0; i<_mulValues.size(); ++i)
        {
            this->_firstPuzzleAnswer += (_mulValues[i].first * _mulValues[i].second);
            std::cout << "mul(" << _mulValues[i].first << "," << _mulValues[i].second << ")" << std::endl;
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        this->parseMulValuesWithConditionals();

        for(int i=0; i<_mulValues.size(); ++i)
        {
            this->_secondPuzzleAnswer += (_mulValues[i].first * _mulValues[i].second);
            std::cout << "mul(" << _mulValues[i].first << "," << _mulValues[i].second << ")" << std::endl;
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
    std::cout << "Advent of Code 2024 - Day 03" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}