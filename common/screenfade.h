#if !defined( SCREENFADEH )
#define SCREENFADEH
#ifdef _WIN32
#pragma once
#endif

typedef struct screenfade_s
{
	float		fadeSpeed;		// How fast to fade (tics / second) (+ fade in, - fade out)
	float		fadeEnd;		// When the fading hits maximum
	float		fadeTotalEnd;	// Total End Time of the fade (used for FFADE_OUT)
	float		fadeReset;		// When to reset to not fading (for fadeout and hold)
	BYTE		fader, fadeg, fadeb, fadealpha;
	int			fadeFlags;		// Fading flags
} screenfade_t;

#endif // !SCREENFADEH