#ifndef __DEFER_H__
#define __DEFER_H__

#include <functional>
#include <utility>

using namespace std;

template<typename F>
class Defer_Worker
{
public:
    Defer_Worker(const F&& t) : t(t)
    {}

    ~Defer_Worker()
    { t(); }

private:
    const F t;
};

template<typename F>
const Defer_Worker<F> DeferFactory(const F&& t)
{
    return Defer_Worker<F>(forward<decltype(t)>(t));
}

#define Defer(x) DeferFactory([&](){x;})

#endif
