PROJECT_NAME=	my_autocomplete
BIN_OUT=		$(PROJECT_NAME)
CC=				gcc
CFLAGS= 		-W -Wall
BUILD_DIR=		build

WASM_FUNC=		EXPORTED_FUNCTIONS='["_api_predict_init_dictionary","_api_predict_has_word","_api_predict_word", "_api_predict_get_words"]'
WASM_FEATURES=	EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
WASM_PRELOAD=	--preload-file ./dictionary.mydb

WASM_OUT=		autocomplete.js

SRC=			src/util.c \
				src/tree.c \
				src/predict_file.c \
				src/dictionary.c \
				src/autocomplete.c \
				src/wasm_interface.c

SRC_TEST=		$(SRC) src/main.c

OBJ=			$(SRC:.c=.o)

OBJ_TEST=		$(SRC_TEST:.c=.o)

$(PROJECT_NAME): all

all:	$(OBJ_TEST)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(BIN_OUT) $(OBJ_TEST)
	cp dictionary.mydb $(BUILD_DIR)

clean:
	rm -fv $(OBJ_TEST)

fclean:		clean
	rm -fv $(BUILD_DIR)/$(BIN_OUT)

re:	fclean all

wasm:
	echo $(shell pwd)
	docker run --rm  -v $(shell pwd):/src trzeci/emscripten emcc $(SRC) -o $(WASM_OUT) $(WASM_PRELOAD) -s $(WASM_FUNC) -s $(WASM_FEATURES)