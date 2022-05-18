#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace xen
{
    // interprets input bits
    struct Input
    {
        // set the input bits
        void set(short bits) { _input = bits; }
        void clear() { _input = 0x0; }

        // key checks
        bool forward() { return ((_forwardKey & _input) != 0); }
        bool backward() { return ((_backwardKey & _input) != 0); }
        bool left() { return ((_leftKey & _input) != 0); }
        bool right() { return ((_rightKey & _input) != 0); }

    private:
        // distant TODO make these changeable in menu
        short _input = 0x0;
        short _forwardKey = 0x0001;     // bit 1
        short _leftKey = 0x0002;        // bit 3
        short _backwardKey = 0x0004;    // bit 1
        short _rightKey = 0x0008;       // bit 4
    };

    struct GameState
    {
        float currentTime;
        float deltaTime;
    };
} // namespace xen

#endif // GAMESTATE_H
