
#ifndef MATCH_H
#define MATCH_H

struct Match {
    int line;
    int col;

    Match(int lineNumber = 0, int column = 0)
        : line(lineNumber), col(column) {
    }
};

#endif
