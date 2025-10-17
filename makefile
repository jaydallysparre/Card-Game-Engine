HEADERS = -Iinclude -Iinclude/Render -Iinclude/Scene -Iinclude/ObjectPool

FILES = src/*.cpp

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SFML_SPECIFIC = -Iexamples/sfml

ADDER_SPECIFIC = -Iexamples/BinaryAdder examples/BinaryAdder/*.cpp

EXEC = cge

sfml:
	g++ ${HEADERS} ${FILES} ${SFML_SPECIFIC} ${LIBS} main_sfml.cpp -o ${EXEC}

adder:
	g++ ${HEADERS} ${FILES} ${SFML_SPECIFIC} ${ADDER_SPECIFIC} ${LIBS} main_adder.cpp -o AdderGame

debug:
	g++ -fsanitize=address ${HEADERS} ${FILES} ${SFML_SPECIFIC} ${LIBS} -g -o ${EXEC}
