#ifndef PIXELTYPE_H
#define PIXELTYPE_H

#ifndef	COLOR_ORDER_RGB
#ifndef COLOR_ORDER_BGR
#define	COLOR_ORDER_BGR
#endif
#endif

typedef unsigned char	PixelMem;
typedef unsigned char	PelGray8;

// struct of BGR, RGB;
typedef struct S_PelRGB24
{
union{

#ifdef	COLOR_ORDER_BGR
	struct
		{
			unsigned char B;
			unsigned char G;
			unsigned char R;
		};
#elif	COLOR_ORDER_RGB
	struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
		};
#endif
		unsigned char V[3];
	};
	S_PelRGB24(	const unsigned char r=0,
				const unsigned char g=0, 
				const unsigned char b=0)
	: R(r), G(g), B(b) { }
} PelRGB24;

typedef struct S_PelHSL24
{
	unsigned char H;
	unsigned char S;
	unsigned char L;
	S_PelHSL24(	const unsigned char h=0, 
				const unsigned char s=0,
				const unsigned char l=0)
	: H(h),	S(s), L(l)	{	}
} PelHSL24;


typedef struct S_PelLab24
{
	char L;
	char a;
	char b;
	S_PelLab24(	const char lv=0, 
				const char av=0,
				const char bv=0)
	: L(lv), a(av), b(bv)	{	}
} PelLab24;


#endif //PIXELTYPE_H
