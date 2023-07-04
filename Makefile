CC = g++
INCLUDES = -I./ 
LDFLAGS = -L./
LIBS = -lglut -lGL -lGLU -lX11 -lXrandr -lXi -lXinerama -lXcursor -lpthread -ldl -lm
SRC = $(wildcard *.cpp) 
OUT = program

$(OUT): $(SRC)
	$(CC) $(INCLUDES) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(OUT)
