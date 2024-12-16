#include "IAoCHelper.cpp"

//#define DEBUG
#define MAP_FIX_TO_SKIP_OUTER_WALLS 1
#define GPS_TOP 100

class Helper : public IAoCHelper
{
public:
    int _mapWidth;
    int _mapHeight;

    std::pair<int, int> _robotPosition;
    std::vector< std::pair<int, int> > _boxesPosition;
    std::vector< std::pair<int, int> > _insideWallsPosition;
    enum Content { Empty = 0, Box, Wall };
    enum Direction { Left, Right, Up, Down };
    std::vector<Direction> _robotMoves;

    void prepareInput()
    {
        _mapWidth = _fileInput[0].size();
        bool parsingMovement = false;
        auto beginPos = 0;
        auto endPos = 0;
        std::string currentLine;
        for(int i=1; i<_fileInput.size(); ++i)
        {
            // parse map and positions
            if( !parsingMovement )
            {
                if( _fileInput[i+1].size() < 3 )
                {
                    parsingMovement = true;
                    _mapHeight = i+1;
                    i++;
                }
                else
                {
                    // exclude outer walls
                    currentLine = _fileInput[i].substr(1, _mapWidth-2);

                    // parse robot position
                    beginPos = currentLine.find_first_of('@');
                    if( beginPos != std::string::npos )
                    {
                        _robotPosition.first = beginPos+1;
                        _robotPosition.second = i;
                    }
                    
                    // parse boxes position
                    beginPos = 0;
                    do
                    {
                        beginPos = currentLine.find_first_of('O', beginPos);
                        if( beginPos != std::string::npos )
                        {
                            std::pair<int, int> boxPosition = {beginPos+1, i};
                            _boxesPosition.push_back( boxPosition );
                            beginPos++;
                        }
                    }
                    while( beginPos != std::string::npos );

                    // parse inside walls position
                    beginPos = 0;
                    do
                    {
                        beginPos = currentLine.find_first_of('#', beginPos);
                        if( beginPos != std::string::npos )
                        {
                            std::pair<int, int> wallPosition = {beginPos+1, i};
                            _insideWallsPosition.push_back( wallPosition );
                            beginPos++;
                        }
                    }
                    while( beginPos != std::string::npos );
                }
            }
            // parse robot movement
            else
            {
                for(int j=0; j<_fileInput[i].size(); ++j)
                {
                    switch( _fileInput[i][j] )
                    {
                        case '<':
                            _robotMoves.push_back( Direction::Left );
                            break;
                        case '>':
                            _robotMoves.push_back( Direction::Right );
                            break;
                        case '^':
                            _robotMoves.push_back( Direction::Up );
                            break;
                        case 'v':
                            _robotMoves.push_back( Direction::Down );
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    void printMapState(std::vector< std::vector<Content> > mapState, std::pair<int, int> currentRobotPosition)
    {
        std::cout << std::endl;
        for(int i=0; i<mapState.size(); ++i)
        {
            std::cout << '#';
            for(int j=0; j<mapState[i].size(); ++j)
            {
                if( mapState[i][j] == Content::Box )
                    std::cout << 'O';
                else if( mapState[i][j] == Content::Wall )
                    std::cout << '#';
                else if( currentRobotPosition.first-MAP_FIX_TO_SKIP_OUTER_WALLS == j && currentRobotPosition.second-MAP_FIX_TO_SKIP_OUTER_WALLS == i )
                    std::cout << '@';
                else
                    std::cout << '.';
            }
            std::cout << '#' << std::endl;
        }
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

//        std::cout << _boxesPosition.size() << " " <<  _insideWallsPosition.size() << " robot " << _robotPosition.first << "," << _robotPosition.second << " " << _robotMoves.size() << std::endl;

        // generate map
        std::vector< std::vector<Content> > mapState = std::vector< std::vector<Content> >(_mapHeight-MAP_FIX_TO_SKIP_OUTER_WALLS-MAP_FIX_TO_SKIP_OUTER_WALLS, std::vector<Content>(_mapWidth-MAP_FIX_TO_SKIP_OUTER_WALLS-MAP_FIX_TO_SKIP_OUTER_WALLS, Content::Empty));
        for(int i=0; i<_boxesPosition.size(); ++i)
        {
            mapState[_boxesPosition[i].second-MAP_FIX_TO_SKIP_OUTER_WALLS][_boxesPosition[i].first-1] = Content::Box;
        }
        for(int i=0; i<_insideWallsPosition.size(); ++i)
        {
            mapState[_insideWallsPosition[i].second-MAP_FIX_TO_SKIP_OUTER_WALLS][_insideWallsPosition[i].first-MAP_FIX_TO_SKIP_OUTER_WALLS] = Content::Wall;
        }

//        printMapState(mapState, _robotPosition);

        // move robot
        std::pair<int, int> currentRobotPosition = _robotPosition;
        for(int i=0; i<_robotMoves.size(); ++i)
        {
            int robotX = currentRobotPosition.first-MAP_FIX_TO_SKIP_OUTER_WALLS;
            int robotY = currentRobotPosition.second-MAP_FIX_TO_SKIP_OUTER_WALLS;
            bool handlingMove = true;
            switch( _robotMoves[i] )
            {
                case Direction::Left:
                {
                    for(int j=robotX-1; handlingMove && j>=0; --j)
                    {
                        if( mapState[robotY][j] == Content::Empty )
                        {
                            // move boxes
                            for(;j<robotX; ++j)
                            {
                                mapState[robotY][j] = mapState[robotY][j+1];
                            }
                            currentRobotPosition.first--;
                            handlingMove = false;
                        }
                        else if( mapState[robotY][j] == Content::Wall )
                        {
                            handlingMove = false;
                        }
                    }

                    break;
                }
                case Direction::Right:
                {
                    for(int j=robotX+1; handlingMove && j<mapState[robotY].size(); ++j)
                    {
                        if( mapState[robotY][j] == Content::Empty )
                        {
                            // move boxes
                            for(;j>robotX; --j)
                            {
                                mapState[robotY][j] = mapState[robotY][j-1];
                            }
                            currentRobotPosition.first++;
                            handlingMove = false;
                        }
                        else if( mapState[robotY][j] == Content::Wall )
                        {
                            handlingMove = false;
                        }
                    }

                    break;
                }
                case Direction::Up:
                {
                    for(int j=robotY-1; handlingMove && j>=0; --j)
                    {
                        if( mapState[j][robotX] == Content::Empty )
                        {
                            // move boxes
                            for(;j<robotY; ++j)
                            {
                                mapState[j][robotX] = mapState[j+1][robotX];
                            }
                            currentRobotPosition.second--;
                            handlingMove = false;
                        }
                        else if( mapState[j][robotX] == Content::Wall )
                        {
                            handlingMove = false;
                        }
                    }

                    break;
                }
                case Direction::Down:
                {
                    for(int j=robotY+1; handlingMove && j<mapState.size(); ++j)
                    {
                        if( mapState[j][robotX] == Content::Empty )
                        {
                            // move boxes
                            for(;j>robotY; --j)
                            {
                                mapState[j][robotX] = mapState[j-1][robotX];
                            }
                            currentRobotPosition.second++;
                            handlingMove = false;
                        }
                        else if( mapState[j][robotX] == Content::Wall )
                        {
                            handlingMove = false;
                        }
                    }

                    break;
                }
                default:
                    break;
            }

//            printMapState(mapState, currentRobotPosition);
        }

        // sum boxes GPS
        for(int i=0; i<mapState.size(); ++i)
        {
            for(int j=0; j<mapState[i].size(); ++j)
            {
                if( mapState[i][j] == Content::Box )
                {
                    this->_firstPuzzleAnswer += ((i+1) * GPS_TOP + j+1);
                }
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
    std::cout << "Advent of Code 2024 - Day 15" << std::endl;

#ifdef DEBUG
    const std::string inputFileName = "debuginput";
#else
    const std::string inputFileName = "input";
#endif
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}