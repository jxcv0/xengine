#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace
{
    short _input = 0;
    short _forwardKey = 0x0001;     // bit 1
    short _leftKey = 0x0002;        // bit 2
    short _backwardKey = 0x0004;    // bit 3
    short _rightKey = 0x0008;       // bit 4
} // namespace

namespace xen::input
{
    // set the input bits
    void set(short i) { _input = i; }
    void clear() { _input = 0; }

    // key checks
    bool forward() { return ((_forwardKey & _input) != 0); }
    bool backward() { return ((_backwardKey & _input) != 0); }
    bool left() { return ((_leftKey & _input) != 0); }
    bool right() { return ((_rightKey & _input) != 0); }
} // namespace xen::input

#endif // GAMESTATE_H
