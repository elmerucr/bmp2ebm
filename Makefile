picture2asm: main.cpp stb_image.h
	g++ -o picture2asm main.cpp
	strip picture2asm
	cp picture2asm /home/elmer/Applications/bin
	
clean:
	rm picture2asm
