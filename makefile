SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

run: build
	@./$(BIN_DIR)/run

build: $(BIN_DIR)/run

$(BIN_DIR)/run: $(BUILD_DIR)/bs.o $(BUILD_DIR)/task.o $(SRC_DIR)/main.cpp
	@mkdir -p $(BIN_DIR)
	@g++ $(SRC_DIR)/main.cpp $(BUILD_DIR)/task.o $(BUILD_DIR)/bs.o -o $(BIN_DIR)/run

$(BUILD_DIR)/task.o: $(SRC_DIR)/task/Task.cpp
	@mkdir -p $(BUILD_DIR)
	@g++ -c $(SRC_DIR)/task/Task.cpp -o $(BUILD_DIR)/task.o

$(BUILD_DIR)/bs.o: $(SRC_DIR)/background_server/BackgroundServer.cpp
	@mkdir -p $(BUILD_DIR)
	@g++ -c $(SRC_DIR)/background_server/BackgroundServer.cpp -o $(BUILD_DIR)/bs.o

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)