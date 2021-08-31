#pragma once
#include <bitset>

struct RawDelta
{
	int x, y;

	RawDelta() = default;
};

class Event
{
private:
	//MOUSE
	static RawDelta rd;

	static bool leftIsPressed;

	//KEYBOARD
	static constexpr unsigned int keys = 256;
	static std::bitset<keys> keystates;

	//STATE
	static bool movement;
public:
	//MOUSE
	static void OnRawDelta(int dx, int dy) { rd.x = dx; rd.y = dy; }
	static void ClearRawDelta() { rd.x = 0; rd.y = 0; }
	static RawDelta ReadRawData() { return rd; }

	static bool LeftIsClicked() { return leftIsPressed; }
	static void OnLeftClick() { leftIsPressed = true; }
	static void OnLeftRelease() { leftIsPressed = false; }

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
inline bool Event::leftIsPressed;
inline std::bitset<Event::keys> Event::keystates;
inline bool Event::movement;