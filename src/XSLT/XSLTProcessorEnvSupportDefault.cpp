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
#include "XSLTProcessorEnvSupportDefault.hpp"



#include <iostream>


#include <util/XMLURL.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>

#include "StylesheetRoot.hpp"
#include "XSLTProcessor.hpp"
#include "XSLTInputSource.hpp"


XSLTProcessorEnvSupportDefault::XSLTProcessorEnvSupportDefault(XSLTProcessor*	theProcessor) :
	XPathEnvSupportDefault(),
	m_processor(theProcessor)
{
}



XSLTProcessorEnvSupportDefault::~XSLTProcessorEnvSupportDefault()
{
}



const NodeRefListBase*
XSLTProcessorEnvSupportDefault::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const
{
	if (m_processor == 0)
	{
		return XPathEnvSupportDefault::getNodeSetByKey(doc,
													   name,
													   ref,
													   resolver,
													   executionContext);
	}
	else
	{
		return m_processor->getNodeSetByKey(doc,
											name,
											ref,
											resolver,
											executionContext);
	}
}

DOM_Document
XSLTProcessorEnvSupportDefault::parseXML(
		const DOMString&	urlString,
		const DOMString&	base) const
{
	if (m_processor == 0)
	{
		return XPathEnvSupportDefault::parseXML(urlString, base);
	}
	else
	{
		XMLParserLiaison& parserLiaison = m_processor->getXMLParserLiaison() ;
		if (0 != m_processor)
		{
			XMLURL xslURL(c_wstr(base), c_wstr(urlString));
			XSLTInputSource		inputSource(xslURL.getURLText());
			return parserLiaison.parseXMLStream(inputSource);
		}
		else
			return DOM_Document();
	}
}



XObject*
XSLTProcessorEnvSupportDefault::getVariable(
			XObjectFactory&		factory,
			const QName&		name) const
{
	if (m_processor == 0)
	{
		return XPathEnvSupportDefault::getVariable(factory,
												   name);
	}
	else
	{
		return m_processor->getVariable(name);
	}
}



bool
XSLTProcessorEnvSupportDefault::shouldStripSourceNode(const DOM_Node&	node) const
{
	if (m_processor == 0)
	{
		return XPathEnvSupportDefault::shouldStripSourceNode(node);
	}
	else
	{
		return m_processor->shouldStripSourceNode(node);
	}
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource				/* where */,
			eClassification		classification,
			const DOM_Node&		/* styleNode */,
			const DOM_Node&		/* sourceNode */,
			const DOMString&	msg,
			int					lineNo,
			int					charOffset) const
{
	std::cerr << msg
			  << ", at line number "
			  << static_cast<long>(lineNo)
			  << " at offset "
			  << static_cast<long>(charOffset)
			  << std::endl;

	return classification == XPathEnvSupport::eError ? true : false;
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource					/* where */,
			eClassification			classification,
			const PrefixResolver*	/* resolver */,
			const DOM_Node&			/* sourceNode */,
			const DOMString&		msg,
			int						lineNo,
			int						charOffset) const
{
	std::cerr << msg
			  << ", at line number "
			  << static_cast<long>(lineNo)
			  << " at offset "
			  << static_cast<long>(charOffset)
			  << std::endl;

	return classification == XPathEnvSupport::eError ? true : false;
}
