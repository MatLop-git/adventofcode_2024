#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Helper
{
public:
    int _firstPuzzleAnswer = 0;
    int _secondPuzzleAnswer = 0;
    std::vector<std::string> _fileInput;

    struct OrderingRule
    {
        int page;
        std::vector<int> pagesBefore;
        std::vector<int> pagesAfter;
    };

    std::map<int, OrderingRule> _orderingRules;
    std::vector< std::vector<int> > _pagesUpdates;

    void parseValues()
    {
        bool parsingRules = true;
        for(int i=0; i<_fileInput.size(); ++i)
        {
            std::string currentLine = _fileInput[i];
            if( currentLine.size() < 3 )
            {
                parsingRules = false;
            }
            // parse rules
            else if( parsingRules )
            {
                int divPos = currentLine.find_first_of('|');
                int leftPage = std::stoi( currentLine.substr(0, divPos) );
                int rightPage = std::stoi( currentLine.substr(divPos+1) );
                _orderingRules[ leftPage ].pagesAfter.push_back( rightPage );
                _orderingRules[ rightPage ].pagesAfter.push_back( rightPage );
            }
            // parse pages updates
            else
            {
                auto beginPos = 0;
                auto endPos = 0;
                std::vector<int> pagesUpdate;
                int page = 0;
                do
                {
                    endPos = currentLine.find_first_of(',', beginPos);
                    page = std::stoi( currentLine.substr(beginPos, endPos) );
                    pagesUpdate.push_back( page );
                    beginPos = endPos + 1;
                }
                while(endPos > -1);
                _pagesUpdates.push_back( pagesUpdate );
            }
        }

//        for (int i = 0; i < _pagesUpdates.size(); ++i)
//        {
//            std::cout << i << ": ";
//            for (int j = 0; j < _pagesUpdates[i].size(); ++j)
//            {
//                std::cout << _pagesUpdates[i][j] << " - ";
//            }
//            std::cout << std::endl;
//        }
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        parseValues();

        for(int i=0; i<_pagesUpdates.size(); ++i)
        {
            // check correct update
            bool isCorrect = true;
            std::vector<int> pageUpdate = _pagesUpdates[i];
            int pageUpdateSize = pageUpdate.size();
            int j=0;
            while( isCorrect && (j<pageUpdateSize) )
            {
                int page = _pagesUpdates[i][j];
                if( _orderingRules.find(page) != _orderingRules.end() )
                {
                    OrderingRule rule = _orderingRules[page];
                    // check previous pages
                    int k=j-1;
                    while( isCorrect && (k>=0) )
                    {
                        isCorrect = (std::find(rule.pagesAfter.begin(), rule.pagesAfter.end(), pageUpdate[k]) == rule.pagesAfter.end());
                        k--;
                    }
                    // check next pages
                    k=j+1;
                    while( isCorrect && (k<pageUpdateSize) )
                    {
                        isCorrect = (std::find(rule.pagesBefore.begin(), rule.pagesBefore.end(), pageUpdate[k]) == rule.pagesBefore.end());
                        k++;
                    }
                }
                j++;
            }
            if( isCorrect )
            {
                int middleValue = (pageUpdateSize/2);
                middleValue = pageUpdate[middleValue];
                this->_firstPuzzleAnswer += middleValue;
//                std::cout << (i+1) << " - " << middleValue << std::endl;
            }
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

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
    std::cout << "Advent of Code 2024 - Day 05" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}