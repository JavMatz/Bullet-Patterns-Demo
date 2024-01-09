CC=clang++
CFLAGS=-Wall -std=c++11 -ISFML-2.6.1/include
LDFLAGS=-lm -LSFML-2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system # Link against the math library, adjust as needed

SRC=main.cpp
TARGET=BulletPatternDemo

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run:
	export LD_LIBRARY_PATH=SFML-2.6.1/lib && ./$(TARGET)
