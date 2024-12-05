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

    std::pair<int, int> parseMulValue(std::string str, int &beginPos, int &endPos)
    {
        std::pair<int, int> mulValue(-1, -1);

        // find "mul("
        bool found = false;
        do
        {
            beginPos = str.find("mul(", beginPos);
            if( beginPos > -1 )
            {
                // discard "mul(" from search
                beginPos += 4;
                // find ','
                endPos = str.find_first_of(',', beginPos);
                if( endPos > -1 )
                {
                    // validate is number
                    std::string value = str.substr(beginPos, endPos-beginPos);
                    if( isNumber(value) )
                    {
                        mulValue.first = std::stoi( value );
                        beginPos = endPos + 1;
                        // find ')'
                        endPos = str.find_first_of(')', beginPos);
                        if( endPos > -1 )
                        {
                            // validate is number
                            value = str.substr(beginPos, endPos-beginPos);
                            if( isNumber(value) )
                            {
                                // mul value is correct, add to list and continue searching from the end of this mul(x,y)
                                mulValue.second = std::stoi( value );
                                beginPos = endPos + 1;
                                found = true;
                            }
                        }
                    }
                }
            }
        }
        while(beginPos > -1 && endPos > -1 && found == false);

        return mulValue;
    }

    void parseMulValues()
    {
        _mulValues.clear();
        // parse values and put these into aux vector
        for(int i=0; i<_fileInput.size(); ++i)
        {
            std::string currentLine = _fileInput[i];
            auto beginPos = 0;
            auto endPos = 0;
            // get mul values
            do
            {
                std::pair mulValue = parseMulValue(currentLine, beginPos, endPos);
                if( mulValue.first != -1 )
                {
                    _mulValues.push_back( mulValue );
                }
            }
            while(beginPos > -1 && endPos > -1);
        }
    }

    void parseMulValuesWithConditionals()
    {
        _mulValues.clear();
        // parse values and put these into aux vector
        bool enabled = true;
        for(int i=0; i<_fileInput.size(); ++i)
        {
            std::string currentLine = _fileInput[i];
            auto beginPos = 0;
            auto endPos = 0;
            auto nextDont = 0;

            // if the last instruction on previous line was to enable we search for next "don't()"            
            if( enabled == true )
            {
                nextDont = currentLine.find("don't()");
            }
            // but if the last instruction on previous line was to disable we search for "do()" before searching for "don't"
            else
            {
                // find next "do()" and "don't()"
                beginPos = currentLine.find("do()");
                if( beginPos > -1 )
                {
                    enabled = true;
                    // discard "do()" from search
                    beginPos += 4;
                    nextDont = currentLine.find("don't()", beginPos);
                    if( nextDont < 0 )
                    {
                        nextDont = currentLine.size();
                    }
                }
            }

            // get mul values
            do
            {
                std::pair mulValue = parseMulValue(currentLine, beginPos, endPos);
                if( mulValue.first != -1 )
                {
                    if( endPos <= nextDont )
                    {
                        // enabled mul value, add to vector
                        _mulValues.push_back( mulValue );
                    }
                    else
                    {
                        enabled = false;
                        // disabled mul value, find next "do()" and "don't()"
                        beginPos = currentLine.find("do()", nextDont);
                        if( beginPos > -1 )
                        {
                            enabled = true;
                            // discard "do()" from search
                            beginPos += 4;
                            nextDont = currentLine.find("don't()", beginPos);
                            if( nextDont < 0 )
                            {
                                nextDont = currentLine.size();
                            }
                        }
                    }
                }
            }
            while(beginPos > -1 && endPos > -1);
        }
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        this->parseMulValues();

        for(int i=0; i<_mulValues.size(); ++i)
        {
            this->_firstPuzzleAnswer += (_mulValues[i].first * _mulValues[i].second);
//            std::cout << "mul(" << _mulValues[i].first << "," << _mulValues[i].second << ")" << std::endl;
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        this->parseMulValuesWithConditionals();

        for(int i=0; i<_mulValues.size(); ++i)
        {
            this->_secondPuzzleAnswer += (_mulValues[i].first * _mulValues[i].second);
//            std::cout << "mul(" << _mulValues[i].first << "," << _mulValues[i].second << ")" << std::endl;
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