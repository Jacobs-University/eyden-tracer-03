// BSP node class for BSP trees
// Written by Dr. Sergey G. Kosov in 2019 for Jacobs University
#pragma once

#include "types.h"

class CBSPNode;
using ptr_bspnode_t = std::shared_ptr<CBSPNode>;

// ================================ BSP Node Class ================================
/**
 * @brief Binary Space Partitioning (BSP) node class
 */
class CBSPNode
{
public:
	/**
	 * @brief Leaf node constructor
	 * @param vpPrims The vector of pointers to the primitives included in the leaf node
	 */
	CBSPNode(const std::vector<ptr_prim_t>& vpPrims)
		: CBSPNode(vpPrims, 0, 0, nullptr, nullptr)
	{}
	/**
	 * @brief Branch node constructor
	 * @param splitDim The splitting dimension
	 * @param splitVal The splitting value
	 * @param left Pointer to the left sub-tree
	 * @param right Pointer to the right sub-tree
	 */
	CBSPNode(int splitDim, float splitVal, ptr_bspnode_t left, ptr_bspnode_t right)
		: CBSPNode(std::nullopt, splitDim, splitVal, left, right)
	{}
	CBSPNode(const CBSPNode&) = delete;
	virtual ~CBSPNode(void) = default;
	const CBSPNode& operator=(const CBSPNode&) = delete;

	/**
	 * @brief Traverses the ray \b ray and checks for intersection with a primitive
	 * @details If the intersection is found, \b ray.t is updated
	 * @param[in,out] ray The ray
	 * @param[in] t0 The distance from ray origin at which the ray enters the scene
	 * @param[in] t1 The distance from ray origin at which the ray leaves the scene
	 * @retval true If ray \b ray intersects any object
	 * @retval false otherwise
	 */
	bool intersect(Ray& ray, double t0, double t1) const
	{
		if (isLeaf()) {
			bool hit = false;
			for (const auto prim : m_vpPrims) {
				hit |= prim->intersect(ray);
			}
			return hit;
		} else {
			// dist between split and ray
			double dist = (m_splitVal - ray.org[m_splitDim]) / ray.dir[m_splitDim];
			
			//determin which node is in front
			ptr_bspnode_t front;
			ptr_bspnode_t back;
			if (ray.dir[m_splitDim] < 0) {
				front = Right();
				back  = Left();
			} else {
				front = Left();
				back  = Right();
			}
			
			if (dist <= t0) { //try back
				return back->intersect(ray, t0, t1);
			}
			if (dist >= t1) { //try front
				return front->intersect(ray, t0, t1);
			}
			//try both
			return front->intersect(ray, t0, dist) || back->intersect(ray, dist, t1);
		}
	}

	/**
	 * @brief Returns the pointer to the \a left child
	 * @returns The pointer to the root-node of the \a left sub-tree
	 */
	ptr_bspnode_t Left(void) const { return m_pLeft; }
	/**
	 * @brief Returns the pointer to the \a right child
	 * @returns The pointer to the root-node of the \a right sub-tree
	 */
	ptr_bspnode_t Right(void) const { return m_pRight; }

	
private:
	/**
	 * @brief Genereal private constructor
	 * @details This constructor may contruct both types of the nodes (leaf and branch) and is to be called from public leaf- and branch-node contructors
	 * @param vpPrims The vector of pointers to the primitives included in the leaf node
	 * @param splitDim The splitting dimension
	 * @param splitVal The splitting value
	 * @param left Pointer to the left sub-tree
	 * @param right Pointer to the right sub-tree
	 */
	CBSPNode(std::optional<std::vector<ptr_prim_t>> vpPrims, int splitDim, float splitVal, ptr_bspnode_t left, ptr_bspnode_t right)
		: m_vpPrims(vpPrims.value_or(std::vector<ptr_prim_t>()))
		, m_splitDim(splitDim)
		, m_splitVal(splitVal)
		, m_pLeft(left)
		, m_pRight(right)
	{}

	/**
	 * @brief Checks whether the node is either leaf or branch node
	 * @retval true if the node is the leaf-node
	 * @retval false if the node is a branch-node
	 */
	bool isLeaf(void) const { return (!m_pLeft && !m_pRight); }
	
	
private:
	std::vector<ptr_prim_t>	m_vpPrims;		///< The vector of pointers to the primitives included in the leaf node
	int 					m_splitDim;		///< The splitting dimension
	float 					m_splitVal;		///< The splitting value
	ptr_bspnode_t 	        m_pLeft;		///< Pointer to the left sub-tree
	ptr_bspnode_t 	        m_pRight;		///< Pointer to the right sub-tree
};

