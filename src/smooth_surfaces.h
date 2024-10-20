/* C/C++ standard libraries */
#include <iostream>
#include <thread>

/* Third-party libraries */
#include <igl/remove_duplicate_vertices.h>

#include <Eigen/Core>

namespace SmoothSurfaces
{
	void parallel_for(int start, int end, const std::function<void(int)>& func, int num_threads)
	{
		std::vector<std::thread> threads;
		int chunk_size = (end - start + num_threads - 1) / num_threads;

		for (int t = 0; t < num_threads; ++t)
		{
			threads.emplace_back([&, t]()
			{
				int chunk_start = start + t * chunk_size;
				int chunk_end = std::min(chunk_start + chunk_size, end);
				for (int i = chunk_start; i < chunk_end; ++i)
				{
					func(i);
				}
			});
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

	void evaluate(Eigen::MatrixXd& V, Eigen::MatrixXi& F, double lambda, int iterations, Eigen::MatrixXd& Vsmooth, Eigen::MatrixXi& Fsmooth)
	{
		if (lambda > 1 || lambda < 0)
		{
			lambda = 1;
		}

		// Remove duplicate Vertices
		Eigen::MatrixXd Vclean;
		Eigen::MatrixXi Fclean;
		Eigen::MatrixXi SVI;
		Eigen::MatrixXi SVJ;
		igl::remove_duplicate_vertices(V, F, 0.0, Vclean, SVI, SVJ, Fclean);

		auto loopSize = Vclean.rows();
		auto num_threads = std::thread::hardware_concurrency();
		std::vector<Eigen::Vector3d> smoothedVertices(Vclean.rows());

		for (int iter = 0; iter < iterations; iter++)
		{
			parallel_for(0, loopSize, [&] (int t)
			{
				Eigen::Vector3d vertex = Vclean.row(t);
				Eigen::Vector3d smoothedVertex = Eigen::Vector3d::Zero();
				int numAdjacentFaces = 0;

				for (int j = 0; j < Fclean.rows(); j++)
				{
					const Eigen::Vector3i& face = Fclean.row(j);
					if (face[0] == t || face[1] == t || face[2] == t)
					{
						Eigen::Vector3d v1 = Vclean.row(face[0]);
						Eigen::Vector3d v2 = Vclean.row(face[1]);
						Eigen::Vector3d v3 = Vclean.row(face[2]);

						smoothedVertex += (v1 + v2 + v3) / 3.0;
						numAdjacentFaces++;
					}
				}

				if (numAdjacentFaces > 0)
				{
					smoothedVertex /= static_cast<double>(numAdjacentFaces);
				}

				smoothedVertex = vertex + lambda * (smoothedVertex - vertex);
				smoothedVertices[t] = smoothedVertex;
			}, num_threads);

			for (int i = 0; i < Vclean.rows(); i++)
			{
				Vclean.row(i) = smoothedVertices[i];
			}

			std::cout << "Smooting iteration : " << iter << "\n";
		}

		Vsmooth = Vclean;
		Fsmooth = Fclean;
	}
}
