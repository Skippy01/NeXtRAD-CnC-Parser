CC		= g++
CFLAGS		= -g -c -Wall 
LDFLAGS		= 
SOURCES		= cnc.cpp tcpsocket/tcpstream.cpp tcpsocket/tcpconnector.cpp
INCLUDES	= -I.
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= cnc

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

