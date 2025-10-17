HEADERS = -Iinclude -Iinclude/Render -Iinclude/Scene -IObjectPool

FILES = src/*.cpp

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SFML_SPECIFIC = -Iexamples/sfml

EXEC = cge

sfml:
	g++ ${HEADERS} ${FILES} ${SFML_SPECIFIC} ${LIBS} -o ${EXEC}

debug:
	g++ -fsanitize=address ${HEADERS} ${FILES} ${SFML_SPECIFIC} ${LIBS} -g -o ${EXEC}
