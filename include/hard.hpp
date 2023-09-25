#pragma once

struct hard
{
    int fa;
    int fi;
    hard(int fa, int fi) : fa(fa), fi(fi) {};
    hard(const hard &) = default;
    hard(hard &&) = delete;
};

bool operator==(const hard &lhs, const hard &rhs)
{
    return lhs.fa == rhs.fa && lhs.fi == rhs.fi;
}