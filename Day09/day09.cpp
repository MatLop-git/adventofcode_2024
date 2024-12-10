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

        // calculate checksum
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

    void saveOrderedDiskMapFileForDebug(std::vector<std::pair<int, int>> rawOrderedDiskMap)
    {
        // Create and open a text file
        std::ofstream outputFile("output.txt");

        // Write to the file
        for(int i=0; i<rawOrderedDiskMap.size(); ++i)
        {
            for(int j=0; j<rawOrderedDiskMap[i].second; ++j)
            {
                outputFile << std::to_string(rawOrderedDiskMap[i].first);
                outputFile << ",";
            }
        }

        // Close the file
        outputFile.close();
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // initialize free space and ordered disk map counters
        std::vector<std::pair<int, int>> rawOrderedDiskMap; // {id, size}   id -1 means free space
        int pos = 0;
        int size = 0;
        int diskUsage = 0;
        for(int i=0; i<_initialFilesMap.size(); ++i)
        {
            diskUsage += _initialFilesMap[i];
            rawOrderedDiskMap.push_back( {i, _initialFilesMap[i]} );
            if( i < _initialFreeSpaceMap.size() )
            {
                size = _initialFreeSpaceMap[i];
                if( size > 0 )
                {
                    rawOrderedDiskMap.push_back( {-1, size} );
                }
            }
        }

        // move file blocks from the end
        int i = rawOrderedDiskMap.size()-1;
        do
        {
            std::pair<int, int> block = rawOrderedDiskMap[i];
            int blockId = block.first;
            if( blockId < 0 )
            {
                // rawOrderedDiskMap.erase(rawOrderedDiskMap.begin()+i);
                i--;
            }
            else
            {
                int blockSize = block.second;
                // find free space for the file blocks to be arranged
                int j = 0;
                bool found = false;
                do
                {
                    if( rawOrderedDiskMap[j].first < 0 && rawOrderedDiskMap[j].second >= blockSize )
                    {
                        found = true;
                        // empty the position used by current block, merge free spaces if needed
                        if( rawOrderedDiskMap[i-1].first != -1 && (i+1 < rawOrderedDiskMap.size() || rawOrderedDiskMap[i+1].first != -1) )
                        {
                            rawOrderedDiskMap[i].first = -1;
                        }
                        else
                        {
                            if( rawOrderedDiskMap[i-1].first == -1 && rawOrderedDiskMap[i+1].first == -1 )
                            {
                                rawOrderedDiskMap[i-1].second += blockSize;
                                rawOrderedDiskMap[i-1].second += rawOrderedDiskMap[i+1].second;
                                rawOrderedDiskMap.erase(rawOrderedDiskMap.begin()+i+1);
                            }
                            else if( rawOrderedDiskMap[i-1].first == -1 )
                            {
                                rawOrderedDiskMap[i-1].second += blockSize;
                            }
                            else
                            {
                                rawOrderedDiskMap[i+1].second += blockSize;
                            }
                            rawOrderedDiskMap.erase(rawOrderedDiskMap.begin()+i);
                        }
 
                        // update remaining free space after block move
                        if( rawOrderedDiskMap[j].second == blockSize )
                        {
                            rawOrderedDiskMap[j].first = blockId;
                        }
                        else
                        {
                            rawOrderedDiskMap[j].second -= blockSize;
                            // insert block in updated position
                            rawOrderedDiskMap.insert(rawOrderedDiskMap.begin()+j, block);
                            i++;
                        }
                    }
                    j++;
                }
                while( found == false && j < i );

                i--;
            }
        }
        while( i >= 0 );

        // calculate checksum
        i = 0;
        for(int j=0; j<rawOrderedDiskMap.size(); ++j)
        {
            int blockId = rawOrderedDiskMap[j].first;
            int blockSize = rawOrderedDiskMap[j].second;
            if( blockId > -1 )
            {
                for(int k=0; k<blockSize; ++k)
                {
                    unsigned long long checksum = (blockId * (i + k));
                    _secondPuzzleAnswer += checksum;
                }
            }
            i += blockSize;
        }
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