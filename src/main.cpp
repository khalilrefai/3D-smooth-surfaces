/* Third-party libraries */
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>

/* Local inclusions */
#include "smooth_surfaces.h"

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		std::cerr << "Usage: " << argv[0] << " <input_filepath> <lambda (between 0 and 1)> <iterations number> <output_filepath> <visualization (0 or 1)>" << std::endl;
		return 1;
	}

	// Import mesh
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	if (!igl::read_triangle_mesh(argv[1], V, F))
	{
		std::cerr << "Error: Unable to import mesh from file " << argv[1] << "\n";
		return 1;
	}

	// Smooth surfaces
	auto lambda = std::stod(argv[2]); // value between 0 and 1
	auto iterations = std::stoi(argv[3]);
	Eigen::MatrixXd Vsmooth;
	Eigen::MatrixXi Fsmooth;
	SmoothSurfaces::evaluate(V, F, lambda, iterations, Vsmooth, Fsmooth);

    // Export result
	if (!igl::write_triangle_mesh(argv[4], Vsmooth, Fsmooth))
	{
		std::cerr << "Error: Unable to export mesh to file " << argv[4] << "\n";
		return 1;
	}

    // Visualization
    if (std::stoi(argv[5]))
    {
        igl::opengl::glfw::Viewer viewer;
        viewer.data().set_mesh(Vsmooth, Fsmooth);
        viewer.launch();
    }

	return 0;
}
