#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"
#include "Solid.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"

#include "LightOmni.h"
#include "timer.h"

Mat RenderFrame(void)
{
	// Camera resolution
	const Size resolution(800, 600);
	
	// Define a scene
	CScene scene;
	
	// Add camera to scene
	scene.add(std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 3.5f, -13), Vec3f(0, 0, 1), Vec3f(0, 1, 0), 60));

	// Eyelight shader
	auto pShader = std::make_shared<CShaderEyelight>(Vec3f::all(1));

	// Load scene description
#ifdef WIN32
	const std::string dataPath = "../data/";
#else
	const std::string dataPath = "../../data/";
#endif
	CSolid solid(pShader, dataPath + "Torus Knot.obj");
	scene.add(solid);

	// Build BSPTree
	scene.buildAccelStructure(20, 3);
	
	Vec3f pointLightIntensity(3, 3, 3);
	Vec3f lightPosition2(-3, 5, 4);
	Vec3f lightPosition3(0, 1, 4);
	
	scene.add(std::make_shared<CLightOmni>(pointLightIntensity, lightPosition2));
	scene.add(std::make_shared<CLightOmni>(pointLightIntensity, lightPosition3));

	Mat img(resolution, CV_32FC3);							// image array
	Ray ray;                                          		// primary ray

	parallel_for_(cv::Range(0, img.rows), [&](const Range& r) {
		for (int y = r.start; y < r.end; y++){
		 for (int x = 0; x < img.cols; x++) {
			scene.getActiveCamera()->InitRay(ray, x, y);	// initialize ray
			img.at<Vec3f>(y, x) = scene.RayTrace(ray);
		 }
	});
	img.convertTo(img, CV_8UC3, 255);
	return img;



int main(int argc, char* argv[])
{
	DirectGraphicalModels::Timer::start("Rendering frame... ");
	Mat img = RenderFrame();
	DirectGraphicalModels::Timer::stop();
	imshow("Image", img);
	waitKey();
	imwrite("D:/renders/torus knot.jpg", img);
	return 0;
}
