#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "PixelType.h"

class Translation
{
public:
	static size_t	LabCoding(const PelLab24 &lab1, const PelLab24 &lab2);
	static void		LabDecode(size_t code, PelLab24& lab1, PelLab24& lab2);

	static size_t	DimCoding(size_t width, size_t height);
	static void		DimDecode(size_t code, size_t& lab1, size_t& lab2);
private:
	Translation();
};

#endif //TRANSLATION_H
