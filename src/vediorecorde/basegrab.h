
#ifndef BASEGRAB_H
#define BASEGRAB_H

class BaseGrab
{
    public:
        virtual ~BaseGrab() {}
        virtual void start()=0;
        virtual void saveFile()=0;
        virtual void stop()=0;
};

#endif