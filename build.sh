cmake --preset=debug
cmake --build --preset=debug-build
ln -sf build/debug/compile_commands.json compile_commands.json

cmake --preset=release
cmake --build --preset=release-build

cp -rf data build/debug
cp -rf data build/release
