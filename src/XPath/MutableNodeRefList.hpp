/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(MUTABLENODEREFLIST_HEADER_GUARD_1357924680)
#define MUTABLENODEREFLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XPath/NodeRefList.hpp>



class XPathSupport;
class XalanNodeList;



/**
 * Local implementation of MutableNodeRefList.  This class is for internal use
 * only.
 */
class XALAN_XPATH_EXPORT MutableNodeRefList : public NodeRefList
{
public:

	/**
	 * Construct an empty mutable node list.
	 * 
	 * @param theSupport XPath support class instance
	 */
	explicit
	MutableNodeRefList(XPathSupport*	theSupport = 0);

	/**
	 * Construct a mutable node list from another list.
	 * 
	 * @param theSource source list
	 */
	MutableNodeRefList(const MutableNodeRefList&	theSource);

	/**
	 * Construct a mutable node list from another list.
	 * 
	 * @param theSource  source list
	 */
	explicit
	MutableNodeRefList(const NodeRefListBase&	theSource);

	virtual
	~MutableNodeRefList();

	MutableNodeRefList&
	operator=(const MutableNodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefList&		theRHS);

	MutableNodeRefList&
	operator=(const NodeRefListBase&	theRHS);

	MutableNodeRefList&
	operator=(const XalanNodeList*	theRHS);

	/**
	 * Add a node at to the list.
	 * 
	 * @param n node to add
	 */
	virtual void
	addNode(XalanNode*	n);

	/**
	 * Insert a node at a given position.
	 * 
	 * @param n   node to insert
	 * @param pos position of insertion
	 */
	virtual void
	insertNode(
			XalanNode*		n,
			unsigned int	pos);

	/**
	 * Remove a node from the list.
	 * 
	 * @param n   node to insert
	 */
	virtual void
	removeNode(const XalanNode*		n);

	/**
	 * Remove a node from the list.
	 * 
	 * @param pos position of node in list
	 */
	virtual void
	removeNode(unsigned int		pos);

	/**
	 * Remove all nodes.
	 */
	virtual void
	clear();

	/**
	 * Set a item.
	 * 
	 * @param pos position of node to modify
	 * @param n   node to insert, default is empty node
	 */
	virtual void
	setNode(unsigned int	pos,
			XalanNode*		n = 0);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.  If
	 * a node is null, don't add it.
	 * 
	 * @param nodelist node list to add
	 */
	virtual void
	addNodes(const XalanNodeList&	nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.  If
	 * a node is null, don't add it.
	 * 
	 * @param nodelist node list to add
	 */
	virtual void
	addNodes(const NodeRefListBase&		nodelist);

	/**
	 * Copy NodeList members into this nodelist, adding in document order.
	 * 
	 * @param nodelist node list to add
	 */
	virtual void
	addNodesInDocOrder(const XalanNodeList&		nodelist);
  
	/**
	 * Copy NodeList members into this nodelist, adding in document order.
	 * 
	 * @param nodelist node list to add
	 */
	virtual void
	addNodesInDocOrder(const NodeRefListBase&	nodelist);
  
	/**
	 * Add a node into list where it should occur in document order.
	 *
	 * @param node node object to add
	 * @param test true if we should test for doc order
	 */
	virtual void
	addNodeInDocOrder(
			XalanNode*	node,
			bool		test);

	virtual XPathSupport*
	getSupport() const;

protected:

	// Default vector allocation size.  It seems high, but
	// it's really worth it...
	enum
	{
		eDefaultVectorSize = 1000
	};

	/**
	 * Ensure that an allocation is either the default allocation
	 * amount, or the amount specified in the parameter, whichever
	 * is larger.
	 *
	 * @param theSize The requested size.
	 */
	void
	ensureAllocation(NodeListVectorType::size_type	theSize = 0)
	{
		m_nodeList.reserve(eDefaultVectorSize > theSize ? eDefaultVectorSize : theSize);
	}


private:

	XPathSupport*	m_support;
};



#endif	// MUTABLENODEREFLIST_HEADER_GUARD_1357924680
