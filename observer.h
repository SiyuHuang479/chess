#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Chess;
class Observer {
  public:
    virtual void notify(Chess& c, int x1, int y1, int x2, int y2) = 0;
    virtual ~Observer() = default;
};

#endif
