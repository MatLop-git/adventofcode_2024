#include "IAoCHelper.cpp"

static const int MAX_ATTEMPTS = 100;
static const int TOKENS_A = 3;
static const int TOKENS_B = 1;

class Machine
{
public:
    std::pair<int, int> _buttonA;
    std::pair<int, int> _buttonB;
    std::pair<int, int> _prizePosition;
};

class Helper : public IAoCHelper
{
public:
    std::vector<Machine> _machines;

    void prepareInput()
    {
        for(int i=0; i<_fileInput.size(); i=i+4)
        {
            Machine machine;
            // button A
            auto beginPos = _fileInput[i].find_first_of('X') + 2;
            auto endPos = _fileInput[i].find_first_of(',', beginPos);
            machine._buttonA.first = std::stoi( _fileInput[i].substr(beginPos, endPos-beginPos) );
            beginPos = _fileInput[i].find_first_of('Y', beginPos+1) + 2;
            machine._buttonA.second = std::stoi( _fileInput[i].substr(beginPos) );

            // button B
            beginPos = _fileInput[i+1].find_first_of('X') + 2;
            endPos = _fileInput[i+1].find_first_of(',', beginPos);
            machine._buttonB.first = std::stoi( _fileInput[i+1].substr(beginPos, endPos-beginPos) );
            beginPos = _fileInput[i+1].find_first_of('Y', beginPos+1) + 2;
            machine._buttonB.second = std::stoi( _fileInput[i+1].substr(beginPos) );

            // prize location
            beginPos = _fileInput[i+2].find_first_of('X') + 2;
            endPos = _fileInput[i+2].find_first_of(',', beginPos);
            machine._prizePosition.first = std::stoi( _fileInput[i+2].substr(beginPos, endPos-beginPos) );
            beginPos = _fileInput[i+2].find_first_of('Y', beginPos+1) + 2;
            machine._prizePosition.second = std::stoi( _fileInput[i+2].substr(beginPos) );

            _machines.push_back( machine );
        }
    }
/*
    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        std::pair<int, int> currentDistance;
        for(int i=0; i<_machines.size(); ++i)
        {
            // press B (cheaper) until reaching/passing prize position
            Machine machine = _machines[ i ];
            int bToPrizeX = machine._prizePosition.first / machine._buttonB.first;
            if( machine._prizePosition.first % machine._buttonB.first != 0 )
            {
                bToPrizeX++;
            }
            int bToPrizeY = machine._prizePosition.second / machine._buttonB.second;
            if( machine._prizePosition.second % machine._buttonB.second != 0 )
            {
                bToPrizeY++;
            }
            int bToPrize = std::max(bToPrizeX, bToPrizeY);
            int aToPrize = 0;
            bool prizeUnreachable = false;
            bool prizeReached = false;

            // handle pressing B is not enought to reach prize position
            if( bToPrize > MAX_ATTEMPTS )
            {
                bToPrize = MAX_ATTEMPTS;
                currentDistance.first = bToPrize * machine._buttonB.first;
                currentDistance.second = bToPrize * machine._buttonB.second;
                std::pair<int, int> remainingDistance = { machine._prizePosition.first - currentDistance.first, machine._prizePosition.second - currentDistance.second };

                // press A until reaching/passing prize position
                int aToPrizeX = remainingDistance.first / machine._buttonA.first;
                if( remainingDistance.first % machine._buttonA.first != 0 )
                {
                    aToPrizeX++;
                }
                int aToPrizeY = remainingDistance.second / machine._buttonA.second;
                if( remainingDistance.second % machine._buttonA.second != 0 )
                {
                    aToPrizeY++;
                }
                aToPrize = std::max(aToPrizeX, aToPrizeY);
                // validate if prize cannot be reach by max pressing both A and B
                if( aToPrize > MAX_ATTEMPTS )
                {
                    prizeUnreachable = false;
                }
                else
                {
                    currentDistance.first += aToPrize * machine._buttonA.first;
                    currentDistance.second += aToPrize * machine._buttonA.second;
                }
            }
            else
            {
                currentDistance = { bToPrize * machine._prizePosition.first, bToPrize * machine._prizePosition.second };
            }

            // try combinations until reach exact prize position
            while( prizeUnreachable == false && prizeReached == false )
            {
                prizeReached = (currentDistance.first == machine._prizePosition.first && currentDistance.second == machine._prizePosition.second);
                if(prizeReached == false)
                {
                    // if past prize position, less B
                    if( currentDistance.first > machine._prizePosition.first )
                    {
                        bToPrize--;
                        currentDistance.first -= machine._buttonB.first;
                        currentDistance.second -= machine._buttonB.second;
                    }
                    // if yet to reach prize position, press A
                    else
                    {
                        aToPrize++;
                        currentDistance.first += machine._buttonA.first;
                        currentDistance.second += machine._buttonA.second;
                    }

                    // validate if prize cannot be reach by max pressing A or with A and B combinations
                    prizeUnreachable = (aToPrize > MAX_ATTEMPTS || bToPrize < 0);
                }
            }

            if( prizeReached )
            {
                std::cout << (i+1) << " " << aToPrize << " " << bToPrize << " = " << ((aToPrize * TOKENS_A) + (bToPrize * TOKENS_B)) << std::endl;
                this->_firstPuzzleAnswer += (aToPrize * TOKENS_A) + (bToPrize * TOKENS_B);
            }
        }
    }
*/
    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        std::pair<int, int> currentDistance;
        for(int i=0; i<_machines.size(); ++i)
        {
            // press B (cheaper) until reaching/passing prize position
            Machine machine = _machines[ i ];
            int bToPrizeX = machine._prizePosition.first / machine._buttonB.first;
            if( machine._prizePosition.first % machine._buttonB.first != 0 )
            {
                bToPrizeX++;
            }
            int bToPrizeY = machine._prizePosition.second / machine._buttonB.second;
            if( machine._prizePosition.second % machine._buttonB.second != 0 )
            {
                bToPrizeY++;
            }
            int bToPrize = std::max(bToPrizeX, bToPrizeY);
            int aToPrize = 0;
            bool prizeUnreachable = false;
            bool prizeReached = false;

            // handle pressing B is not enought to reach prize position
            if( bToPrize > MAX_ATTEMPTS )
            {
                bToPrize = MAX_ATTEMPTS;
                currentDistance.first = bToPrize * machine._buttonB.first;
                currentDistance.second = bToPrize * machine._buttonB.second;
                std::pair<int, int> remainingDistance = { machine._prizePosition.first - currentDistance.first, machine._prizePosition.second - currentDistance.second };

                int aToPrizeX = 0;
                int aToPrizeY = 0;
                // press A until reaching/passing prize position
                if( remainingDistance.first > 0 )
                {
                    aToPrizeX = remainingDistance.first / machine._buttonA.first;
                    if( remainingDistance.first % machine._buttonA.first != 0 )
                    {
                        aToPrizeX++;
                    }
                }
                if( remainingDistance.second > 0 )
                {
                    aToPrizeY = remainingDistance.second / machine._buttonA.second;
                    if( remainingDistance.second % machine._buttonA.second != 0 )
                    {
                        aToPrizeY++;
                    }
                    aToPrize = std::max(aToPrizeX, aToPrizeY);
                }

                // validate if prize cannot be reach by max pressing both A and B
                if( aToPrize > MAX_ATTEMPTS )
                {
                    prizeUnreachable = false;
                }
                else
                {
                    currentDistance.first += aToPrize * machine._buttonA.first;
                    currentDistance.second += aToPrize * machine._buttonA.second;
                }
            }
            else
            {
                currentDistance = { bToPrize * machine._buttonB.first, bToPrize * machine._buttonB.second };
            }

            // try combinations until reach exact prize position
            while( prizeUnreachable == false && prizeReached == false )
            {
                prizeReached = (currentDistance.first == machine._prizePosition.first && currentDistance.second == machine._prizePosition.second);
                if(prizeReached == false)
                {
                    // if past prize position, less B
                    if( currentDistance.first > machine._prizePosition.first )
                    {
                        bToPrize--;
                        currentDistance.first -= machine._buttonB.first;
                        currentDistance.second -= machine._buttonB.second;
                    }
                    // if yet to reach prize position, press A
                    else
                    {
                        aToPrize++;
                        currentDistance.first += machine._buttonA.first;
                        currentDistance.second += machine._buttonA.second;
                    }

                    // validate if prize cannot be reach by max pressing A or with A and B combinations
                    prizeUnreachable = (aToPrize > MAX_ATTEMPTS || bToPrize < 0);
                }
            }

            if( prizeReached )
            {
                std::cout << (i+1) << " " << aToPrize << " " << bToPrize << " = " << ((aToPrize * TOKENS_A) + (bToPrize * TOKENS_B)) << std::endl;
                this->_firstPuzzleAnswer += (aToPrize * TOKENS_A) + (bToPrize * TOKENS_B);
            }
        }
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;
    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 13" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}