#include "IAoCHelper.cpp"

class Helper : public IAoCHelper
{
    enum Direction { Left, Right, Up, Down };

    std::pair<int, int> _startTile;
    std::pair<int, int> _endTile;
    std::vector< std::vector<bool> > _maze;

    void prepareInput()
    {
        _maze = std::vector< std::vector<bool> >(_fileInput.size());
        for(int y=0; y<_fileInput.size(); ++y)
        {
            for(int x=0; x<_fileInput[y].size(); ++x)
            {
                if( _fileInput[y][x] == '#' )
                {
                    _maze[y].push_back( true );
                }
                else
                {
                    if( _fileInput[y][x] == 'S' )
                    {
                        _startTile.first = x;
                        _startTile.second = y;
                    }
                    else if( _fileInput[y][x] == 'E' )
                    {
                        _endTile.first = x;
                        _endTile.second = y;
                    }
                    _maze[y].push_back( false );
                }
            }
        }
    }

    std::vector< std::vector<bool> > generateMapState()
    {
//        std::vector< std::vector<bool> > mapState;
//        return mapState;
        return _maze;
    }

    void printMapState()
    {
        for(int i=0; i<_maze.size(); ++i)
        {
            for(int j=0; j<_maze[i].size(); ++j)
            {
                std::cout << (_maze[i][j] ? '#' : '.');
            }
            std::cout << std::endl;
        }
    }

    long long findPath(std::vector< std::vector<bool> > mapState, int x, int y, Direction direction)
    {
        if( _endTile.first == x && _endTile.second == y )
        {
            // reached end tile
            return 0;
        }

        // avoid going through this tile again
        mapState[y][x] = true;

        long long score = -1;
        long long nextScore = -1;
        // try go right
        if( (direction != Direction::Left) && (x+1 < mapState[y].size()) && (mapState[y][x+1] == false) )
        {
            nextScore = findPath(mapState, x+1, y, Direction::Right);
            if( nextScore > -1 )
            {
                nextScore += (direction == Direction::Right ? 1 : 1001);
                if( score == -1 || nextScore < score )
                {
                    score = nextScore;
                }
            }
        }
        // try go left
        if( (direction != Direction::Right) && (x-1 >= 0) && (mapState[y][x-1] == false) )
        {
            nextScore = findPath(mapState, x-1, y, Direction::Left);
            if( nextScore > -1 )
            {
                nextScore += (direction == Direction::Left ? 1 : 1001);
                if( score == -1 || nextScore < score )
                {
                    score = nextScore;
                }
            }
        }
        // try go up
        if( (direction != Direction::Down) && (y-1 >= 0) && (mapState[y-1][x] == false) )
        {
            nextScore = findPath(mapState, x, y-1, Direction::Up);
            if( nextScore > -1 )
            {
                nextScore += (direction == Direction::Up ? 1 : 1001);
                if( score == -1 || nextScore < score )
                {
                    score = nextScore;
                }
            }
        }
        // try go down
        if( (direction != Direction::Up) && (y+1 < mapState.size()) && (mapState[y+1][x] == false) )
        {
            nextScore = findPath(mapState, x, y+1, Direction::Down);
            if( nextScore > -1 )
            {
                nextScore += (direction == Direction::Down ? 1 : 1001);
                if( score == -1 || nextScore < score )
                {
                    score = nextScore;
                }
            }
        }

        // restore tile emptyness
        mapState[y][x] = false;

        return score;
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        std::vector< std::vector<bool> > mapState = generateMapState();
        printMapState();

        // calculate path
        Direction currentDirection = Direction::Right;
        this->_firstPuzzleAnswer = findPath(mapState, _startTile.first, _startTile.second, currentDirection);
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 16" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    unsigned long long answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}