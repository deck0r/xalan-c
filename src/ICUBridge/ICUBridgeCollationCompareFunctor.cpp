/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#include "ICUBridgeCollationCompareFunctor.hpp"
#include "ICUBridge.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



#include <unicode/coll.h>



const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor		ICUBridgeCollationCompareFunctor::s_defaultFunctor;



ICUBridgeCollationCompareFunctor::ICUBridgeCollationCompareFunctor() :
	m_isValid(false),
	m_collator(0)
{
	UErrorCode	theStatus = U_ZERO_ERROR;

	m_collator = Collator::createInstance(theStatus);

	if (theStatus == U_ZERO_ERROR || theStatus == U_USING_DEFAULT_ERROR)
	{
		m_isValid = true;
	}
}



ICUBridgeCollationCompareFunctor::~ICUBridgeCollationCompareFunctor()
{
	delete m_collator;
}



static UChar	dummy = 0;



int
ICUBridgeCollationCompareFunctor::operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
{
	if (isValid() == false)
	{
		return s_defaultFunctor(theLHS, theRHS);
	}
	else
	{
		assert(m_collator != 0);

#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
		return m_collator->compare(
					ICUBridge::XalanDOMCharStringToUnicodeString(theLHS),
					ICUBridge::XalanDOMCharStringToUnicodeString(theRHS));
#else
		// $$$ ToDo: This code is necessary because DOMStrings can
		// have a null representation.
		const XalanDOMChar* const	lhs = theLHS == 0 ? &dummy : theLHS;
		const XalanDOMChar* const	rhs = theRHS == 0 ? &dummy : theRHS;

		return m_collator->compare(
					lhs,
					length(lhs),
					rhs,
					length(rhs));
#endif
	}
}
