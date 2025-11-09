#include "pxrt.hpp"

pxrt::pxrt() {}
//pxrt::pxrt(std::vector<Canvas> &nCanvas) :  nCanvas(nCanvas) {}

pxrt::pxrtFormat pxrt::readpxrt(const char* file) {
	std::ifstream is;
	is.open(file, std::ios::binary);

	pxrtFormat pxrtFile;

	is.read(reinterpret_cast<char*>(&pxrtFile.fileSignature), sizeof(pxrtFile.fileSignature));
	const char* filesig = "PXRT";
	if (std::strncmp(pxrtFile.fileSignature, filesig, sizeof(pxrtFile.fileSignature)) != 0) {
		puts("not pxrt");
		pxrtFile.frames = 0;
		return pxrtFile;

	}

	is.read(reinterpret_cast<char*>(&pxrtFile.frames), sizeof(pxrtFile.frames));
	std::cout << "file signature: " << pxrtFile.fileSignature << std::endl;
	std::cout << "frames: " <<(pxrtFile.frames) << std::endl;

	for (size_t i = 0; i < pxrtFile.frames; i++)
	{
		canvas c;


		c.width;
		c.height;
		c.frameTime;
		is.read(reinterpret_cast<char*>(&c.width), sizeof(c.width));
		is.read(reinterpret_cast<char*>(&c.height), sizeof(c.height));
		is.read(reinterpret_cast<char*>(&c.frameTime), sizeof(c.frameTime));
		//char *pixels = new char[width*height*4];

		c.pixels.resize(c.width * c.height * 4);

		is.read(reinterpret_cast<char*>(c.pixels.data()), (c.pixels.size()));

		std::cout << "width: " << c.width << std::endl;
		std::cout << "height: " << c.height << std::endl;
		std::cout << "frametime: " << c.frameTime << std::endl;
		std::cout << "pixel data: " << std::endl;

		/*for (size_t i = 0; i < c.pixels.size(); i++)
		{
			std::cout << std::hex << std::int16_t{ (c.pixels[i]) } << " ";
		} */
		std::cout << " pixel data end" << std::endl;
		
		pxrtFile.canvasData.push_back(c);

	}

	is.close();
	return pxrtFile;






}


int pxrt::writepxrt(const char* file) {
	std::ofstream os;
	os.open(file, std::ios::binary);

	pxrtFormat pxrtFile;

	strncpy_s(pxrtFile.fileSignature, "PXRT", sizeof(pxrtFile.fileSignature));
	pxrtFile.frames = currentState.nCanvas->size();
	os.write(reinterpret_cast<char*>(pxrtFile.fileSignature), sizeof(pxrtFile.fileSignature));
	//os.write(reinterpret_cast<char*>(pxrtFile.fileSignature), sizeof(pxrtFile.fileSignature));
	os.write(reinterpret_cast<char*>(&pxrtFile.frames), sizeof(uint64_t));





	for (Canvas c : *currentState.nCanvas) {


		unsigned int width = c.getCanvasSize().x;
		unsigned int height = c.getCanvasSize().y;
		unsigned int frameTime = c.frameTime;

		os.write(reinterpret_cast<char*>(&width), sizeof(unsigned int));
		os.write(reinterpret_cast<char*>(&height), sizeof(unsigned int));
		os.write(reinterpret_cast<char*>(&frameTime), sizeof(unsigned int));

		const uint8_t* pixelptr = c.getCanvasPixelPtr();


		os.write(reinterpret_cast<const char*>(pixelptr), (width*height*4));


		
	}


	//pxrtFile.canvasObjects  = *pxrtFile.canvasObjects.data();

	os.write(("PXRTEND"), 8);
	os.close();
	return 0;
}
