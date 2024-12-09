#include "IAoCHelper.cpp"

class Helper : public IAoCHelper
{
    std::vector<int> _initialFilesMap;
    std::vector<int> _initialFreeSpaceMap;

    void prepareInput()
    {
        bool isFile = true;
        // _fileInput is only one line
        std::string currentLine = _fileInput[0];
        for(int i=0; i<currentLine.size(); ++i)
        {
            int value = (currentLine[i] - 48);  // 48 is char for 0
            if( isFile )
            {
                _initialFilesMap.push_back( value );
            }
            else
            {
                _initialFreeSpaceMap.push_back( value );
            }
            isFile = !isFile;
        }
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // initialize free space and ordered disk map counters
        std::vector<std::pair<int, int>> freeSpaceMapAux;
        std::vector<int> rawOrderedDiskMap;
        int pos = 0;
        int size = 0;
        int diskUsage = 0;
        for(int i=0; i<_initialFilesMap.size(); ++i)
        {
            diskUsage += _initialFilesMap[i];
            pos += _initialFilesMap[i];
            for(int j=_initialFilesMap[i]; j>0; --j)
            {
                rawOrderedDiskMap.push_back(i);
            }

            if( i < _initialFreeSpaceMap.size() )
            {
                size = _initialFreeSpaceMap[i];
                if( size > 0 )
                {
                    freeSpaceMapAux.push_back( {pos, size} );
                    pos += size;
                }
            }
        }

        // move file blocks from the end
        int i = _initialFilesMap.size()-1;
        do
        {
            int size = _initialFilesMap[i];
            // remove the file blocks to be arranged
            rawOrderedDiskMap.erase(rawOrderedDiskMap.end()-size, rawOrderedDiskMap.end());
            // find free space for the file blocks to be arranged
            pos = freeSpaceMapAux[0].first;
            do
            {
                int availableSpace = freeSpaceMapAux[0].second;
                int remainingSpace = availableSpace - size;
                // fill the free space with the file blocks to be arranged
                if( remainingSpace > 0 )
                {
                    // update free space
                    freeSpaceMapAux[0].first += size;
                    freeSpaceMapAux[0].second = remainingSpace;
                    // update raw disk map
                    for(int j=size; j>0; --j)
                    {
                        rawOrderedDiskMap.insert(rawOrderedDiskMap.begin()+pos, i);
                    }
                    size = 0;
                }
                // if the file blocks are more than the free space slot, add the amount that fits and handle the remaining on next loop
                else
                {
                    // update free space
                    freeSpaceMapAux.erase(freeSpaceMapAux.begin());
                    // update raw disk map
                    for(int j=availableSpace; j>0; --j)
                    {
                        rawOrderedDiskMap.insert(rawOrderedDiskMap.begin()+pos, i);
                    }
                    size -= availableSpace;
                }
                pos = freeSpaceMapAux[0].first;
            }
            while( size > 0  && pos < diskUsage );

            // if there are file blocks to move but there is no more free space slots (map is fully compacted), return the remaining to the end of map
            if( size > 0 )
            {
                for(int j=size; j>0; --j)
                {
                    rawOrderedDiskMap.push_back(i);
                }
            }
            i--;
        }
        while( i >= 0 && pos < diskUsage );

//        saveOrderedDiskMapFileForDebug(rawOrderedDiskMap);

        for(int i=rawOrderedDiskMap.size()-1; i>=0; --i)
        {
            unsigned long long checksum = (rawOrderedDiskMap[i] * i);
            _firstPuzzleAnswer += checksum;
        }
    }

    void saveOrderedDiskMapFileForDebug(std::vector<int> rawOrderedDiskMap)
    {
        // Create and open a text file
        std::ofstream outputFile("output.txt");

        // Write to the file
        for(int i=0; i<rawOrderedDiskMap.size(); ++i)
        {
            outputFile << std::to_string(rawOrderedDiskMap[i]);
            outputFile << ",";
        }

        // Close the file
        outputFile.close();
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // TODO
    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 09" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}