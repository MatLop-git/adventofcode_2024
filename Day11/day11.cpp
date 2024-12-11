#include "IAoCHelper.cpp"

class Helper : public IAoCHelper
{
    const int TARGET_BLINKS = 25;

    std::vector<unsigned long long> _initialStones;

    void prepareInput()
    {
        // parse initial stones
        auto beginPos = 0;
        auto endPos = beginPos;
        do
        {
            endPos = _fileInput[0].find_first_of(' ', beginPos);
            _initialStones.push_back( std::stoull( _fileInput[0].substr(beginPos, endPos) ) );
            beginPos = endPos + 1;
        }
        while( endPos != std::string::npos);
    }

    void blink(std::vector<unsigned long long> &stones)
    {
        for(int i=stones.size()-1; i>=0; --i)
        {
            std::string numberString = std::to_string(stones[i]);
            if( stones[i] == 0 )
            {
                stones[i] = 1;
            }
            else if( numberString.size() % 2 == 0 )
            {
                int half = numberString.size() / 2;
                // set right half of digits to current stone
                unsigned long long newValue = std::stoull( numberString.substr(half, half) );
                stones[i] = newValue;
                // add a new stone with the left half of digits
                newValue = std::stoull( numberString.substr(0, half) );
                stones.insert(stones.begin()+i, newValue);
            }
            else
            {
                stones[i] = stones[i] * 2024;
            }
        }
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // change stones according to blinks
        std::vector<unsigned long long> stones = _initialStones;
        for(int i=0; i<TARGET_BLINKS; ++i)
        {
            blink(stones);
        }

        // count stones
        this->_firstPuzzleAnswer = stones.size();
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 11" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}