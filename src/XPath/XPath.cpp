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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file.
#include "XPath.hpp"



#include <cassert>
#include <cfloat>



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include "FoundIndex.hpp"
#include "MutableNodeRefList.hpp"
#include "QNameByReference.hpp"
#include "PrefixResolver.hpp"
#include "SimpleNodeLocator.hpp"
#include "XLocator.hpp"
#include "XPathException.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



XPath::XPath(bool	createDefaultLocator) :
	m_defaultXLocator(createDefaultLocator == false ? 0 : createXLocatorHandler()),
	m_expression()
{
}



XPath::~XPath()
{
}



void
XPath::shrink()
{
	m_expression.shrink();
}



void
XPath::installFunction(
			const XalanDOMString&	funcName,
			const Function& 		func)
{
	s_functions.InstallFunction(funcName,
							    func);
}



bool
XPath::uninstallFunction(const XalanDOMString&	funcName)
{
	return 	s_functions.UninstallFunction(funcName);
}


XLocator*
XPath::createXLocatorHandler() const
{
	return SimpleNodeLocator::getDefaultInstance();
}



const XObjectPtr
XPath::execute(XPathExecutionContext&	executionContext) const
{
	assert(executionContext.getPrefixResolver() != 0);

	return executeMore(executionContext.getCurrentNode(), 0, executionContext);
}



const XObjectPtr
XPath::execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	// Push and pop the PrefixResolver...
	XPathExecutionContext::PrefixResolverSetAndRestore	theResolverSetAndRestore(
									executionContext,
									&prefixResolver);

	// Push and pop the current node...
	XPathExecutionContext::CurrentNodeSetAndRestore		theNodeSetAndRestore(
									executionContext,
									context);

	return execute(executionContext);
}



const XObjectPtr
XPath::execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext) const
{
	// Push and pop the PrefixResolver...
	XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
									executionContext,
									contextNodeList);	

	return execute(context, prefixResolver, executionContext);
}



const XObjectPtr
XPath::executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext) const
{
	XObjectPtr	result;

	switch(m_expression.m_opMap[opPos])
	{
	case XPathExpression::eOP_XPATH:
		result = executeMore(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eOP_MATCHPATTERN:
		result = matchPattern(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eEMPTY:
		opPos++;
		break;

	case XPathExpression::eOP_OR:
		result = Or(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_AND:
		result = And(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NOTEQUALS:
		result = notequals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EQUALS:
		result = equals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LTE:
		result = lte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LT:
		result = lt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GTE:
		result = gte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GT:
		result = gt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_PLUS:
		result = plus(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MINUS:
	  result = minus(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_MULT:
		result = mult(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_DIV:
		result = div(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MOD:
		result = mod(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_QUO:
		result = quo(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NEG:
		result = neg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_STRING:
		result = string(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_BOOL:
		result = boolean(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NUMBER:
		result = number(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_UNION:
		result = Union(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LITERAL:
		result = literal(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_VARIABLE:
	  result = variable(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_GROUP:
		result = group(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NUMBERLIT:
		result = numberlit(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_ARGUMENT:
		result = arg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EXTFUNCTION:
		result = runExtFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_FUNCTION:
		result = runFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATH:
		result = locationPath(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATHPATTERN:
		result = locationPathPattern(context, opPos, executionContext);
		break;

/*
	case XPathExpression::eOP_PREDICATE:
		break;

	case XPathExpression::XPathExpression::eFROM_ANCESTORS:
		break;

	case XPathExpression::eFROM_ANCESTORS_OR_SELF:
		break;

	case XPathExpression::eFROM_ATTRIBUTES:
		break;

	case XPathExpression::eFROM_CHILDREN:
		break;

	case XPathExpression::eFROM_DESCENDANTS:
		break;

	case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
		break;

	case XPathExpression::eFROM_FOLLOWING:
		break;

	case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
		break;

	case XPathExpression::eFROM_PARENT:
		break;

	case XPathExpression::eFROM_PRECEDING:
		break;

	case XPathExpression::eFROM_PRECEDING_SIBLINGS:
		break;

	case XPathExpression::eFROM_SELF:
		break;

	case XPathExpression::eFROM_NAMESPACE:
		break;

	case XPathExpression::eFROM_ATTRIBUTE:
		break;

	case XPathExpression::eFROM_DOC:
		break;

	case XPathExpression::eFROM_DOCREF:
		break;

	case XPathExpression::eFROM_ID:
		break;
	case XPathExpression::eFROM_IDREF:
		break;

	case XPathExpression::eFROM_ROOT:
		break;

	case eNODETYPE_COMMENT:
		break;

	case eNODETYPE_TEXT:
		break;

	case eNODETYPE_PI:
		break;

	case eNODETYPE_NODE:
		break;

	case eNODETYPE_ROOT:
		break;

	case eNODETYPE_ANYELEMENT:
		break;

	case eNODENAME:
		break;
*/

	default:
		{
			const XalanDOMString	theOpCode = LongToDOMString(m_expression.m_opMap[opPos]);

			executionContext.error(TranscodeFromLocalCodePage("ERROR! Unknown op code: ") + theOpCode,
								   context);
		}
		break;
	}

	return result;
}



double
XPath::getMatchScore(XalanNode*					context,
					 const PrefixResolver&		resolver,
					 XPathExecutionContext&		executionContext) const
{
	double	score = s_MatchScoreNone;

	int		opPos = 0;

	if(m_expression.m_opMap[opPos] == XPathExpression::eOP_MATCHPATTERN)
	{
		assert(context != 0);

		// Push and pop the PrefixResolver...
		XPathExecutionContext::PrefixResolverSetAndRestore	theSetAndRestore(
									executionContext,
									&resolver);

		opPos += 2;

		XLocator*	locator = executionContext.getXLocatorFromNode(context);

		if(0 == locator)
		{
			locator = m_defaultXLocator;
		}
		assert(locator != 0);

		while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN &&
			  score == s_MatchScoreNone)
		{
			const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

			// opPos+=2;		
			score = locator->locationPathPattern(*this, executionContext, *context, opPos);

			if(score == s_MatchScoreNone)
			{
				opPos = nextOpPos;
			}
		}
	}
	else
	{
		executionContext.error(TranscodeFromLocalCodePage("Expected match pattern in getMatchScore!"),
							   context);
	}
	
	return score;
}



double
XPath::nodeTest(
			XalanNode*				context,
			int						/* opPos */,
			int						/* argLen */,
			int						/* stepType */,
			XPathExecutionContext&	executionContext) const
{
	executionContext.warn(TranscodeFromLocalCodePage("XPath needs a derived object to implement nodeTest!"),
						  context);

    return s_MatchScoreNone;
}



void
XPath::getTargetElementStrings(TargetElementStringsVectorType&	targetStrings) const
{
	int opPos = 2;

	targetStrings.reserve(eDefaultTargetStringsSize);

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		opPos += 2;
	  
		while(m_expression.m_opMap[opPos] != XPathExpression::eENDOP)
		{
			const int	nextStepPos =
				m_expression.getNextOpCodePosition(opPos);

			const int	nextOp = m_expression.m_opMap[nextStepPos];

			if(nextOp == XPathExpression::eOP_PREDICATE ||
			   nextOp == XPathExpression::eENDOP)
			{
				const int	stepType = m_expression.m_opMap[opPos];

				opPos += 3;

				switch(stepType)
				{
				case XPathExpression::eOP_FUNCTION:
					targetStrings.push_back(PSEUDONAME_ANY);
					break;

				case XPathExpression::eFROM_ROOT:
					targetStrings.push_back(PSEUDONAME_ROOT);
					break;

				case XPathExpression::eMATCH_ATTRIBUTE:
				case XPathExpression::eMATCH_ANY_ANCESTOR:
				case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
					{
						const int	tok = m_expression.m_opMap[opPos];

						opPos++;

						switch(tok)
						{
						case XPathExpression::eNODETYPE_COMMENT:
							targetStrings.push_back(PSEUDONAME_COMMENT);
							break;

						case XPathExpression::eNODETYPE_TEXT:
							targetStrings.push_back(PSEUDONAME_TEXT);
							break;

						case XPathExpression::eNODETYPE_NODE:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODETYPE_ROOT:
							targetStrings.push_back(PSEUDONAME_ROOT);
							break;

						case XPathExpression::eNODETYPE_ANYELEMENT:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODETYPE_PI:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODENAME:
							{
								// Skip the namespace
								const int	tokenIndex = m_expression.m_opMap[opPos + 1];

								if(tokenIndex >= 0)
								{
									const XalanDOMString&	targetName =
										m_expression.m_tokenQueue[tokenIndex].str();

									if(::equals(targetName, PSEUDONAME_ANY) == true)
									{
										targetStrings.push_back(PSEUDONAME_ANY);
									}
									else
									{
										targetStrings.push_back(targetName);
									}
								}
								else
								{
									targetStrings.push_back(PSEUDONAME_ANY);
								}
							}
							break;

						default:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;
						}
					}
					break;
				}
			}

			opPos = nextStepPos;
		}

		opPos = nextOpPos;
	}
}



const XObjectPtr
XPath::xpath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	return executeMore(context, opPos + 2, executionContext);
}



const XObjectPtr
XPath::matchPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const

{
	XObjectPtr	score;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		score = executeMore(context, opPos, executionContext);
		assert(score.null() == false);

		if(score->num() != s_MatchScoreNone)
		{
			break;
		}
		else
		{
			opPos = nextOpPos;
		}
	}

	if(score.null() == true)
	{
		score = executionContext.getXObjectFactory().createNumber(s_MatchScoreNone);
	}

	return score;
}



MutableNodeRefList*
XPath::step(
			XalanNode*				context,
			int						/* opPos */,
			XPathExecutionContext&	executionContext) const
{    
	executionContext.warn(TranscodeFromLocalCodePage("XPath needs a derived object to implement step!"),
						  context);

	return 0;
}



const XObjectPtr
XPath::Or(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	bool	fResult = expr1->boolean();

	if(fResult == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
		assert(expr2.get() != 0);

		fResult = expr2->boolean();
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



const XObjectPtr
XPath::And(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	if (expr1->boolean() == true)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
		assert(expr2.get() != 0);

		if (expr2->boolean() == true)
		{
			fResult = true;
		}
	}	

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



const XObjectPtr
XPath::notequals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() != *expr2.get());
}



const XObjectPtr
XPath::equals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() == *expr2.get());
}



const XObjectPtr
XPath::lte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() <= *expr2.get());
}



const XObjectPtr
XPath::lt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() < *expr2.get());
}



const XObjectPtr
XPath::gte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() >= *expr2.get());
}



const XObjectPtr
XPath::gt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(*expr1.get() > *expr2.get());
}



const XObjectPtr
XPath::plus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::add(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::minus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::subtract(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::mult(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::multiply(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::div(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::divide(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::mod(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::modulus(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::quo(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	// Actually, this is no longer supported by xpath...
	executionContext.warn(TranscodeFromLocalCodePage("Old syntax: quo(...) is no longer defined in XPath."));

	return div(context, opPos, executionContext);
}



const XObjectPtr
XPath::neg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::negative(expr1->num()));
}



const XObjectPtr
XPath::string(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	// Try to optimize when the result of the execution is
	// already a string.
	if (expr1->getType() == XObject::eTypeString)
	{
		return expr1;
	}
	else
	{
		return executionContext.getXObjectFactory().createString(expr1->str());
	}
}



const XObjectPtr
XPath::boolean(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	// Try to optimize when the result of the execution is
	// already a boolean.
	if (expr1->getType() == XObject::eTypeBoolean)
	{
		return expr1;
	}
	else
	{
		return executionContext.getXObjectFactory().createBoolean(expr1->boolean());
	}
}


 
const XObjectPtr
XPath::number(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	// Try to optimize when the result of the execution is
	// already a number.
	if (expr1->getType() == XObject::eTypeNumber)
	{
		return expr1;
	}
	else
	{
		return executionContext.getXObjectFactory().createNumber(expr1->num());
	}
}



const XObjectPtr
XPath::Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	resultNodeList(executionContext);
	

	while(m_expression.m_opMap[opPos] != XPathExpression::eENDOP)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr = executeMore(context, opPos, executionContext);

		const NodeRefListBase&	nl =
				expr->nodeset();

		resultNodeList->addNodesInDocOrder(nl, executionContext);	

		opPos = nextOpPos;
	}

	return executionContext.getXObjectFactory().createNodeSet(resultNodeList);
}



const XObjectPtr
XPath::literal(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > unsigned(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > unsigned(m_expression.m_opMap[opPos + 2]));

	const XObject&	theLiteral = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	return executionContext.getXObjectFactory().createString(theLiteral.str());
}



const XObjectPtr
XPath::variable(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(executionContext.getPrefixResolver() != 0);

	const XObject&	ns = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	const XObject&	varName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 3]];

	// $$$ ToDo: I don't this will be parsed right in the first place...
	const QNameByReference	qname(ns.str(),
								  varName.str());	

	XObjectPtr result;

	try
	{
		result = executionContext.getVariable(qname);
	}
	catch(...)
	{
		executionContext.error(TranscodeFromLocalCodePage("Could not get variable named ") + varName.str());

		throw;
	}

	return result;
}



const XObjectPtr
XPath::group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	return executeMore(context, opPos + 2, executionContext);
}



const XObjectPtr
XPath::numberlit(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > unsigned(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > unsigned(m_expression.m_opMap[opPos + 2]));

	const XObject&	theLiteral = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	return executionContext.getXObjectFactory().createNumber(theLiteral.num());
}
  


const XObjectPtr
XPath::arg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	return executeMore(context, opPos + 2, executionContext);
}



const XObjectPtr
XPath::locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	assert(context != 0);

/*
	const int	stepType = m_expression.m_opMap[opPos + 2];

	XLocator xlocator = null;
	if(OP_VARIABLE == stepType)
	{
	  // Bit of a hack here...
	  XObject obj = execute(context, opPos+2);
	  NodeList nl = obj.nodeset();
	  if(nl.getLength() > 0)
	  {
		// Use the xlocator of the first node...
		// I guess we should really loop through the contexts, but things 
		// will get very nasty quick, so just do this for now.
		xlocator = m_callbacks.getXLocatorFromNode(nl.item(0));
	  }
	  else
	  {
		xlocator = m_callbacks.getXLocatorFromNode(context);
	  }
	}
	else
	{
	  xlocator = m_callbacks.getXLocatorFromNode(context);
	}
*/

	XLocator*	xlocator = executionContext.getXLocatorFromNode(context);

	if(0 == xlocator)
	{
		xlocator = m_defaultXLocator;
	}

	return xlocator->locationPath(*this, executionContext, *context, opPos);
}



const XObjectPtr
XPath::predicate(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const XObjectPtr expr1 = executeMore(context, opPos + 2, executionContext);

	// $$$ ToDo: This appears to be just an optimization, but is it really?
/*
	int objType = expr1.getType();
	if((XObject.CLASS_NUMBER != objType) && (XObject.CLASS_BOOLEAN != objType))
	{
	  expr1 = expr1.bool() ? m_true : m_false;
	}
*/

	return expr1;
}



const XObjectPtr
XPath::locationPathPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(context != 0);

	XLocator*	locator = executionContext.getXLocatorFromNode(context);

	if(0 == locator)
	{
		locator = m_defaultXLocator;
	}

	const double	result = locator->locationPathPattern(*this, executionContext, *context, opPos);

	return executionContext.getXObjectFactory().createNumber(result);
}



const XObjectPtr
XPath::runExtFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endExtFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	const XalanDOMString&	ns = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]].str();

	opPos++;

	const XalanDOMString&	funcName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]].str();

	opPos++;

	typedef XPathExecutionContext::XObjectArgVectorType		XObjectArgVectorType;

	XObjectArgVectorType	args;

	while(opPos < endExtFunc)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		args.push_back(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
	}

	const XObjectPtr	theResult =
		extfunction(context, opPos, ns, funcName, args, executionContext);

	return theResult;
}



const XObjectPtr
XPath::extfunction(
			XalanNode*								context,
			int										/* opPos */,
			const XalanDOMString&					theNamespace,
			const XalanDOMString&					functionName, 
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const
{
	return 	executionContext.extFunction(theNamespace,
										 functionName,
										 context,
										 argVec);
}



const XObjectPtr
XPath::runFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	// This is actually the position in the token queue of the
	// string that contains the name of the function.
	const int	funcID = m_expression.m_opMap[opPos];

	opPos++;

	// Number of args is next, not used for now...
	const int	argCount = m_expression.m_opMap[opPos];

	opPos++;

	if (argCount == 0)
	{
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context);
	}
	else if (argCount == 1)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
		
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg);
	}
	else if (argCount == 2)
	{
		int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg1(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg2(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
		
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg1, theArg2);
	}
	else if (argCount == 3)
	{

		int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg1(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg2(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg3(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg1, theArg2, theArg3);
	}
	else
	{
		typedef XPathExecutionContext::XObjectArgVectorType		XObjectArgVectorType;

		XObjectArgVectorType	args;

		args.reserve(argCount);

		while(opPos < endFunc)
		{
			const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

			args.push_back(executeMore(context, opPos, executionContext));

			opPos = nextOpPos;
		}

		return function(context, opPos, funcID, args, executionContext);
	}
}



const XObjectPtr
XPath::function(
			XalanNode*								context,
			int										opPos,
			int										funcID,
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const
 
{
	return s_functions[funcID].execute(executionContext, context, opPos, argVec);
}



static XalanDOMString	PSEUDONAME_ANY;
static XalanDOMString	PSEUDONAME_ROOT;
static XalanDOMString	PSEUDONAME_TEXT;
static XalanDOMString	PSEUDONAME_COMMENT;
static XalanDOMString	PSEUDONAME_PI;
static XalanDOMString	PSEUDONAME_OTHER;



const XalanDOMString&	XPath::PSEUDONAME_ANY = ::PSEUDONAME_ANY;
const XalanDOMString&	XPath::PSEUDONAME_ROOT = ::PSEUDONAME_ROOT;
const XalanDOMString&	XPath::PSEUDONAME_TEXT = ::PSEUDONAME_TEXT;
const XalanDOMString&	XPath::PSEUDONAME_COMMENT = ::PSEUDONAME_COMMENT;
const XalanDOMString&	XPath::PSEUDONAME_PI = ::PSEUDONAME_PI;
const XalanDOMString&	XPath::PSEUDONAME_OTHER = ::PSEUDONAME_OTHER;



const double			XPath::s_MatchScoreNone = -9999999999999.0;
const double			XPath::s_MatchScoreQName = 0.0;
const double			XPath::s_MatchScoreNSWild = -0.25;
const double			XPath::s_MatchScoreNodeTest = -0.5;
const double			XPath::s_MatchScoreOther = 0.5;



// Don't auto-create the table...
XPath::FunctionTableType		XPath::s_functions(false);



void
XPath::initialize()
{
	s_functions.CreateTable();

	::PSEUDONAME_ANY = XALAN_STATIC_UCODE_STRING("*");
	::PSEUDONAME_ROOT = XALAN_STATIC_UCODE_STRING("/");
	::PSEUDONAME_TEXT = XALAN_STATIC_UCODE_STRING("#text");
	::PSEUDONAME_COMMENT = XALAN_STATIC_UCODE_STRING("#comment");
	::PSEUDONAME_PI = XALAN_STATIC_UCODE_STRING("#pi");
	::PSEUDONAME_OTHER = XALAN_STATIC_UCODE_STRING("*");
}



void
XPath::terminate()
{
	clear(::PSEUDONAME_ANY);
	clear(::PSEUDONAME_ROOT);
	clear(::PSEUDONAME_TEXT);
	clear(::PSEUDONAME_COMMENT);
	clear(::PSEUDONAME_PI);
	clear(::PSEUDONAME_OTHER);

	s_functions.DestroyTable();
}
