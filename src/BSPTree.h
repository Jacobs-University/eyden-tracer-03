#pragma once

#include "BSPNode.h"
#include "BoundingBox.h"
#include "IPrim.h"
#include "ray.h"

namespace {
	// Calculates and return the bounding box, containing the whole scene
	CBoundingBox calcBoundingBox(const std::vector<ptr_prim_t>& vpPrims)
	{
		CBoundingBox res;
		// --- PUT YOUR CODE HERE ---
		/*	we traverse through all primitives and
		*	extend the box to encompass all of
		*	their boxes.
		*/
		for (auto pPrim : vpPrims) {
			res.extend(pPrim->getBoundingBox());
		}
		return res;
	}

	// Returns the best dimension index for next split
	int MaxDim(const Vec3f& v)
	{
		return (v.val[0] > v.val[1]) ? ((v.val[0] > v.val[2]) ? 0 : 2) : ((v.val[1] > v.val[2]) ? 1 : 2);
	}
}

// ================================ BSP Tree Class ================================
/**
 * @brief Binary Space Partitioning (BSP) tree class
 */
class CBSPTree
{
public:
	CBSPTree(void) = default;
	CBSPTree(const CBSPTree&) = delete;
	~CBSPTree(void) = default;
	const CBSPTree& operator=(const CBSPTree&) = delete;
	
	/**
	 * @brief Builds the BSP tree for the primitives provided via \b vpPrims
	 * @param vpPrims The vector of pointers to the primitives in the scene
	 * @param maxDepth The maximum allowed depth of the tree.
	 * Increasing the depth of the tree may speed-up rendering, but increse the memory consumption.
	 * @param minPrimitives The minimum number of primitives in a leaf-node.
	 * This parameters should be alway above 1.
	 */
	void build(const std::vector<ptr_prim_t>& vpPrims, size_t maxDepth = 20, size_t minPrimitives = 3) {
		m_treeBoundingBox = calcBoundingBox(vpPrims);
		m_maxDepth = maxDepth;
		m_minPrimitives = minPrimitives;
		std::cout << "Scene bounds are : " << m_treeBoundingBox << std::endl;
		m_root = build(m_treeBoundingBox, vpPrims, 0);
	}
	/**
	 * @brief Checks whether the ray \b ray intersects a primitive.
	 * @details If ray \b ray intersects a primitive, the \b ray.t value will be updated
	 * @param[in,out] ray The ray
	 */
	bool intersect(Ray& ray) const
	{
		// --- PUT YOUR CODE HERE --
		double t0 = 0, t1 = ray.t;
		m_treeBoundingBox.clip(ray, t0, t1);
		//intersection check
		if(t0>t1) return false;

		return m_root->intersect(ray, t0, t1);
	}


private:
	/**
	 * @brief Builds the BSP tree
	 * @details This function builds the BSP tree recursively
	 * @param box The bounding box containing all the scene primitives
	 * @param vpPrims The vector of pointers to the primitives included in the bounding box \b box
	 * @param depth The distance from the root node of the tree
	 */
	 
	/*ptr_bspnode_t build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)
	{
		// Check for stoppong criteria
		if (depth >= m_maxDepth || vpPrims.size() <= m_minPrimitives)
			return std::make_shared<CBSPNode>(vpPrims);                                     // => Create a leaf node and break recursion

		// else -> prepare for creating a branch node
		// First split the bounding volume into two halfes
		int     splitDim = MaxDim(box.getMaxPoint() - box.getMinPoint());                   // Calculate split dimension as the dimension where the aabb is the widest
		float   splitVal = (box.getMinPoint()[splitDim] + box.getMaxPoint()[splitDim]) / 2; // Split the aabb exactly in two halfes
		auto    splitBoxes = box.split(splitDim, splitVal);
		CBoundingBox& lBox = splitBoxes.first;
		CBoundingBox& rBox = splitBoxes.second;

		// Second order the primitives into new nounding boxes
		std::vector<ptr_prim_t> lPrim;
		std::vector<ptr_prim_t> rPrim;
		for (auto pPrim : vpPrims) {
			if (pPrim->getBoundingBox().overlaps(lBox))
				lPrim.push_back(pPrim);
			if (pPrim->getBoundingBox().overlaps(rBox))
				rPrim.push_back(pPrim);
		}

		// Next build recursively 2 subtrees for both halfes
		auto pLeft = build(lBox, lPrim, depth + 1);
		auto pRight = build(rBox, rPrim, depth + 1);

		return std::make_shared<CBSPNode>(splitDim, splitVal, pLeft, pRight);
	}*/
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//new build function problem 4.1
	
	ptr_bspnode_t build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)
	{
		// Check for stoppong criteria
		if (depth >= m_maxDepth || vpPrims.size() <= m_minPrimitives)
			return std::make_shared<CBSPNode>(vpPrims);                                     // => Create a leaf node and break recursion

		// else -> prepare for creating a branch node
		// First split the bounding volume into two halfes
		int	splitDim = MaxDim(box.getMaxPoint() - box.getMinPoint());                   // Calculate split dimension as the dimension where the aabb is the widest
		//now check how many are left and right to balance the tree
		//
		//	the split values are going to be balanced, based 
		//	on the number of primitives in the scene.
		//
		//	we need to check how many prims we have (n),
		//	we sort them based on the distance to the
		//	minpoint of the main box.
		//	then we split at the middle n/2
		//	this split in the middle works because 
		//	they are alredy sorted.
		//
		//	make an array of distances; from the centre of 
		//	the prim's Bounding Box, to the minPoint of the main box
		
		std::vector<double> primDistances;
		//loop through prims to get their distances and save them into the vector
		for (auto pPrim : vpPrims)
		{
			//primpoint is the centre of the prim's own bounding box
			Vec3f primpoint = ((pPrim->getBoundingBox().getMaxPoint() -
				pPrim->getBoundingBox().getMinPoint()) / 2);
			double distance = primpoint[splitDim] - box.getMinPoint()[splitDim];
			primDistances.push_back(distance);
		}
		//sort the list
		sort(primDistances.begin(), primDistances.end());
		//we take the middle to balance Left&Right Prims (already sorted)
		int n = floor(primDistances.size() / 2);
		float splitVal = primDistances.at(n);

		auto    splitBoxes = box.split(splitDim, splitVal);
		CBoundingBox& lBox = splitBoxes.first;
		CBoundingBox& rBox = splitBoxes.second;

		// Second order the primitives into new bounding boxes
		std::vector<ptr_prim_t> lPrim;
		std::vector<ptr_prim_t> rPrim;
		for (auto pPrim : vpPrims) {
			if (pPrim->getBoundingBox().overlaps(lBox))
				lPrim.push_back(pPrim);
			if (pPrim->getBoundingBox().overlaps(rBox))
				rPrim.push_back(pPrim);
		}

		// Next build recursively 2 subtrees for both halfes
		auto pLeft = build(lBox, lPrim, depth + 1);
		auto pRight = build(rBox, rPrim, depth + 1);

		return std::make_shared<CBSPNode>(splitDim, splitVal, pLeft, pRight);
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//new build function problem 4.2
	/*
	ptr_bspnode_t build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)
	{//
	 //	we build the tree by going through all split dimensions,
	 //	and comparing the probabilities of a ray hitting a primitive
	 //	in the new boxes between the dimensions and then choose the best.
	 // 
	 //	the left and right child boxes will have the same number of
	 //	primitives, since we create a balanced tree.
	 //	this means that a smaller sub-box area will increase the
	 //	probability for a ray to hit a prim in one of the two boxes.
	 //	since the algorithm is recursive, this will increase the
	 //	probability over each iteration for a ray hitting primitives.
	 // 
	 //	this algorithm helps increase the ray-primitive intersection
	 //	probability, it does not focus on speed of building the tree.
	 // 
	 //	the idea for this algorithm was inspired by the
	 //	Surface Area Heuristic based on [Goldsmith and Salmon 1987] 
	 //	and [MacDonald and Booth [1989,1990]]. 
	 //	found in chapter 2.5 in the Dissertation by Dipl.-Inform. WODNIOK:
	 //	https://tuprints.ulb.tu-darmstadt.de/8820/1/Dissertation_Dominik_Wodniok_2019.pdf
	 //
		// Check for stoppong criteria
		if (depth >= m_maxDepth || vpPrims.size() <= m_minPrimitives)
			return std::make_shared<CBSPNode>(vpPrims);                                     // => Create a leaf node and break recursion

		// else -> prepare for creating a branch node
		
		//	make an array of distances; from the centre of 
		//	the prim's Bounding Box, to the minPoint of the main box
		//	(for each dimension)
		
		std::vector<double> primDistances;
		std::vector<double> primDistances1;
		std::vector<double> primDistances2;

		//loop through prims to get their distances and save them into the vectors
		for(int i=0; i<3; i++)//for x,y,z dimensions at i
			for (auto pPrim : vpPrims)
			{
				//primpoint is the centre of the prim's own bounding box
				Vec3f primpoint = ((pPrim->getBoundingBox().getMaxPoint() -
					pPrim->getBoundingBox().getMinPoint()) / 2);
				double distance = primpoint[i] - box.getMinPoint()[i];//at i dimension
				if(i==0)
					primDistances.push_back(distance);
				else if(i==1)
					primDistances1.push_back(distance);
				else if(i==2)
					primDistances2.push_back(distance);
			}
		//sort the lists for each dimension
		sort(primDistances.begin(), primDistances.end());
		sort(primDistances1.begin(), primDistances1.end());
		sort(primDistances2.begin(), primDistances2.end());
		//we take the middle to balance Left&Right Prims (already sorted)
		//we get the splitVal for each Dimension
		int n = floor(primDistances.size() / 2);
		float splitVal = primDistances.at(n);
		int n1 = floor(primDistances1.size() / 2);
		float splitVal1 = primDistances1.at(n);
		int n2 = floor(primDistances2.size() / 2);
		float splitVal2 = primDistances2.at(n);

		//	now we look at the split value for each of the dimensions,
		//	to calculate the areas of the Left&Right boxes.
		//	so we can determine which dimension split creates
		//	the smallest surface area. 
		//	(for the best ray-primitive intersection probability).
		// 
		//	to calculate the area would be too complicated,
		//	it is easier to just see the ratio between the shortest
		//	path to the splitVal (from either min or max point of box) 
		//	(for a higher primitive density per area), 
		//	to the full length of the box edge in that dimention.
		//
		double shortest = box.getMinPoint()[0] + splitVal;
		if ((box.getMaxPoint()[0] - splitVal) < shortest)
			shortest = box.getMaxPoint()[0] - splitVal;

		double shortest1 = box.getMinPoint()[1] + splitVal1;
		if ((box.getMaxPoint()[1] - splitVal1) < shortest1)
			shortest1 = box.getMaxPoint()[1] - splitVal1;
		
		double shortest2 = box.getMinPoint()[2] + splitVal2;
		if ((box.getMaxPoint()[2] - splitVal2) < shortest2)
			shortest2 = box.getMaxPoint()[2] - splitVal2;
		
		//	now we look for the smallest Ratio, because the smaller ratio
		//	means a higher primitive density in that sub-box.
		//
		//get the Ratios
		double Ratio = shortest / (box.getMaxPoint()[0] - box.getMinPoint()[0]);
		double Ratio1 = shortest1 / (box.getMaxPoint()[1] - box.getMinPoint()[1]);
		double Ratio2 = shortest2 / (box.getMaxPoint()[2] - box.getMinPoint()[2]);
		//determine the smallest ratio
		int splitDim;
		if (Ratio <= Ratio1 && Ratio <= Ratio2)
			splitDim = 0;//x dimension
		else if (Ratio1 <= Ratio && Ratio1 <= Ratio2)
			splitDim = 1;//y dimension
		else
			splitDim = 2;//z dimension	
		//now we know the best dimention to split in
		auto    splitBoxes = box.split(splitDim, splitVal);
		CBoundingBox& lBox = splitBoxes.first;
		CBoundingBox& rBox = splitBoxes.second;



		// Second order the primitives into new bounding boxes
		std::vector<ptr_prim_t> lPrim;
		std::vector<ptr_prim_t> rPrim;
		for (auto pPrim : vpPrims) {
			if (pPrim->getBoundingBox().overlaps(lBox))
				lPrim.push_back(pPrim);
			if (pPrim->getBoundingBox().overlaps(rBox))
				rPrim.push_back(pPrim);
		}

		// Next build recursively 2 subtrees for both halfes
		auto pLeft = build(lBox, lPrim, depth + 1);
		auto pRight = build(rBox, rPrim, depth + 1);

		return std::make_shared<CBSPNode>(splitDim, splitVal, pLeft, pRight);
	}
	*/
	
private:
	CBoundingBox 	m_treeBoundingBox;		///<
	size_t			m_maxDepth;				///< The maximum allowed depth of the tree
	size_t			m_minPrimitives;		///< The minimum number of primitives in a leaf-node
	ptr_bspnode_t   m_root = nullptr;		///<
};
	
