#include "IAoCHelper.cpp"

class Helper : public IAoCHelper
{
    int _inputWidth;
    int _inputHeight;
    std::vector< std::vector<int> > _nodesMap;  // This should be a matrix of booleans, making it a matrix of int in an attempt to anticipate to second half problem :P

    void prepareInput()
    {
        _inputHeight = _fileInput.size();
        _inputWidth = _fileInput[0].size();
    }

    void addAntiNodes(std::pair<int, int> antennaPos1, std::pair<int, int> antennaPos2)
    {
        // calculate distance
        int distanceX = antennaPos2.first - antennaPos1.first;
        int distanceY = antennaPos2.second - antennaPos1.second;

        // add node at top
        std::pair<int, int> nodePos;
        nodePos.first = antennaPos1.first - distanceX;
        nodePos.second = antennaPos1.second - distanceY;
        if( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second >= 0) )
        {
            _nodesMap[nodePos.second][nodePos.first]++;
        }

        // add node at bottom
        nodePos.first = antennaPos2.first + distanceX;
        nodePos.second = antennaPos2.second + distanceY;
        if( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second < _inputHeight) )
        {
            _nodesMap[nodePos.second][nodePos.first]++;
        }
    }

    void addAntiNodesWithResonance(std::pair<int, int> antennaPos1, std::pair<int, int> antennaPos2)
    {
        // antennas are antinodes
        _nodesMap[antennaPos1.second][antennaPos1.first]++;
        _nodesMap[antennaPos2.second][antennaPos2.first]++;

        // calculate distance
        int distanceX = antennaPos2.first - antennaPos1.first;
        int distanceY = antennaPos2.second - antennaPos1.second;

        // add nodes at top
        std::pair<int, int> nodePos = antennaPos1;
        do
        {
            nodePos.first = nodePos.first - distanceX;
            nodePos.second = nodePos.second - distanceY;
            if( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second >= 0) )
            {
                _nodesMap[nodePos.second][nodePos.first]++;
            }
        }
        while( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second >= 0) );

        // add nodes at bottom
        nodePos = antennaPos2;
        do
        {
            nodePos.first = nodePos.first + distanceX;
            nodePos.second = nodePos.second + distanceY;
            if( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second < _inputHeight) )
            {
                _nodesMap[nodePos.second][nodePos.first]++;
            }
        }
        while( (nodePos.first >= 0) && (nodePos.first < _inputWidth) && (nodePos.second < _inputHeight) );
    }

    void findAntiNodes(bool withResonance)
    {
        _nodesMap = std::vector< std::vector<int> >(_inputHeight, std::vector<int>(_inputWidth, 0));

        // Look for antennas from left to right and top to bottom of file input
        for(int y=0; y<_inputHeight; ++y)
        {
            auto antennaPos = -1;
            do
            {
                antennaPos = _fileInput[y].find_first_not_of('.', antennaPos+1);
                if( antennaPos != std::string::npos )
                {
                    char frequency = _fileInput[y][antennaPos];

                    // search for second antenna on same line (to the right)
                    auto antennaPos2 = antennaPos;
                    do
                    {
                        antennaPos2 = _fileInput[y].find_first_of(frequency, antennaPos2+1);
                        if( antennaPos2 != std::string::npos )
                        {
                            if( withResonance == true)
                            {
                                addAntiNodesWithResonance({antennaPos, y}, {antennaPos2, y});
                            }
                            else
                            {
                                addAntiNodes({antennaPos, y}, {antennaPos2, y});
                            }
                        }
                    }
                    while( antennaPos2 != std::string::npos );

                    // search for antenna on next lines
                    for(int y2=y+1; y2<_inputHeight; ++y2)
                    {
                        antennaPos2 = -1;
                        do
                        {
                            antennaPos2 = _fileInput[y2].find_first_of(frequency, antennaPos2+1);
                            if( antennaPos2 != std::string::npos )
                            {
                                if( withResonance == true)
                                {
                                    addAntiNodesWithResonance({antennaPos, y}, {antennaPos2, y2});
                                }
                                else
                                {
                                    addAntiNodes({antennaPos, y}, {antennaPos2, y2});
                                }
                            }
                        }
                        while( antennaPos2 != std::string::npos );
                    }
                }
            }
            while( antennaPos != std::string::npos );
        }
    }

    void saveNodesMapForDebug()
    {
        // Create and open a text file
        std::ofstream outputFile("output.txt");

        // Write to the file
        for(int i=0; i<_nodesMap.size(); ++i)
        {
            for(int j=0; j<_nodesMap[i].size(); ++j)
            {
                outputFile << (_nodesMap[i][j] == 0 ? '.' : '#');
            }
            outputFile << "\n";
        }

        // Close the file
        outputFile.close();
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        findAntiNodes( false );

        // calculate number of antinodes
        for(int i=0; i<_nodesMap.size(); ++i)
        {
            for(int j=0; j<_nodesMap[i].size(); ++j)
            {
                if( _nodesMap[i][j] > 0 )
                {
                    _firstPuzzleAnswer++;
                }
            }
        }
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        findAntiNodes( true );

        // calculate number of antinodes
        for(int i=0; i<_nodesMap.size(); ++i)
        {
            for(int j=0; j<_nodesMap[i].size(); ++j)
            {
                if( _nodesMap[i][j] > 0 )
                {
                    _secondPuzzleAnswer++;
                }
            }
        }
    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 08" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}