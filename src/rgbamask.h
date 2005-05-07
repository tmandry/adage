// Different values depending on endianness

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const int rmask = 0xff000000;
	const int gmask = 0x00ff0000;
	const int bmask = 0x0000ff00;
	const int amask = 0x000000ff;
#else
	const int rmask = 0x000000ff;
	const int gmask = 0x0000ff00;
	const int bmask = 0x00ff0000;
	const int amask = 0xff000000;
#endif
