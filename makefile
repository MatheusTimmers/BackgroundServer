SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

all: $(BIN_DIR)/run

$(BIN_DIR)/run: $(BUILD_DIR)/file.o $(BUILD_DIR)/bs.o $(SRC_DIR)/main.cpp
	@mkdir -p $(BIN_DIR)
	g++ $(SRC_DIR)/main.cpp $(BUILD_DIR)/file.o $(BUILD_DIR)/bs.o -o $(BIN_DIR)/run

$(BUILD_DIR)/file.o: $(SRC_DIR)/file_reader/FileReader.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $(SRC_DIR)/file_reader/FileReader.cpp -o $(BUILD_DIR)/file.o

$(BUILD_DIR)/bs.o: $(SRC_DIR)/background_server/BackgroundServer.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $(SRC_DIR)/background_server/BackgroundServer.cpp -o $(BUILD_DIR)/bs.o

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

