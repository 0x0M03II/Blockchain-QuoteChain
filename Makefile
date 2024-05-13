# Makefile

CXX = g++
CXXFLAGS = -g -O2 -std=c++11 -pthread -march=native -Wno-deprecated-declarations
LDFLAGS = -L/usr/lib -lntl -lgmp -lm -lssl -lcrypto
TARGET = quotechain

# Add the new cpp files here
SOURCES = main.cpp quotechain.cpp

OBJECTS = $(SOURCES:.cpp=.o)

OS := $(shell uname)

ifeq ($(OS),Linux)
    LDFLAGS = -L/usr/local/lib/ -lm -lssl -lcrypto
endif
ifeq ($(OS),Darwin)
    LDFLAGS = -L/usr/local/lib/ -lm -lssl -lcrypto
endif

all: check_deps $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

check_deps:
	@echo "Checking dependencies..."
	@case `uname` in \
		Linux*) ldconfig -p | grep libssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL." && exit 1) ;; \
		Darwin*)ls -l /usr/local/lib/ | egrep -i openssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL using Homebrew." && exit 1) ;; \
		*) echo "Unsupported operating system." && exit 1 ;; \
	esac
	@echo "All dependencies are installed."
