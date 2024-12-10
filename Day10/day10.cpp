#include "IAoCHelper.cpp"

const int TRAIL_SIZE = 10;

class TrailHead
{
public:
    std::pair<int, int> _position;  // x, y
    std::vector< std::vector< std::pair<int, int> > > _trails;

    TrailHead(int x, int y)
    {
        _position = {x, y};
    }

    std::pair<int, int> getPosition() { return _position; }
    int getTrailsCount() { return _trails.size(); }

    int getScore()
    {
        // Only scores the trails that leads to different end tops
        std::vector< std::pair<int,int> > scoredTops;
        for(int i=0; i<_trails.size(); ++i)
        {
            std::pair<int, int> topPosition = _trails[i][TRAIL_SIZE-2];
            bool alreadyScored = false;
            int j=0;
            while( alreadyScored == false && j < scoredTops.size() )
            {
                alreadyScored = (topPosition.first == scoredTops[j].first && topPosition.second == scoredTops[j].second);
                j++;
            }

            if( alreadyScored == false )
            {
                scoredTops.push_back(topPosition);
            }
        }

        return scoredTops.size();
    }

    int getRating()
    {
        return getTrailsCount();
    }

    void addTrail(std::vector< std::pair<int, int> > trail) { _trails.push_back(trail); }
};

class Helper : public IAoCHelper
{
    int _maxX;
    int _maxY;

    std::vector<TrailHead> _trailHeads;

    void prepareInput()
    {
        _maxX = _fileInput[0].size();
        _maxY = _fileInput.size();

        // find the trailheads and their trails
        for(int i; i<_fileInput.size(); ++i)
        {
            int pos = 0;
            do
            {
                pos = _fileInput[i].find_first_of('0', pos);
                if( pos != std::string::npos )
                {
                    TrailHead trailHead(pos, i);
                    findTrails( trailHead );
                    if( trailHead.getTrailsCount() > 0 )
                    {
                        _trailHeads.push_back(trailHead);
                    }
                    pos++;
                }
            }
            while(pos != std::string::npos);
        }
    }

    void findTrails(TrailHead &trailHead)
    {
        std::vector<std::pair<int, int>> trail;
        std::pair<int, int> initialPosition = trailHead.getPosition();

        // find '1' up
        std::pair<int, int> nextPosition;
        if( initialPosition.second-1 >= 0 )
        {
            nextPosition.first = initialPosition.first;
            nextPosition.second = initialPosition.second - 1;
            findTrails(trailHead, 1, trail, nextPosition);
        }
        // find '1' right
        if( initialPosition.first+1 < _maxX )
        {
            nextPosition.first = initialPosition.first + 1;
            nextPosition.second = initialPosition.second;
            findTrails(trailHead, 1, trail, nextPosition);
        }
        // find '1' bottom
        if( initialPosition.second+1 < _maxY )
        {
            nextPosition.first = initialPosition.first;
            nextPosition.second = initialPosition.second + 1;
            findTrails(trailHead, 1, trail, nextPosition);
        }
        // find '1' left
        if( initialPosition.first-1 >= 0 )
        {
            nextPosition.first = initialPosition.first - 1;
            nextPosition.second = initialPosition.second;
            findTrails(trailHead, 1, trail, nextPosition);
        }
    }

    void findTrails(TrailHead &trailHead, int height, std::vector<std::pair<int, int>> trail, std::pair<int, int> position)
    {
        char heightChar = height + '0';
        if( _fileInput[position.second][position.first] == heightChar )
        {
            trail.push_back( position );
            if( height == TRAIL_SIZE-1 )
            {
                trailHead.addTrail(trail);
            }
            else
            {
                // find '1' up
                std::pair<int, int> nextPosition;
                if( position.second-1 >= 0 )
                {
                    nextPosition.first = position.first;
                    nextPosition.second = position.second - 1;
                    findTrails(trailHead, height+1, trail, nextPosition);
                }
                // find '1' right
                if( position.first+1 < _maxX )
                {
                    nextPosition.first = position.first + 1;
                    nextPosition.second = position.second;
                    findTrails(trailHead, height+1, trail, nextPosition);
                }
                // find '1' bottom
                if( position.second+1 < _maxY )
                {
                    nextPosition.first = position.first;
                    nextPosition.second = position.second + 1;
                    findTrails(trailHead, height+1, trail, nextPosition);
                }
                // find '1' left
                if( position.first-1 >= 0 )
                {
                    nextPosition.first = position.first - 1;
                    nextPosition.second = position.second;
                    findTrails(trailHead, height+1, trail, nextPosition);
                }
            }
        }
    }

    virtual void calculateFirstPuzzleAnswer()
    {
        this->_firstPuzzleAnswer = 0;

        // get the trailheads score
        for(int i=0; i<_trailHeads.size(); ++i)
        {
            TrailHead trailHead = _trailHeads[i];
            _firstPuzzleAnswer += trailHead.getScore();
        }
    }

    virtual void calculateSecondPuzzleAnswer()
    {
        this->_secondPuzzleAnswer = 0;

        // get the trailheads rating
        for(int i=0; i<_trailHeads.size(); ++i)
        {
            TrailHead trailHead = _trailHeads[i];
            _secondPuzzleAnswer += trailHead.getRating();
        }
    }
};

int main()
{
    std::cout << "Advent of Code 2024 - Day 10" << std::endl;

    const std::string inputFileName = "input";
    Helper helper;
    helper.calculateAnswers(inputFileName);

    int answer = helper.getFirstPuzzleAnswer();
    std::cout << "First half answer: " << answer << std::endl;

    answer = helper.getSecondPuzzleAnswer();
    std::cout << "Second half answer: " << answer << std::endl;
}