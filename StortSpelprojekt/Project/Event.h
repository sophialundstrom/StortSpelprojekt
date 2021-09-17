#pragma once
#include <bitset>

struct RawDelta
{
	int x, y;

	RawDelta() = default;
};

struct MouseCoords
{
	int x, y;
};

class Event
{
private:
	//MOUSE
	static RawDelta rd;

	static MouseCoords mc;
	static bool leftIsPressed;
	static bool rightIsPressed;

	//KEYBOARD
	static constexpr unsigned int keys = 256;
	static std::bitset<keys> keystates;

	//STATE
	static bool movement;
public:
	//MOUSE
	static MouseCoords MousePosition() { return mc; }
	static void OnRawDelta(int dx, int dy) { rd.x = dx; rd.y = dy; }
	static void ClearRawDelta() { rd.x = 0; rd.y = 0; }
	static RawDelta ReadRawDelta() { return rd; }

	static bool LeftIsClicked() { return leftIsPressed; }
	static void OnLeftClick(int x, int y) { leftIsPressed = true; mc = { x, y }; }
	static void OnLeftRelease() { leftIsPressed = false; }

	static bool RightIsClicked() { return rightIsPressed; }
	static void OnRightClick(int x, int y) { rightIsPressed = true; mc = { x, y }; }
	static void OnRightRelease() { rightIsPressed = false; }

	//KEYBOARD
	static bool KeyIsPressed(unsigned char keycode) { return keystates[keycode]; }
	static void OnKeyPressed(unsigned char keycode) { keystates.set(keycode, true); }
	static void OnKeyReleased(unsigned char keycode) { keystates.set(keycode, false); }

	//STATE
	static bool MovementEnabled() { return movement; }
	static void DisableMovement() { movement = false; }
	static void EnableMovement() { movement = true; }
};

inline RawDelta Event::rd;
inline MouseCoords Event::mc;
inline bool Event::leftIsPressed;
inline bool Event::rightIsPressed;
inline std::bitset<Event::keys> Event::keystates;
inline bool Event::movement;