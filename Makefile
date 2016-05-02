all : segment stereo render

segment: CImg.h segment.cpp
	g++ -Dcimg_display=0 segment.cpp -o segment -I. -O3 -lpthread

stereo: CImg.h stereo.cpp
	g++ -Dcimg_display=0 stereo.cpp -o stereo -I. -O3 -lpthread

render: CImg.h render.cpp
	g++ -Dcimg_display=0 render.cpp -o render -I. -O3 -lpthread

clean:
	rm segment stereo render
