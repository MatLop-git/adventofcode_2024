#include "IAoCHelper.cpp"

#include <map>

class Region
{
private:
    int _minX = 100000;
    int _maxX = 0;
    int _minY = 100000;
    int _maxY = 0;
    int _deltaX = 0;
    int _deltaY = 0;
    std::vector< std::vector<bool> > _regionMap;

public:
    char _plantType;
    int _perimeter;
    std::vector< std::pair<int, int> > _nodes;

    int getArea() { return _nodes.size(); }
    int getPerimeter() { return _perimeter; }
    int getPrice() { return getArea() * getPerimeter(); }
    void setNodes(std::vector< std::pair<int, int> > nodes)
    {
        _nodes = nodes;
        // calculate min max x y
        _minX = 100000;
        _maxX = 0;
        _minY = 100000;
        _maxY = 0;
        for(int i=nodes.size()-1; i>=0; --i)
        {
            if( nodes[i].first < _minX )
            {
                _minX = nodes[i].first;
            }
            if( nodes[i].first > _maxX )
            {
                _maxX = nodes[i].first;
            }
            if( nodes[i].second < _minY )
            {
                _minY = nodes[i].second;
            }
            if( nodes[i].second > _maxY )
            {
                _maxY = nodes[i].second;
            }
        }
        // max values are included, so size+1
        _maxX++;
        _maxY++;

        // generate minified input
        _regionMap = std::vector< std::vector<bool> >(_maxY-_minY, std::vector<bool>(_maxX-_minX, false));
        for(int i=nodes.size()-1; i>=0; --i)
        {
            int x = nodes[i].first - _minX;
            int y = nodes[i].second - _minY;
            _regionMap[y][x] = true;
        }

        _perimeter = 0;
        // find horizontal sides for perimeter
        for(int y=0; y<_regionMap.size(); ++y)
        {
            bool findingStart = true;
            // look for region changes in file
            for(int x=0; x<_regionMap[y].size(); ++x)
            {
                if( (findingStart && _regionMap[y][x] == true) || (!findingStart && _regionMap[y][x] == false) )
                {
                    _perimeter++;
                    findingStart = !findingStart;
                }
            }
            // handle side at end of input
            if( !findingStart )
            {
                _perimeter++;
                findingStart = !findingStart;
            }
        }

        // find vertical sides for perimeter
        for(int x=0; x<_regionMap[0].size(); ++x)
        {
            bool findingStart = true;
            // look for region changes in column
            for(int y=0; y<_regionMap.size(); ++y)
            {
                if( (findingStart && _regionMap[y][x] == true) || (!findingStart && _regionMap[y][x] == false) )
                {
                    _perimeter++;
                    findingStart = !findingStart;
                }
            }
            // handle side at end of input
            if( !findingStart )
            {
                _perimeter++;
                findingStart = !findingStart;
            }
        }
    }
};

class Helper : public IAoCHelper
{
public:
    std::map<char, std::vector<int>> _regionsArea;
    std::map<char, int> _regionsPerimeter;

    std::map<char, std::vector<Region>> _plantsRegions;

    void prepareInput()
    {
//        prepareAreas();
//        preparePerimeter();

        // prepare regions
        for(int y=0; y<_fileInput.size(); ++y)
        {
            for(int x=0; x<_fileInput[y].size(); ++x)
            {
                if( _plantsRegions.find(_fileInput[y][x]) == _plantsRegions.end() )
                {
                    findRegions(_fileInput[y][x]);
                }
            }
        }
    }

    void findRegions(char plant)
    {
        std::vector<std::string> currentInput = _fileInput;
        for(int y=0; y<currentInput.size(); ++y)
        {
            for(int x=0; x<currentInput[y].size(); ++x)
            {
                if( currentInput[y][x] == plant )
                {
                    Region newRegion;
                    newRegion._plantType = plant;
                    std::vector< std::pair<int, int> > nodes = findRegionNodes(plant, currentInput, x, y);
                    newRegion.setNodes( nodes );
                    _plantsRegions[plant].push_back( newRegion );
                }
            }
        }
    }

    std::vector< std::pair<int, int> > findRegionNodes(char plant, std::vector<std::string> &currentInput, int x, int y)
    {
        std::vector< std::pair<int, int> > nodes;
        if( currentInput[y][x] == plant )
        {
            nodes.push_back( {x, y} );
            currentInput[y][x] = '#';
            // find around
            std::vector< std::pair<int, int> > moreNodes;
            if( x-1 >= 0 )
            {
                moreNodes = findRegionNodes(plant, currentInput, x-1, y);
                nodes.insert(nodes.end(), moreNodes.begin(), moreNodes.end());
            }
            if( x+1 < currentInput[0].size() )
            {
                moreNodes = findRegionNodes(plant, currentInput, x+1, y);
                nodes.insert(nodes.end(), moreNodes.begin(), moreNodes.end());
            }
            if( y-1 >= 0 )
            {
                moreNodes = findRegionNodes(plant, currentInput, x, y-1);
                nodes.insert(nodes.end(), moreNodes.begin(), moreNodes.end());
            }
            if( y+1 < currentInput.size() )
            {
                moreNodes = findRegionNodes(plant, currentInput, x, y+1);
                nodes.insert(nodes.end(), moreNodes.begin(), moreNodes.end());
            }
        }
        return nodes;
    }
/*
    void prepareAreas()
    {
        for(const auto& [region, area] : _regionsArea)
        {
            findRegion(region, currentInput);
        }
    }

    void prepareAreas2()
    {
        // count regions
        for(int y=0; y<_fileInput.size(); ++y)
        {
            for(int x=0; x<_fileInput[y].size(); ++x)
            {
                _regionsArea[ _fileInput[y][x] ]++;
            }
        }

//        std::cout << "area " << _regionsArea['B'] << std::endl;
    }

    void findRegion(char region)
    {
        std::vector<std::string> currentInput = _fileInput;

        for(int y=0; y<currentInput.size(); ++y)
        {
            for(int x=0; x<currentInput[y].size(); ++x)
            {
                if( _fileInput[y][x] == region )
                {
                    findRegion(region, currentInput, x, y);
                }
            }
        }
    }

    void preparePerimeters()
    {
    }

    void preparePerimeters2()
    {
        // find sides for perimeters
        for(const auto& [region, area] : _regionsArea)
        {
            // find horizontal sides
            for(int y=0; y<_fileInput.size(); ++y)
            {
                bool findingStart = true;
                // look for region changes in file
                for(int x=0; x<_fileInput[y].size(); ++x)
                {
                    if( (findingStart && _fileInput[y][x] == region) || (!findingStart && _fileInput[y][x] != region) )
                    {
                        _regionsPerimeter[region]++;
                        findingStart = !findingStart;
                    }
                }
                // handle side at end of input
                if( !findingStart )
                {
                    _regionsPerimeter[region]++;
                    findingStart = !findingStart;
                }
            }

            // find vertical sides
            for(int x=0; x<_fileInput[0].size(); ++x)
            {
                bool findingStart = true;
                // look for region changes in column
                for(int y=0; y<_fileInput.size(); ++y)
                {
                    if( (findingStart && _fileInput[y][x] == region) || (!findingStart && _fileInput[y][x] != region) )
                    {
                        _regionsPerimeter[region]++;
                        findingStart = !findingStart;
                    }
                }
                // handle side at end of input
                if( !findingStart )
                {
                    _regionsPerimeter[region]++;
                    findingStart = !findingStart;
                }
            }
        }
    }
*/
    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // calculate total price
        for(const auto& [plant, regions] : _plantsRegions)
        {
            for(int i=0; i<regions.size(); ++i)
            {
                Region region = regions[i];
                this->_firstPuzzleAnswer += region.getPrice();
                std::cout << plant << " " << i << ": " << region.getArea() << " x " << region.getPerimeter() << " $" << region.getPrice() << std::endl;
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
    std::cout << "Advent of Code 2024 - Day 12" << std::endl;

    const std::string inputFileName = "input";
//    const std::string inputFileName = "debuginput";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}