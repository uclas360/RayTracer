# RayTracer - A C++ Ray Tracing Engine for 3D Graphics

<h1 align="center">
  <br>
  <a href="https://github.com/uclas360/RayTracer/releases"><img src="images/raytracer.png" alt="RayTracer" width="600"></a>
  <br>
  RayTracer
  <br>
</h1>

<h4 align="center">A RayTracer made in C++.</h4>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a>
</p>

![screenshot](images/demo1.png)

## Key Features

* **Real-time Rendering**: Render 3D scenes in real-time using ray tracing techniques.
* **Geometric Shapes**: Supports basic shapes like spheres and planes for scene construction.
* **3D Model Import**: Import 3D models using the Obj file format.
* **Lighting and Shading**: Implement basic lighting and shading models to enhance realism.
* **Camera Controls**: Navigate through the scene with intuitive camera controls.

## How To Use

### Clone this repository

To get started, clone the repository to your local machine:

```bash
git clone git@github.com:PouletHalal/RayTracer.git
```

### Go into the repository

Navigate to the cloned directory:

```bash
cd RayTracer
```

### Build the program

Create a build directory and compile the program:

```bash
mkdir build && cd build && cmake .. && cd ..
cmake --build build/ -j
```

### Run the program with graphical output

To run the program and view the graphical output, use the following command:

```bash
./raytracer scene.cfg -W 400 -H 400 -g
```

### Run the program without graphical output

For running the program without graphical output, simply omit the `-g` flag:

```bash
./raytracer scene.cfg -W 400 -H 400
```

## Contributing

Contributions are welcome! If you want to contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes.
4. Test your changes thoroughly.
5. Submit a pull request with a clear description of your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Releases

You can find the latest releases [here](https://github.com/uclas360/RayTracer/releases). Download the appropriate files and follow the instructions to run the program.

## Acknowledgments

* Special thanks to the open-source community for their contributions and support.
* Inspiration drawn from various ray tracing projects and tutorials.

## Resources

For more information on ray tracing, consider checking the following resources:

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Real-Time Rendering](https://www.realtimerendering.com/)

## Contact

For any inquiries or feedback, feel free to reach out:

- **Email**: your-email@example.com
- **GitHub**: [YourGitHubProfile](https://github.com/YourGitHubProfile)

## Final Note

This project is a work in progress. Future updates will include more features, optimizations, and enhancements. Stay tuned for improvements and thank you for your interest in RayTracer!