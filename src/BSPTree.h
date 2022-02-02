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
		for (auto EachPrim : vpPrims) {
			res.extend(EachPrim->getBoundingBox());
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
	/*
		As soon as you have reached a maximum depth (e.g. 20), 
		or you have less then a minimum number of primitives (e.g. 3 or 4), 
		stop subdividing and generate a leaf node. Otherweise, 
		split your bounding box in the middle (in the maximum dimension), 
		sort your current primitives into two vector left and right, 
		and recursively call BuildTree with the respective bounding boxes and vector for left and right. 
		Start subdivision with a list of all primitives, the total scene bounds, and an initial recursion depth of 0.
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
		// --- PUT YOUR CODE HERE ---
		
		double t0 = 0;
		double t1 = ray.t;
		m_treeBoundingBox.clip(ray, t0, t1);
		if (t1 < t0) return false;  

		return m_root->intersect(ray, t0, t1);

		//return false;
	}


private:
	/**
	 * @brief Builds the BSP tree
	 * @details This function builds the BSP tree recursively
	 * @param box The bounding box containing all the scene primitives
	 * @param vpPrims The vector of pointers to the primitives included in the bounding box \b box
	 * @param depth The distance from the root node of the tree
	 */
	ptr_bspnode_t build(const CBoundingBox& box, const std::vector<ptr_prim_t>& vpPrims, size_t depth)
	{
		//first check if we need to build branch instead of returning single node
		if (depth >= m_maxDepth || vpPrims.size() <= m_minPrimitives) {
			return std::make_shared<CBSPNode>(vpPrims);
		}
		/*
			we need to have splitDim and splitval 
			MaxDim for retuning best index
			split value according to the target dimensions value
		*/
		std::cout << "size of vectors " << vpPrims.size() << std::endl;
		int splitDim = MaxDim(box.getMaxPoint() - box.getMinPoint());
		auto MaxPoint = box.getMaxPoint();
		auto MinPoint = box.getMinPoint();
		//float splitVal = (MaxPoint[splitDim] - MinPoint[splitDim]) / 2.0f;
		float splitVal = (MaxPoint[splitDim] + MinPoint[splitDim]) / 2.0f;
		/*std::cout << "Max Point " << MaxPoint << " Min Point" << MinPoint << std::endl;
		std::cout << "splitdim " << splitDim << " splitval " << splitVal << std::endl;
		*/
		
		/*
			build branch
			CBSPNode(int splitDim, float splitVal, ptr_bspnode_t left, ptr_bspnode_t right)
			: CBSPNode(std::nullopt, splitDim, splitVal, left, right)
			{}
		*/
		std::pair<CBoundingBox, CBoundingBox> SplitPair = box.split(splitDim, splitVal);
		auto LeftBox = SplitPair.first;
		auto RightBox = SplitPair.second;
		/*
			sort current prims
			check if leftbox and rightbox overlaps with each prims
		*/
		std::vector <ptr_prim_t> PrimsForLeft;
		std::vector <ptr_prim_t> PrimsForRight;
		for (auto EachPrim : vpPrims) {
			//if overlap with left box then added to left box's prims, same to the right
			/*if (LeftBox.overlaps(EachPrim->getBoundingBox())) {
				PrimsForLeft.push_back(EachPrim);
			}
			if (LeftBox.overlaps(EachPrim->getBoundingBox())) {
				PrimsForRight.push_back(EachPrim);
			}*/
			if (EachPrim->getBoundingBox().overlaps(LeftBox)) {
				PrimsForLeft.push_back(EachPrim);
			}
			if (EachPrim->getBoundingBox().overlaps(RightBox)) {
				PrimsForRight.push_back(EachPrim);
			}

		}
		//building branches to next right and left by boxes constructed above
		auto NextLeft = build(LeftBox, PrimsForLeft, depth + 1);
		auto NextRight = build(RightBox, PrimsForRight, depth + 1);
		auto NextBranch = std::make_shared<CBSPNode>(splitDim, splitVal, NextLeft, NextRight);

		return NextBranch;
	}

	
private:
	CBoundingBox 	m_treeBoundingBox;		///<
	size_t			m_maxDepth;				///< The maximum allowed depth of the tree
	size_t			m_minPrimitives;		///< The minimum number of primitives in a leaf-node
	ptr_bspnode_t   m_root = nullptr;		///<
};
	
