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

    std::vector<std::vector<int>> _reports;

    void parseValues()
    {
        // parse values and put these into aux arrays
        for(int i=0; i<_fileInput.size(); ++i)
        {
            // get levels
            std::vector<int> report;
            std::string currentLine = _fileInput[i];
            auto beginPos = 0;
            auto endPos = 0;
            int level = 0;
            do
            {
                endPos = currentLine.find_first_of(' ', beginPos);
                level = std::stoi( currentLine.substr(beginPos, endPos) );
                report.push_back( level );
                beginPos = endPos + 1;
            }
            while(endPos > -1);
            _reports.push_back( report );
        }
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        for(int i=0; i<_reports.size(); ++i)
        {
            std::vector<int> report = _reports[i];
            // calculate report direction
            bool direction = (report[0] < report[1]);
            // verify levels safety
            bool safe = true;
            int j=1;
            do
            {
                int diff = report[j]-report[j-1];
                if( direction )
                {
                    safe = (1 <= diff) && (diff <= 3);
                }
                else
                {
                    safe = (1 <= -diff) && (-diff <= 3);
                }
                j++;
            }
            while(safe && j<report.size());

            if( safe )
            {
                this->_firstPuzzleAnswer++;
            }
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        for(int i=0; i<_reports.size(); ++i)
        {
            std::vector<int> report = _reports[i];
            // calculate report direction
            int directionAux = 0;
            for(int j=1; j<report.size(); ++j)
            {
                directionAux += (report[j] > report[j-1] ? 1 : -1);
            }
            bool direction = directionAux > 0;
            // verify levels safety
            bool safe = true;
            bool removedSingleBadLevel = false;
            bool handleSingleBadLevel = false;
            int j=1;
            do
            {
                int diff = (handleSingleBadLevel == false ? report[j]-report[j-1] : report[j]-report[j-2]);
                if( direction )
                {
                    safe = (1 <= diff) && (diff <= 3);
                }
                else
                {
                    safe = (1 <= -diff) && (-diff <= 3);
                }

                // handle remove first level
                if( safe == false && handleSingleBadLevel == true && j == 2 )
                {
                    int diff = report[j]-report[j-1];
                    if( direction )
                    {
                        safe = (1 <= diff) && (diff <= 3);
                    }
                    else
                    {
                        safe = (1 <= -diff) && (-diff <= 3);
                    }
                }
                handleSingleBadLevel = false;

                // verify if single bad level should be removed
                if( safe == false && removedSingleBadLevel == false )
                {
                    safe = true;
                    handleSingleBadLevel = true;
                    removedSingleBadLevel = true;
                }

                j++;
            }
            while(safe && j<report.size());

            if( safe )
            {
                std::cout << i+1 << ": safe" << std::endl;
                this->_secondPuzzleAnswer++;
            }
            else
            {
                std::cout << i+1 << ": bad" << std::endl;
            }
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
    std::cout << "Advent of Code 2024 - Day 02" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}