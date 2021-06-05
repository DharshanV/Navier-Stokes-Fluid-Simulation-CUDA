#pragma once
typedef unsigned char uchar;

struct Color {
	Color(uchar r = 255,uchar g = 255, uchar b = 255,uchar a = 255) : r(r), g(g), b(b), a(a) {
		
	}
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

