#include "Arduino.h"

class L298NDriver {
    public:
        L298NDriver();
        void init(int M1APIN, int M1BPIN, int M2APIN, int M2BPIN, int ENA, int ENB);
        void move(int speed, bool forward);
        void turn(int angleVel, bool forward, bool turnLeft);
        void stop();
    private:
        int _M1APIN;
        int _M1BPIN;
        int _M2APIN;
        int _M2BPIN;
        int _ENA;
        int _ENB;
};