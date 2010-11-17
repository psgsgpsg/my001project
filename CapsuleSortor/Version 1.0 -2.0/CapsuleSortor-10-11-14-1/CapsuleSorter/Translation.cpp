#include "Translation.h"

size_t Translation::LabCoding(const PelLab24 &lab1, const PelLab24 &lab2)
{
	size_t color1 = (lab1.a & 0xFF) << 8 | (lab1.b & 0xFF);
	size_t color2 = (lab2.a & 0xFF) << 8 | (lab2.b & 0xFF);
	size_t allColor = (color1 << 16) | color2;
	return allColor;	
}

void Translation::LabDecode(size_t code, PelLab24& lab1, PelLab24& lab2)
{
	size_t highData = (code>>16) & 0xFFFF;		
	size_t lowData	= (code    ) & 0xFFFF;
	lab1.a	= static_cast<char>((highData >> 8)  & 0xFF);
	lab1.b	= static_cast<char>((highData     )  & 0xFF);
	lab2.a	= static_cast<char>((lowData  >> 8)  & 0xFF);
	lab2.b	= static_cast<char>((lowData      )  & 0xFF);
}

size_t Translation::DimCoding(size_t width, size_t height)
{
	return (((width & 0xFFFF)<<16) | (height & 0xFFFF));	
}

void Translation::DimDecode(size_t code, size_t& width, size_t& height)
{
	width	= (code>>16) & 0xFFFF;
	height	= (code    ) & 0xFFFF;	
}