main:
	@make --no-print-directory build
	mv ./build/compile_commands.json .

build: FORCE
	@echo "-- USING QT_OUTPUT_DIR: $(QT_OUTPUT_DIR)"
	cmake -DCMAKE_BUILD_TYPE=Release \
		-S . -B build
	cmake --build build # alternatively: cd build && make

inspect:
	otool -L ./build/CanvasSync.app/Contents/MacOS/CanvasSync

open:
	open ./build/CanvasSync.app

get-qt:
	mkdir -p $(QT_SRC_DIR)
	@if [ ! -f $(QT_SRC_DIR)/configure ]; then \
		git clone https://code.qt.io/qt/qt5.git -b v$(QT_VERSION) $(QT_SRC_DIR); \
		cd $(QT_SRC_DIR) && ./init-repository -f --module-subset=qtbase; \
	fi

config-qt:
	cd $(QT_SRC_DIR) && ./configure $(QT_CONFIG_FLAGS) -prefix $(QT_STATIC_DIR)
	cd $(QT_SRC_DIR)/qtbase && ./configure $(QTBASE_CONFIG_FLAGS) \
		-nomake examples \
		-nomake tests

clean-qt:
	cd $(QT_SRC_DIR) && ninja clean

build-qt:
	cd $(QT_SRC_DIR) && ninja -j6 && ninja install

FORCE: ;
