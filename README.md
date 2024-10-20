# Smoothing surfaces of 3D mesh
![screenshot](https://github.com/khalilrefai/3D-smooth-surfaces/blob/main/screenshot.PNG?raw=true)

## Overview

This C++ algorithm smooths surfaces of a 3D mesh.

## Build and Run Instructions

Follow these steps to build and run the C++ implementation:

1. Create a build directory:

    ```bash
    mkdir build
    ```

2. Change into the build directory:

    ```bash
    cd build
    ```

3. Run CMake to configure the build:

    ```bash
    cmake ..
    ```

4. Build the project:

    ```bash
    cmake --build . --config Release
    ```

5. Execute the algorithm:

    ```bash
    ./smooth-surfaces <input_filepath> <lambda (between 0 and 1)> <iterations number> <output_filepath> <visualization (0 or 1)>
    ```

Replace `<input_filepath>`, `<lambda (between 0 and 1)>`, `<iterations number>`, `<output_filepath>` and `<visualization (0 or 1)>` with the actual file paths and values.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, feel free to submit pull requests or open issues.

## License

This project is licensed under the [MIT License](LICENSE).
