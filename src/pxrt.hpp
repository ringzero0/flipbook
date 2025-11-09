#pragma once
#include <iostream>
#include <fstream>
#include "Canvas.hpp"
#include "States.hpp"
class pxrt
{
public:

	struct canvas {
		unsigned int width;
		unsigned int height;
		unsigned int frameTime;
		std::vector<uint8_t> pixels;

	};

	struct pxrtFormat
	{
		char fileSignature[5];
		uint64_t	frames;
		std::vector<canvas> canvasData;
	};

	//std::vector<Canvas> nCanvas;
	pxrt();
	//pxrt(std::vector<Canvas>& nCanvas);


	pxrtFormat readpxrt(const char* file);

	int writepxrt(const char* file);




};

