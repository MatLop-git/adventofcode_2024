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
    std::vector< std::vector<int> > _pagesUpdatesOk;
    std::vector< std::vector<int> > _pagesUpdatesToFix;

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
                _orderingRules[ leftPage ].page = leftPage;
                _orderingRules[ leftPage ].pagesAfter.push_back( rightPage );
                _orderingRules[ rightPage ].page = rightPage;
                _orderingRules[ rightPage ].pagesBefore.push_back( leftPage );
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

    void validatePagesUpdates()
    {
        for(int i=0; i<_pagesUpdates.size(); ++i)
        {
            // check correct update
            bool isCorrect = true;
            std::vector<int> pagesUpdate = _pagesUpdates[i];
            int pagesUpdateSize = pagesUpdate.size();
            int j=0;
            while( isCorrect && (j<pagesUpdateSize) )
            {
                int page = pagesUpdate[j];
                if( _orderingRules.find(page) != _orderingRules.end() )
                {
                    OrderingRule rule = _orderingRules[page];
                    // check previous pages
                    int k=j-1;
                    while( isCorrect && (k>=0) )
                    {
                        isCorrect = (std::find(rule.pagesAfter.begin(), rule.pagesAfter.end(), pagesUpdate[k]) == rule.pagesAfter.end());
                        k--;
                    }
                    // check next pages
                    k=j+1;
                    while( isCorrect && (k<pagesUpdateSize) )
                    {
                        isCorrect = (std::find(rule.pagesBefore.begin(), rule.pagesBefore.end(), pagesUpdate[k]) == rule.pagesBefore.end());
                        k++;
                    }
                }
                j++;
            }
            if( isCorrect )
            {
                _pagesUpdatesOk.push_back( pagesUpdate );
            }
            else
            {
                _pagesUpdatesToFix.push_back( pagesUpdate );
            }
        }
    }

    void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // sum the middle page of correct pages updates
        std::vector<int> pagesUpdate;
        for(int i=0; i<_pagesUpdatesOk.size(); ++i)
        {
            pagesUpdate = _pagesUpdatesOk[i];
            int middleValue = (pagesUpdate.size()/2);
            middleValue = pagesUpdate[middleValue];
            this->_firstPuzzleAnswer += middleValue;
//            std::cout << (i+1) << " - " << middleValue << std::endl;
        }
    }

    void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // fix incorrect pages updates
        for(int i=0; i<_pagesUpdatesToFix.size(); ++i)
        {
            // fix pages update
            bool isCorrect = false;
            std::vector<int> pagesUpdate = _pagesUpdatesToFix[i];
            int pagesUpdateSize = pagesUpdate.size();
            int j=0;
            while( !isCorrect || (j<pagesUpdateSize) )
            {
                isCorrect = true;
                pagesUpdate = _pagesUpdatesToFix[i];
                int page = pagesUpdate[j];
                if( _orderingRules.find(page) != _orderingRules.end() )
                {
                    OrderingRule rule = _orderingRules[page];
                    // check previous pages
                    int k=j-1;
                    while( isCorrect && (k>=0) )
                    {
                        isCorrect = (std::find(rule.pagesAfter.begin(), rule.pagesAfter.end(), pagesUpdate[k]) == rule.pagesAfter.end());
                        if( !isCorrect )
                        {
                            _pagesUpdatesToFix[i].erase(_pagesUpdatesToFix[i].begin()+j);
                            _pagesUpdatesToFix[i].insert(_pagesUpdatesToFix[i].begin()+k, page);
                            j = -1;
                        }
                        k--;
                    }
// no need to check for next pages as we are checking from the begining again
//                    // check next pages
//                    k=j+1;
//                    while( isCorrect && (k<pagesUpdateSize) )
//                    {
//                        isCorrect = (std::find(rule.pagesBefore.begin(), rule.pagesBefore.end(), pagesUpdate[k]) == rule.pagesBefore.end());
//                        if( !isCorrect )
//                        {
//                            _pagesUpdatesToFix[i].erase(_pagesUpdatesToFix[i].begin()+j);
//                            _pagesUpdatesToFix[i].insert(_pagesUpdatesToFix[i].begin()+k, page);
//                            j = -1;
//                        }
//                        k++;
//                    }
                }
                j++;
            }

            // add the middle page of fixed pages update to the result
            int middleValue = (pagesUpdate.size()/2);
            middleValue = pagesUpdate[middleValue];
            this->_secondPuzzleAnswer += middleValue;
//            std::cout << (i+1) << " - " << middleValue << std::endl;
        }
    }

   void calculateAnswers(std::string inputFileName)
   {
        this->readFileInput(inputFileName);
        this->parseValues();
        this->validatePagesUpdates();
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