all:
	g++ main.cpp -o my_game \
    -I/usr/local/include \
    -L/usr/local/lib64 \
    -DSFML_STATIC \
    -lsfml-graphics-s -lsfml-window-s -lsfml-system-s \
    -lX11 -lXrandr -lXcursor -lXi -lGL -lpthread -lrt -ludev -lfreetype

	g++ backend/backend.cpp -o backend/backend 
	./my_game

clean:
	rm backend/run backend/render backend/data.txt mouseOutput.txt