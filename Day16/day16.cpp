#include "IAoCHelper.cpp"

#include <chrono>
#include <thread>

class Helper : public IAoCHelper
{
    enum Direction { Left, Right, Up, Down };

    std::pair<int, int> _startTile;
    std::pair<int, int> _endTile;
    std::vector< std::vector<bool> > _maze;

    void prepareInput()
    {
        // parse maze, start tile and end tile
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

    std::vector< std::vector<unsigned long long> > generateMapState()
    {
        // transform maze (false,true) matrix into states (0,ULLONG_MAX) matrix
        std::vector< std::vector<unsigned long long> > mapState(_maze.size(), std::vector<unsigned long long>(_maze[0].size(), 0));
        for(int y=0; y<_maze.size(); ++y)
        {
            for(int x=0; x<_maze[y].size(); ++x)
            {
                if( _maze[y][x] )
                {
                    mapState[y][x] = ULLONG_MAX;
                }
            }
        }
        return mapState;
    }

    void printMapState(std::vector< std::vector<unsigned long long> > mapState)
    {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        for(int y=0; y<mapState.size(); ++y)
        {
            for(int x=0; x<mapState[y].size(); ++x)
            {
                if( mapState[y][x] == 0 )
                {
                    std::cout << '.';
                }
                else if( mapState[y][x] == ULLONG_MAX )
                {
                    std::cout << '#';
                }
                else
                {
                    std::cout << 'x';
                }
            }
            std::cout << std::endl;
        }
    }

    long long findPath(std::vector< std::vector<unsigned long long> > &mapState, int x, int y, Direction direction, unsigned long long &minScore, unsigned long long score=0)
    {
        if( score > minScore )
        {
            // this path was not explored yet but it is already taking longer the previous best past, so stop going through it
            return 0;
        }

        if( _endTile.first == x && _endTile.second == y )
        {
            // reached end tile, score is valid so return it
            return score;
        }

        if( mapState[y][x] > 0 && mapState[y][x] < score )
        {
            // this tile was explored in a previous run in a better way (less score to reach it), so stop going through this path
            return 0;
        }

        // score this tile for next paths
        mapState[y][x] = score;

        unsigned long long nextScore = 0;
        // try go right
        if( (direction != Direction::Left) && (x+1 < mapState[y].size()) && (mapState[y][x+1] != ULLONG_MAX) )
        {
            nextScore = findPath(mapState, x+1, y, Direction::Right, minScore, score+(direction == Direction::Right ? 1 : 1001));
            if( nextScore > 0 )
            {
                minScore = nextScore;
            }
        }
        // try go up
        if( (direction != Direction::Down) && (y-1 >= 0) && (mapState[y-1][x] != ULLONG_MAX) )
        {
            nextScore = findPath(mapState, x, y-1, Direction::Up, minScore, score+(direction == Direction::Up ? 1 : 1001));
            if( nextScore > 0 )
            {
                minScore = nextScore;
            }
        }
        // try go left
        if( (direction != Direction::Right) && (x-1 >= 0) && (mapState[y][x-1] != ULLONG_MAX) )
        {
            nextScore = findPath(mapState, x-1, y, Direction::Left, minScore, score+(direction == Direction::Left ? 1 : 1001));
            if( nextScore > 0 )
            {
                minScore = nextScore;
            }
        }
        // try go down
        if( (direction != Direction::Up) && (y+1 < mapState.size()) && (mapState[y+1][x] != ULLONG_MAX) )
        {
            nextScore = findPath(mapState, x, y+1, Direction::Down, minScore, score+(direction == Direction::Down ? 1 : 1001));
            if( nextScore > 0 )
            {
                minScore = nextScore;
            }
        }

        return 0;
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = ULLONG_MAX;

        std::vector< std::vector<unsigned long long> > mapState = generateMapState();

        // calculate path
        Direction currentDirection = Direction::Right;
        findPath(mapState, _startTile.first, _startTile.second, currentDirection, this->_firstPuzzleAnswer);
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