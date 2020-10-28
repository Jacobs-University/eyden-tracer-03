# Practical Assignment 3
**Dealine**: 29.10.2020

Please put your name here:  
**Name:** Katrin von Seggern
## Problem 1
### Rendering complex geometry (Points 5)
Until now we have only hardcoded our scene geometry in main.cpp. This is of course not practical. In the new framework, a class ```CSolid``` is added. This class may contain complex geometry formed by multiple primitives. Such geometry may be saved / read from an .obj file. For this problem we will read _torus knot.obj_ file and rended this object, which consists of 12 960 triangles. To make the method work proceed as follows:
1. Fork the current repository
2. Modify the README.md file in your fork and put your name above.
3. Have a look at the file _torus knot.obj_ and at the class ```CSolid```. Study how triangles are stored in the obj-format and in the class. The _v_ ’s indicate a single 3d-vertex position, and the _f_ ’s (faces) are indecies to 3 vertex numbers a triangle consits of (please note that the face indecies are starting with **1 and not 0**).
4. Implement function ```CScene::add(const CSolid& solid)``` which adds a solid to the scene.
5. Make sure that you work with Release and not Debug and disable BSP support in CMake (if it was enabled). Render the scene and write the time needed for 1 frame below:<br>
**T0:** 53526 ms

> **Note:** Rendering may take several minutes.

If your implementation works as expected you should see an image of a torus knot like this:

<img src="./doc/torus knot.jpg" alt="Torus Knot" width="400px">

**Hint:** The obj file-format can be dumped out from various 3d-modelers. Nevertheless, the output might differ from modeler to modeler and there are also other tokens like _vn_ for vertex normals or _vt_ for texture coordinates. Check
[obj file format](https://www.cs.cmu.edu/~mbz/personal/graphics/obj.html) for a full description. 

## Problem 2
### Multi-threading (Points 15)
As we have learned from Problem 1, the increased number of the primitives in scene makes rendering with raytracing inefficient and thus useless. So, the rest of the current assignment will be dedicated to acceleration techniques. One of the most naive ideas to accelerate raytracing would be to increase the CPU power. 
In order to use not one but all cores of CPU proceed as follows:
1. Study the OpenCV function for parallel data processing ```parallel_for_```: [How to use the OpenCV parallel_for_ to parallelize your code](https://docs.opencv.org/master/d7/dff/tutorial_how_to_use_OpenCV_parallel_for_.html)
2. In main.cpp file rewrite the main rendering loop (lines 53 - 57), by paralellizing the loop ```for (int y = 0; y < img.rows; y++)``` with help of ```parallel_for_``` function and enclosing the inner body into a lambda-expression. You do not need to parallelize the inner loop ```for (int x = 0; x < img.cols; x++)```.
3. Render the scene and write the time needed for 1 frame T1 and speedup = T0 / T1 below:<br>
**T1:** 7409 ms<br>
**Speedup:** 53526 ms/7409 ms= 7.2244 times speed up

## Problem 3
### Implementation of a kd-tree acceleration structure (Points 30)
So far, your own ray tracer implementation has used no acceleration structure for reducing the number of ray / primitive intersections. This was simple to implement and worked relatively good. Unfortunately, this, of course, is not practical for larger scenes as you have noticed in the last problems with the torus knot. As such, you need a data structure to speed up the process of finding the first hit of a ray with the primitives. In recent years the kd-tree proved to be a useful acceleration data structure for minimizing ray-intersection tests. To implement your kd-tree proceed as follows:
1. Study n new class ```CBoundingBox``` is now in the framwork which contains two ```Vec3f```’s for the ```m_minPoint, m_maxPoint``` - fields of the bounding box. Furthermore the class has 2 methods ```void CBoundingBox::extend(const Vec3f& p)``` and ```void extend(const CBoundingBox& box)```. Enable BSP support in CMake and implement the following functionality: 
    1. If point _p_ is not inside a bounding box _b_, ```b.bxtend(p)``` should extend the bounding box until it also includes _p_. 
    2. If box _box_ is not fully inside a bounding box _b_, ```b.bxtend(box)``` should extend the bounding box until it also includes _box_. <br>
    **Tip:** The box is initialized with an ’empty box’ (_m_minPoint = +infinity, m_maxPoint = −infinity_).<br>
    **Hint:** You may make use of functons _Min3f_ and _Max3f_ defined in the BoundingBox.cpp file.
2. The method ```virtual CBoundingBox getBoundingBox(void) const = 0``` has to be implemented in every class derived from ```IPrim```.
3. Most acceleration structures require to clip the ray with the bounding box of the scene, as the origin might otherwise be outside the scene bounds. For clipping a ray with the bounding box of a scene, you can best use the slabs algorithm and implement it in ```void CBoundingBox::clip(const Ray& ray, double& t0, double& t1)```.
4. You will need a method to decide whether a primitive is contained in a given bounding box or not. For this purpose the method ```bool CBoundingBox::overlaps(const CBoundingBox& box) const``` exists. Implement theis method. For simplicity, just check the primitives bounding box for overlap with the box.
5. Implement the method ```CBoundingBox calcBoundingBox(const std::vector<ptr_prim_t>& vpPrims)``` in BSPTree.h file, which should calculate the bounding box of the scene, containing all the primitives given by _vpPrims_.<br>
**Hint:** Use the ```void extend(const CBoundingBox& box)``` function.<br>
If everything so far was implemented correctly, the Scene bounds will be: [-6, 0, -5.6] [4.45, 8, 5.6] (approximately)
6. Implement the method ```std::pair<CBoundingBox, CBoundingBox> CBoundingBox::split(int dim, float val) const``` which splits curent bounding box into two pieces. The split should be done with a hyper-plane orthogonal to the axis defined by argument _dim_ and in the positon defined by argument _val_.
7. Implement the method ```std::shared_ptr<CBSPNode> build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)``` of the class ```CBSPTree```. Use the ideas presented at the lecture. As soon as you have reached a maximum depth (_e.g._ 20), or you have less then a minimum number of primitives (_e.g._ 3 or 4), stop subdividing and generate a leaf node. Otherweise, split your bounding box in the middle (in the maximum dimension), sort your current primitives into two vector left and right, and recursively call BuildTree with the respective bounding boxes and vector for left and right. Start subdivision with a list of all primitives, the total scene bounds, and an initial recursion depth of 0.<br>
8. For traversal, use a simple, recursive algorithm, described in the lectures. Please implement this algorithm in methods ```bool CBSPTree::intersect(Ray& ray)``` and ```bool CBSPNode::intersect(Ray& ray, double t0, double t1)```
For more information please read the chapter 7.2 in the [thesis of Dr. Ingo Wald](http://www.sci.utah.edu/~wald/PhD/wald_phd.pdf).
6. Implement the method ```std::shared_ptr<CBSPNode> build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)``` of the class ```CBSPTree```. Use the ideas presented at the lecture. As soon as you have reached a maximum depth (_e.g._ 20), or you have less then a minimum number of primitives (_e.g._ 3 or 4), stop subdividing and generate a leaf node. Otherweise, split your bounding box in the middle (in the maximum dimension), sort your current primitives into two vector left and right, and recursively call BuildTree with the respective bounding boxes and vector for left and right. Start subdivision with a list of all primitives, the total scene bounds, and an initial recursion depth of 0.<br>
9. Render the scene and write the time needed for 1 frame T2 and speedup = T0 / T2 below:<br>
**T2:** .......<br>
**Speedup:** .......

> A the solution for this problem can be found in OpenRT library: www.openrt.org However it is highly recommended to solve this problem using lecture slides only and resorting to the solution only as a last resort. 

Instead of optimizing too much, rather concentrate on a stable, bug-free implementation.

## Problem 4
### Optimization of the KD-Tree build algorithm (Points 50)
The performace of ray traversal in a kd-tree highly depends on the structure of a particular tree. For the same scene it is possible to build a kd-tree in multiple ways. For this problem we will research the tree building algorithm and try to ptimize it.
The core idea of the kd-tree building algorithm is splitting a current bounding box into 2 leafs. For that purpose on the lectures we have considered splitting dimension _splitDim_ to be the maximum dimension of the bounding box and splitting value _splitVal_ to be the center of of the box in that dimension.
1. Elaborate and implement your idea in ```ptr_bspnode_t CBSPTree::build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)``` for better choice of the _splitVal_ parameter. For example, define _splitVal_ in such a way, that the number of primitives in the current bounding box will be split by half. To check your implememntation you may chech the length of the left and right vectors with primitives after the split.
2. Elaborate and implement your idea in ```ptr_bspnode_t CBSPTree::build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)``` for better choice of the _splitDim_ parameter. For example, define _splitVal_ as a function of max dimension _and_ depth of the recursion.
3. Experiment with your ideas. Chose the one, which gives the fastest result and render the scene and write the time needed for 1 frame T3 and speedup = T0 / T3 below:<br>
**T3:** .......<br>
**Speedup:** .......

## Submission
Please submit the assignment by making a pull request.
**Important** : Please make sure that
- No _extra files_ are submitted (except those, which were mentioned in the assignment)
- The changes were made _only_ in those files where you were asked to write your code
- The Continiouse Integration system (appVeyor) can build the submitted code
- The rendered images are also submitted in the folder "renders" 
