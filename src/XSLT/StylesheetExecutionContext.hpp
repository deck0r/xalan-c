/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#if defined(XALAN_OLD_STREAMS)
#include <iostream.h>
#else
#include <iosfwd>
#endif



#include <cstddef>
#include <memory>



// Base class header file...
#include <XPath/XPathExecutionContext.hpp>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <PlatformSupport/XalanNumberFormat.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>



#include <XSLT/KeyTable.hpp>
#include <XSLT/TopLevelArg.hpp>



class CountersTable;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class FormatterListener;
class FormatterToDOM;
class FormatterToHTML;
class FormatterToText;
class FormatterToXML;
class GenerateEvent;
class KeyTable;
class PrefixResolver;
class NodeRefListBase;
class NodeSorter;
class PrintWriter;
class XalanQName;
class SelectionEvent;
class Stylesheet;
class StylesheetRoot;
class XalanOutputStream;
class TracerEvent;
class Writer;
class XalanDocument;
class XalanDocumentFragment;
class XalanElement;
class XalanNode;
class XalanNumberFormat;
class XPath;
class XObject;
class XObjectPtr;
class XResultTreeFrag;



//
// An abstract class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContext : public XPathExecutionContext
{
public:

	typedef size_t	size_type;

	explicit
	StylesheetExecutionContext(XObjectFactory*	theXObjectFactory = 0);

	virtual
	~StylesheetExecutionContext();

	// These interfaces are new...

	/**
	 * Determine whether conflicts should be reported.
	 * 
	 * @return true if conflicts should not be warned
	 */
	virtual bool
	getQuietConflictWarnings() const = 0;

	/**
	 * If this function returns true, only text nodes can
	 * be copied to the result tree.
	 * 
	 * @return true or false
	 */
	virtual bool
	getCopyTextNodesOnly() const = 0;

	/**
	 * Set the flag that determines if only text nodes
	 * can be copied to the result tree.
	 * 
	 * @param fValue The value of the flag
	 */
	virtual void
	setCopyTextNodesOnly(bool	fValue) = 0;

	/*
	 * A class to manage setting and restoring the flag
	 * for restricting copying only text nodes to the
	 * result tree
	 */
	class SetAndRestoreCopyTextNodesOnly
	{
	public:

		SetAndRestoreCopyTextNodesOnly(
			StylesheetExecutionContext&		executionContext,
			bool							fValue) :
			m_executionContext(executionContext),
			m_fValue(executionContext.getCopyTextNodesOnly())
		{
			executionContext.setCopyTextNodesOnly(fValue);
		}

		~SetAndRestoreCopyTextNodesOnly()
		{
			m_executionContext.setCopyTextNodesOnly(m_fValue);
		}

	private:

		// Not implemented...
		SetAndRestoreCopyTextNodesOnly(const SetAndRestoreCopyTextNodesOnly&);

		SetAndRestoreCopyTextNodesOnly&
		operator=(const SetAndRestoreCopyTextNodesOnly&);

		// Data members...
		StylesheetExecutionContext&		m_executionContext;

		const bool						m_fValue;
	};

	/**
	 * Retrieve root document for stylesheet.  Note that
	 * this does not have to be a XalanDocument -- it can
	 * be any node in a document.
	 * 
	 * @return root document
	 */
	virtual XalanNode*
	getRootDocument() const = 0;

	/**
	 * Set root document for stylesheet.  Note that
	 * this does not have to be a XalanDocument -- it can
	 * be any node in a document.
	 * 
	 * @param theDocument root document
	 */
	virtual void
	setRootDocument(XalanNode*	theDocument) = 0;

	/**
	 * Create a new empty document.
	 * 
	 * @return new document
	 */
	virtual XalanDocument*
	createDocument() const = 0;

	/**
	 * Set root stylesheet for stylesheet.
	 * 
	 * @param theStylesheet root stylesheet
	 */
	virtual void
	setStylesheetRoot(const StylesheetRoot*		theStylesheet) = 0;

	/**
	 * Retrieve the current mode.
	 * 
	 * @return QName for mode
	 */
	virtual const XalanQName*
	getCurrentMode() const = 0;

	/**
	 * Set the current mode.
	 * 
	 * @param theMode QName for mode
	 */
	virtual	void
	setCurrentMode(const XalanQName* theMode) = 0; 

	/**
	 * Retrieve the current template
	 * 
	 * @return The current template instance or null if there is no current template
	 */
	virtual const ElemTemplate*
	getCurrentTemplate() const = 0;

	/**
	 * Set the current template
	 * 
	 * @param theTemplate The current template instance
	 */
	virtual	void
	setCurrentTemplate(const ElemTemplate*	theTemplate) = 0; 

	/*
	 * A class to manage setting and restoring the current
	 * template instance.
	 */
	class SetAndRestoreCurrentTemplate
	{
	public:

		SetAndRestoreCurrentTemplate(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplate*				theTemplate) :
			m_executionContext(executionContext),
			m_template(executionContext.getCurrentTemplate())
		{
			executionContext.setCurrentTemplate(theTemplate);
		}

		~SetAndRestoreCurrentTemplate()
		{
			m_executionContext.setCurrentTemplate(m_template);
		}

	private:

		// Not implemented...
		SetAndRestoreCurrentTemplate(const SetAndRestoreCurrentTemplate&);

		SetAndRestoreCurrentTemplate&
		operator=(const SetAndRestoreCurrentTemplate&);

		// Data members...
		StylesheetExecutionContext&		m_executionContext;

		const ElemTemplate* const		m_template;
	};

	/**
	 * Whether diagnostic output is to be generated
	 * 
	 * @return true for diagnostics output 
	 */
	virtual bool
	doDiagnosticsOutput() const = 0;

	/**
	 * Print a diagnostics string to the output device
	 * 
	 * @param theString string to print
	 */
	virtual void
	diag(const XalanDOMString&	theString) = 0;

	/**
	 * Mark the time, so that displayDuration can later display the elapsed
	 * clock ticks.
	 * 
	 * @param theKey element to push
	 */
	virtual void
	pushTime(const void*	theKey) = 0;

	/**
	 * Display the duration since pushTime was called.
	 *
	 * @param theMessage message to display
	 * @param theKey     key for which duration is displayed
	 */
	virtual void
	displayDuration(
			const XalanDOMString&	theMessage,
			const void*				theKey) = 0;

	/**
	 * See if there is an element pending.
	 */
	virtual bool
	isElementPending() const = 0;

	/**
	 * Replace the contents of a pending attribute.
	 * 
	 * @param theName           name of attribute
	 * @param theNewType        type of attribute
	 * @param theNewValue       new value of attribute
	 */
	virtual void
	replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue) = 0;

	/**
	 * Get the current formatter listener.
	 * 
	 * @return pointer to formatter listener
	 */
	virtual FormatterListener*
	getFormatterListener() const = 0;

	/**
	 * Set the current formatter listener.
	 *
	 * @param flistener pointer to new formatter listener
	 */
	virtual void
	setFormatterListener(FormatterListener*		flistener) = 0;

	virtual void
	pushOutputContext(FormatterListener*	flistener = 0) = 0;

	virtual void
	popOutputContext() = 0;

	class OutputContextPushPop
	{
	public:

		/**
		 * Construct an object to push and pop the current output context.
		 *
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewListener the new FormatterListener to set.
		 */
		OutputContextPushPop(
			StylesheetExecutionContext&		theExecutionContext,
			FormatterListener*				theNewListener = 0) :
			m_executionContext(theExecutionContext)
		{
			m_executionContext.pushOutputContext(theNewListener);
		}

		~OutputContextPushPop()
		{
			m_executionContext.popOutputContext();
		}

	private:

		StylesheetExecutionContext&		m_executionContext;
	};

	/**
	 * Add a result attribute to the list of pending attributes.
	 * 
	 * @param aname name of attribute
	 * @param value value of attribute
	 */
	virtual void
	addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value) = 0;

	/**
	 * Add namespace attributes for a node to the list of pending attributes.
	 * 
	 * @param src                 source node
	 */
	virtual void
	copyNamespaceAttributes(const XalanNode&	src) = 0;

	/**
	 * Retrieve the result prefix corresponding to a namespace.
	 * 
	 * @param theNamespace namespace for prefix
	 *
	 * @return A pointer to a string containing the prefix, or 0 if the namespace is not mapped.
	 */
	virtual const XalanDOMString*
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const = 0;

	/**
	 * Retrieve the result namespace corresponding to a prefix.
	 * 
	 * @param thePrefix prefix for namespace
	 *
	 * @return A pointer to a string containing the namespace, or 0 if the prefix is not mapped.
	 */
	virtual const XalanDOMString*
	getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const = 0;

	/**
	 * Determine whether or not a prefix is in use on the pending element or
	 * the pending attributes.
	 * 
	 * @param thePrefix prefix for namespace
	 *
	 * @return true if the prefix is in use, false if not.
	 */
	virtual bool
	isPendingResultPrefix(const XalanDOMString&	thePrefix) = 0;

	/**
	 * Generate a random namespace prefix guaranteed to be unique.
	 * 
	 * @return unique namespace prefix
	 */
	virtual XalanDOMString
	getUniqueNamespaceValue() const = 0;

	/**
	 * Generate a random namespace prefix guaranteed to be unique.
	 * 
	 * @param theValue A string for returning the new prefix
	 */
	virtual void
	getUniqueNamespaceValue(XalanDOMString&		theValue) const = 0;

	/**
	 * Retrieve the current number of spaces to indent.
	 * 
	 * @return number of spaces
	 */
	virtual int
	getIndent() const = 0;

	/**
	 * Set the current number of spaces to indent.
	 * 
	 * @param indentAmount The number of spaces to indent.  Use -1 for the default amount.
	 */
	virtual void
	setIndent(int	indentAmount) = 0;

	// $$$ ToDo: Remove this one!!!!
	/**
	 * Execute an XPath and return the resulting XObject. The lifetime of this
	 * XObject is not necessarily that of the Stylesheet.
	 *
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return pointer to resulting XObject
	 */
//	virtual const XObjectPtr
//	executeXPath(
//			const XalanDOMString&	str,
//			XalanNode*				contextNode,
//			const XalanElement&		resolver) = 0;

	/**
	 * Create and initialize an xpath and return it. This is to be used to
	 * create an XPath that is only used during execution.
	 *
	 * @param str      string expression for XPath evaluation
	 * @param resolver resolver for namespace resolution
	 * @return pointer to resulting XPath
	 */
	virtual const XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Return the XPath created by createMatchPattern().
	 *
	 * @param xpath The XPath to return.
	 */
	virtual void
	returnXPath(const XPath*	xpath) = 0;

	// A helper class to automatically return an XPath instance.
	class XPathGuard
	{
	public:

		XPathGuard(
				StylesheetExecutionContext&		context,
				const XPath*					xpath = 0) :
			m_context(context),
			m_xpath(xpath)
		{
		}

		~XPathGuard()
		{
			if (m_xpath != 0)
			{
				m_context.returnXPath(m_xpath);
			}
		}

		const XPath*
		get() const
		{
			return m_xpath;
		}

		const XPath*
		release()
		{
			const XPath* const	temp = m_xpath;

			m_xpath = 0;

			return temp;
		}

		void
		reset(const XPath*	xpath)
		{
			if (m_xpath != 0)
			{
				m_context.returnXPath(m_xpath);
			}

			m_xpath = xpath;
		}

	private:

		StylesheetExecutionContext&		m_context;

		const XPath*					m_xpath;
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<TopLevelArg>			ParamVectorType;
#else
	typedef std::vector<TopLevelArg>	ParamVectorType;
#endif

	/**
	 * Set a list of top level variables in the specified execution context
	 * stylesheet.
	 *
	 * @param topLevelParams   list of top level parameters
	 */
	virtual void
	pushTopLevelVariables(const ParamVectorType&	topLevelParams) = 0;

	/**
	 * Execute the supplied XPath and and create a
	 * variable in the current context.
	 *
	 * @param element	  element marker for variable
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return a pointer to the XObject result
	 */
	virtual const XObjectPtr
	createVariable(
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver) = 0;

	/**
	 * Create an ResultTreeFragment as a variable and push it
	 * on to the stack with the current context.
	 *
	 * @param element element marker for variable
	 * @param templateChild result tree fragment to use.
	 * @param sourceNode source node
	 * @return a pointer to the XObject result
	 */
	virtual const XObjectPtr
	createVariable(
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode) = 0;

	/**
	 * Execute an XPath using the provided expression, 
	 * and push the result as a variable in the context of
	 * the supplied element.
	 *
	 * @param name		  name of variable
	 * @param element	  element marker for variable
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return nothing
	 */
	virtual void
	pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const XalanDOMString&		str,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver) = 0;

	/**
	 * Execute the supplied XPath and push the result as a
	 * variable in the current context.
	 *
	 * @param name		  name of variable
	 * @param element	  element marker for variable
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return nothing
	 */
	virtual void
	pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver) = 0;

	/**
	 * Create an ResultTreeFragment as a variable and push it
	 * on to the stack with the current context.
	 *
	 * @param name    name of variable
	 * @param element element marker for variable
	 * @param templateChild result tree fragment to use.
	 * @param sourceNode source node
	 */
	virtual void
	pushVariable(
			const XalanQName&			name,
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode) = 0;

	/**
	 * Push a named variable onto the variables stack.
	 * The variable has already been evaluated.
	 *
	 * @param name    name of variable
	 * @param val     pointer to XObject value
	 * @param element element marker for variable
	 */
	virtual void
	pushVariable(
			const XalanQName&			name,
			const XObjectPtr			val,
			const ElemTemplateElement*	element) = 0;

	/**
	 * Push a named variable onto the processor variable stack
	 * The variable will be evaluated when first referenced.
	 *
	 * @param name    name of variable
	 * @param var     pointer to ElemVariable instance
	 * @param element element marker for variable
	 */
	virtual void
	pushVariable(
			const XalanQName&			name,
			const ElemVariable*			var,
			const ElemTemplateElement*	element) = 0;

	/**
	 * Push a context marker onto the stack to let us know when to stop
	 * searching for a var.
	 */
	virtual void
	pushContextMarker() = 0;

	/**
	 * Pop the current context from the current context stack.
	 */
	virtual void
	popContextMarker() = 0;

	/*
	 * A class to manage pushing and popping an element's stack
	 * frame context.
	 */
	class PushAndPopContextMarker
	{
	public:

		PushAndPopContextMarker(StylesheetExecutionContext&		executionContext) :
			m_executionContext(executionContext)
		{
			executionContext.pushContextMarker();
		}

		~PushAndPopContextMarker()
		{
			m_executionContext.popContextMarker();
		}

	private:

		StylesheetExecutionContext&		m_executionContext;
	};

	/**
	 * Resolve the params that were pushed by the caller.
	 */
	virtual void
	resolveTopLevelParams() = 0;

	/**
	 * Reset the vector of top level parameters.
	 */
	virtual void
	clearTopLevelParams() = 0;

	class ResolveAndClearTopLevelParams
	{
	public:

		ResolveAndClearTopLevelParams(StylesheetExecutionContext&	executionContext) :
			m_executionContext(executionContext)
		{
			m_executionContext.resolveTopLevelParams();
		}

		~ResolveAndClearTopLevelParams()
		{
			m_executionContext.clearTopLevelParams();
		}

	private:

		StylesheetExecutionContext&		m_executionContext;
	};

	/**
	 * Given a template, search for the arguments and push them on the stack.
	 * Also, push default arguments on the stack.
	 *
	 * @param xslCallTemplateElement "call-template" element
	 * @param sourceNode             source node
	 * @param targetTemplate         target template
	 */
	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceNode,
			const ElemTemplateElement*	targetTemplate) = 0;

	/**
	 * Given a name, return a string representing the value, but don't look in
	 * the global space.
	 *
	 * @param theName name of variable
	 * @return pointer to XObject for variable
	 */
	virtual const XObjectPtr
	getParamVariable(const XalanQName&	theName) = 0;

	/**
	 * Push a frame marker for an element.
	 *
	 * @param elem the element
	 */
	virtual void
	pushElementFrame(const ElemTemplateElement*		elem) = 0;

	/**
	 * Pop a frame marker for an element.
	 *
	 * @param elem the element
	 */
	virtual void
	popElementFrame(const ElemTemplateElement*	elem) = 0;

	/*
	 * A class to manage pushing and popping an element's stack
	 * frame context.
	 */
	class PushAndPopElementFrame
	{
	public:

		PushAndPopElementFrame(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		element) :
			m_executionContext(executionContext),
			m_element(element)
		{
			executionContext.pushElementFrame(element);
		}

		~PushAndPopElementFrame()
		{
			m_executionContext.popElementFrame(m_element);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const ElemTemplateElement*		m_element;
	};

	/**
	 * Get the top of the global stack frame.
	 *
	 * @return current value of index
	 */
	virtual int
	getGlobalStackFrameIndex() const = 0;

	/**
	 * Get the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @return current value of index
	 */
	virtual int
	getCurrentStackFrameIndex() const = 0;

	/**
	 * Set the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @param currentStackFrameIndex new value of index
	 */
	virtual void
	setCurrentStackFrameIndex(int	currentStackFrameIndex = -1) = 0;

	/*
	 * A class to manage the state of the variable stacks frame index.
	 */
	class SetAndRestoreCurrentStackFrameIndex
	{
	public:

		SetAndRestoreCurrentStackFrameIndex(
			StylesheetExecutionContext&		executionContext,
			int								newIndex) :
			m_executionContext(executionContext),
			m_savedIndex(executionContext.getCurrentStackFrameIndex())
		{
			executionContext.setCurrentStackFrameIndex(newIndex);
		}

		~SetAndRestoreCurrentStackFrameIndex()
		{
			m_executionContext.setCurrentStackFrameIndex(m_savedIndex);
		}

		int
		getStackFrameIndex() const
		{
			return m_savedIndex;
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const int						m_savedIndex;
	};

	/*
	 * A class to manage stack state during execution.
	 */
	class ParamsPushPop
	{
	public:

		ParamsPushPop(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslCallTemplateElement,
			XalanNode*						sourceNode,
			const ElemTemplateElement*		targetTemplate);

		~ParamsPushPop();

		int
		getStackFrameIndex() const
		{
			return m_savedStackFrameIndex;
		}

	private:

		StylesheetExecutionContext&		m_executionContext;
	
		const int						m_savedStackFrameIndex;
	};

	/**
	 * Receive notification of the beginning of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, before any
	 * other methods in this interface or in DTDHandler (except for
	 * setDocumentLocator).</p>
	 *
	 * @exception SAXException
	 */
	virtual void
	startDocument() = 0;

	/**
	 * Receive notification of the end of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, and it will
	 * be the last method invoked during the parse.  The parser shall
	 * not invoke this method until it has either abandoned parsing
	 * (because of an unrecoverable error) or reached the end of
	 * input.</p>
	 *
	 * @exception SAXException
	 */
	virtual void
	endDocument() = 0;

	/**
	 * Receive notification of the beginning of an element.
	 *
	 * @param name element type name
	 */
	virtual void
	startElement(const XalanDOMChar*	name) = 0;

	/**
	 * Receive notification of the end of an element.
	 *
	 * @param name element type name
	 */
	virtual void
	endElement(const XalanDOMChar*	name) = 0;

	/**
	 * Receive notification of character data.
	 *
	 * @param ch     pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 */
	virtual void
	characters(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length) = 0;

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param ch     pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 */
	virtual void
	charactersRaw(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length) = 0;

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param   data	pointer to comment data
	 */
	virtual void
	comment(const XalanDOMChar*		data) = 0;

	/**
	 * Receive notification of a processing instruction.
	 *
	 * @param target processing instruction target
	 * @param data   processing instruction data, or null if none was supplied
	 */
	virtual void
	processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data) = 0;

	/**
	 * Flush the pending element.
	 */
	virtual void
	flushPending() = 0;

	/**
	 * Clone a node to the result tree
	 *
	 * @param node      node to clone
	 * @param styleNode	the stylesheet element that generated the clone.
	 */
	virtual void
	cloneToResultTree(
			const XalanNode&			node,
			const ElemTemplateElement*	styleNode) = 0;

	/**
	 * Clone a node to the result tree
	 *
	 * @param node                  node to clone
	 * @param nodeType				the type of the node
	 * @param isLiteral             true if a literal element
	 * @param overrideStrip         false if white space stripping should be done
	 * @param shouldCloneAttributes true if attributes should be cloned
	 * @param styleNode				the stylesheet element that generated the clone.
	 */
	virtual void
	cloneToResultTree(
			const XalanNode&			node,
			XalanNode::NodeType			nodeType,
			bool						isLiteral,
			bool						overrideStrip,
			bool						shouldCloneAttributes,
			const ElemTemplateElement*	styleNode) = 0;

	/**
	 * Create an XObject that represents a Result tree fragment.
	 *
	 * @param templateChild result tree fragment to use.
	 * @param sourceNode source node
	 * @return XObject instance
	 */
	virtual const XObjectPtr
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode) = 0;

	/**
	 * Output an object to the result tree by doing the right conversions.
	 * This is public for access by extensions.
	 *
	 * @param obj the XObject to output
	 * @param styleNode	the stylesheet element that generate the fragment.
	 */
	virtual void
	outputToResultTree(
			const XObject&				xobj,
			const ElemTemplateElement*	styleNode) = 0;

	/**
	 * Given a result tree fragment, walk the tree and
	 * output it to the result stream.
	 *
	 * @param theTree result tree fragment
	 * @param styleNode	the stylesheet element that generate the fragment.
	 */
	virtual void
	outputResultTreeFragment(
			const XObject&				theTree,
			const ElemTemplateElement*	styleNode) = 0;

	/**
	 * Determine the full XSLT Namespace URI.
	 *
	 * @return Xalan namespace URI
	 */
	virtual const XalanDOMString&
	getXSLNameSpaceURL() const = 0;

	/**
	 * Special Xalan namespace for built-in extensions.
	 *
	 * @return Xalan namespace for extensions
	 */
	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const = 0;

	/**
	 * Determine if an element is on the recursion stack.
	 *
	 * @return true if element on stack
	 */
	virtual bool
	findOnElementRecursionStack(const ElemTemplateElement*	theElement) const = 0;

	/**
	 * Push an element onto the recursion stack.
	 *
	 * @param theElement pointer to element to push
	 */
	virtual void
	pushOnElementRecursionStack(const ElemTemplateElement*	theElement) = 0;

	/**
	 * Pop an element off the recursion stack.
	 *
	 * @return pointer to element popped
	 */
	virtual const ElemTemplateElement*
	popElementRecursionStack() = 0;

	/**
	 * Class for keeping track of elements pushed on the element recursion stack
	 */
	class ElementRecursionStackPusher
	{
	public:

	/**
	 * Construct an instance of the recursion stack pusher.
	 *
	 * @param executionContext current execution context
	 * @param element pointer to element to push
	 */
		ElementRecursionStackPusher(
					StylesheetExecutionContext&		executionContext,
					const ElemTemplateElement*		element) :
			m_executionContext(executionContext)
		{
			m_executionContext.pushOnElementRecursionStack(element);
		}

		~ElementRecursionStackPusher()
		{
			m_executionContext.popElementRecursionStack();
		}

	private:

		StylesheetExecutionContext&			m_executionContext;
	};


	/**
	 * This is a hook that XResultTreeFrag instances (which are reference
	 * counted), can notify the owning StylesheetExecutionContext instance
	 * when they are dereferenced and can be cleaned up.
	 *
	 * @param theXResultTreeFrag The instance that is being returned.
	 *
	 * @return true if the XResultTreeFrag instance belongs to the execution context. false if not.
	 */
	virtual bool
	returnXResultTreeFrag(XResultTreeFrag*	theXResultTreeFrag) = 0;


	enum eDummy
	{
		eDefaultXMLIndentAmount = 0,
		eDefaultHTMLIndentAmount = 0,
	};

	/**
	 * Enums to determine whether or not run-time escaping of URLs has been set.
	 */
	enum eEscapeURLs
	{
		eEscapeURLsDefault,		// Use the value in the stylesheet
		eEscapeURLsNo,			// Don't escape URLs
		eEscapeURLsYes			// Escape URLs
	};

	/**
	 * Get the value for run-time escaping of URLs.  This can
	 * override the property specified by the stylesheet.  The
	 * default behavior is to honor the property in the stylesheet.
	 *
	 * @return The value of the enum
	 */
	virtual eEscapeURLs
	getEscapeURLs() const = 0;

	/**
	 * Set the value for run-time escaping of URLs.  This can
	 * override the property specified by the stylesheet.  The
	 * default behavior is to honor the property in the stylesheet.
	 *
	 * @param value The value of the enum
	 */
	virtual void
	setEscapeURLs(eEscapeURLs	value) = 0;


	/**
	 * Enums to determine whether or not run-time omission of the META tag has been set.
	 */
	enum eOmitMETATag
	{
		eOmitMETATagDefault,	// Use the value in the stylesheet
		eOmitMETATagNo,			// Don't omit the META tag
		eOmitMETATagYes			// Omit the META tag
	};

	/**
	 * Get the value for run-time omission of URLs.  This can
	 * override the property specified by the stylesheet.  The
	 * default behavior is to honor the property in the stylesheet.
	 *
	 * @return The value of the enum
	 */
	virtual eOmitMETATag
	getOmitMETATag() const = 0;

	/**
	 * Get the value for run-time omission of URLs.  This can
	 * override the property specified by the stylesheet.  The
	 * default behavior is to honor the property in the stylesheet.
	 *
	 * @param value The value of the enum
	 */
	virtual void
	setOmitMETATag(eOmitMETATag		value) = 0;

	/**
	 * Create a new FormatterToXML instance.  The execution context
	 * owns the instance and will delete it when reset.
	 *
	 * @param writer            character output stream to use
	 * @param version           version of the output method
	 * @param doIndent          true if output is to be indented
	 * @param indent            number of spaces to indent at each nesting level
	 * @param encoding			character encoding for the writer
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param xmlDecl           true if the XSLT processor should output an XML
	 *                          declaration
	 * @param standalone        true if the XSLT processor should output a
	 *                          standalone document declaration
	 * @return a pointer to the new instance.
	 */
	virtual FormatterToXML*
	createFormatterToXML(
			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(),
			bool					doIndent = false,
			int						indent = eDefaultXMLIndentAmount,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString()) = 0;

	/**
	 * Create a new FormatterToHTML instance.  The execution context
	 * owns the instance and will delete it when reset.
	 *
	 * @param writer            character output stream to use
	 * @param encoding			character encoding for the writer
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param doIndent          true if output is to be indented
	 * @param indent            number of spaces to indent at each nesting level
	 * @param escapeURLs        Whether or not to escape URLs according to the recommendation.  The default is true.
	 * @param omitMetaTag       Whether or not to output a META TAG according to the recommendation.  The default is false.
	 * @return a pointer to the new instance.
	 */
	virtual FormatterToHTML*
	createFormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					doIndent = true,
			int						indent = eDefaultHTMLIndentAmount,
			bool					escapeURLs = true,
			bool					omitMetaTag = false) = 0;

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes, default none
	 * @param currentElement current element for nodes, default none
	 */
	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag,
			XalanElement*			currentElement) = 0;

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes, default none
	 * @param currentElement current element for nodes, default none
	 */
	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem) = 0;

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param writer writer for output
	 * @param encoding character encoding for the writer
	 */
	virtual FormatterToText*
	createFormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding) = 0;

	/**
	 * Borrow a cached FormatterToText instance.
	 *
	 * @return A pointer to the instance.
	 */
	virtual FormatterToText*
	borrowFormatterToText() = 0;

	/**
	 * Return a previously borrowed FormatterToText instance.
	 *
	 * @param theFormatter A pointer the to previously borrowed instance.
	 * @return true if the instance was previously borrowed, false if not.
	 */
	virtual bool
	returnFormatterToText(FormatterToText*	theFormatter) = 0;

	class BorrowReturnFormatterToText
	{
	public:

		BorrowReturnFormatterToText(
				StylesheetExecutionContext&		executionContext,
				Writer&							writer,
				bool							normalizeLinefeed = true,
				bool							handleIgnorableWhitespace = true);

		~BorrowReturnFormatterToText()
		{
			assert(m_formatter != 0);

			m_executionContext.returnFormatterToText(m_formatter);
		}

		FormatterToText&
		operator*() const
		{
			assert(m_formatter != 0);

			return *m_formatter;
		}

		FormatterToText*
		get() const
		{
			assert(m_formatter != 0);

			return m_formatter;
		}

		FormatterToText*
		operator->() const
		{
			return get();
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		FormatterToText*				m_formatter;
	};


	/**
	 * Borrow a cached NodeSorter instance.
	 *
	 * @return A pointer to the instance.
	 */
	virtual NodeSorter*
	borrowNodeSorter() = 0;

	/**
	 * Return a previously borrowed NodeSorter instance.
	 *
	 * @param theSorter A pointer the to previously borrowed instance.
	 * @return true if the instance was previously borrowed, false if not.
	 */
	virtual bool
	returnNodeSorter(NodeSorter*	theSorter) = 0;

	class BorrowReturnNodeSorter
	{
	public:

		BorrowReturnNodeSorter(StylesheetExecutionContext&	executionContext) :
			m_executionContext(executionContext),
			m_sorter(executionContext.borrowNodeSorter())
		{
			assert(m_sorter != 0);
		}

		~BorrowReturnNodeSorter()
		{
			assert(m_sorter != 0);

			m_executionContext.returnNodeSorter(m_sorter);
		}

		NodeSorter&
		operator*() const
		{
			assert(m_sorter != 0);

			return *m_sorter;
		}

		NodeSorter*
		get() const
		{
			assert(m_sorter != 0);

			return m_sorter;
		}

		NodeSorter*
		operator->() const
		{
			return get();
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		NodeSorter*						m_sorter;
	};


	typedef XalanAutoPtr<XalanNumberFormat>		XalanNumberFormatAutoPtr;

	/**
	 * Create a new XalanNumberFormat instance.
	 *
	 * @return an XalanNumberFormatAutoPtr that owns a new
	 * XalanNumberFormat instance.
	 */
	virtual XalanNumberFormatAutoPtr
	createXalanNumberFormat() = 0;


	// Trace interface...

	/**
	 * Determine the number of trace listeners.
	 * 
	 * @return number of listeners
	 */
	virtual size_type
	getTraceListeners() const = 0;

	/**
	 * Fire a generate event.
	 * 
	 * @param ge generate event to fire
	 */
	virtual void
	fireGenerateEvent(const GenerateEvent&	ge) = 0;

	/**
	 * Fire a trace event.
	 * 
	 * @param te trace event to fire
	 */
	virtual void
	fireTraceEvent(const TracerEvent&	te) = 0;

	/**
	 * Fire a selection event.
	 * 
	 * @param se selection event to fire
	 */
	virtual void
	fireSelectEvent(const SelectionEvent&	se) = 0;

	/**
	 * If this is set to true, simple traces of template calls are made.
	 *
	 * @return true if traces made
	 */
	virtual bool
	getTraceSelects() const = 0;

	/**
	 * Compose a diagnostic trace of the current selection
	 *
	 * @param theStylesheetElement The executing stylesheet element
	 * @param nl The list of selected nodes
	 * @param xpath A pointer to the XPath which generated the list of nodes, if any.
	 */
	virtual void
	traceSelect(
			const ElemTemplateElement&	theStylesheetElement,
			const NodeRefListBase&		nl,
			const XPath*				xpath) = 0;

	enum eCaseOrder { eDefault, eLowerFirst, eUpperFirst };

	/**
	 * Compare two strings using the collation of the
	 * current locale.
	 *
	 * @param theLHS a string to compare
	 * @param theRHS a string to compare
	 * @param theCaseOrder the case order for the comparison
	 * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
	 */
	virtual int
	collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS,
			eCaseOrder				theCaseOrder = eDefault) = 0;

	/**
	 * Compare two strings using the collation of the
	 * supplied locale.
	 *
	 * @param theLHS a string to compare
	 * @param theRHS a string to compare
	 * @param theLocal a string that specifies the locale
	 * @param theCaseOrder the case order for the comparison
	 * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
	 */
	virtual int
	collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS,
			const XalanDOMString&	theLocale,
			eCaseOrder				theCaseOrder = eDefault) = 0;

	/**
	 * Compare two strings using the collation of the
	 * current locale.
	 *
	 * @param theLHS a string to compare
	 * @param theRHS a string to compare
	 * @param theCaseOrder the case order for the comparison
	 * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
	 */
	virtual int
	collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			eCaseOrder				theCaseOrder = eDefault) = 0;

	/**
	 * Compare two strings using the collation of the
	 * current locale.
	 *
	 * @param theLHS a string to compare
	 * @param theRHS a string to compare
	 * @param theLocal a string that specifies the locale
	 * @param theCaseOrder the case order for the comparison
	 * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
	 */
	virtual int
	collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			const XalanDOMChar*		theLocale,
			eCaseOrder				theCaseOrder = eDefault) = 0;

	/**
	 * Determine if a KeyDeclaration is being constructed.
	 *
	 * @param The KeyDeclaration in question.
	 * @return true if being constructed
	 */
	virtual	bool
	getInConstruction(const KeyDeclaration&		keyDeclaration) const = 0;

	/**
	 * Add KeyDeclaration to construction list.
	 * 
	 * @param KeyDeclaration being constructed
	 */
	virtual	void
	beginConstruction(const KeyDeclaration& keyDeclaration) = 0; 

	/**
	 * Remove KeyDeclaration from construction list.
	 * 
	 * @param constructed KeyDeclaration
	 */
	virtual	void
	endConstruction(const KeyDeclaration& keyDeclaration) = 0;

	/**
	 * Create a PrintWriter for the provided stream.
	 * 
	 * @param theTextOutputStream The output stream for the PrintWriter.
	 * @return The new instance.
	 */
	virtual PrintWriter*
	createPrintWriter(XalanOutputStream*		theTextOutputStream) = 0;

	/**
	 * Create a PrintWriter.  Create an appropriate output stream
	 * using the provided file name and encoding.
	 * 
	 * @param theFileName The file name for the output stream
	 * @param theEncoding The encoding for the output stream
	 * @return The new instance.
	 */
	virtual PrintWriter*
	createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		theEncoding) = 0;

	/**
	 * Create a PrintWriter using the provided ostream instance.
	 * 
	 * @param ostream The output stream for the PrintWriter.
	 * @return The new instance.
	 */
	virtual PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
	createPrintWriter(ostream&			theStream) = 0;
#else
	createPrintWriter(std::ostream&		theStream) = 0;
#endif

	/**
	 * Get the counters table, which is a table of cached
	 * results that is used by ElemNumber.
	 * 
	 * @return A reference to the counters table.
	 */
	virtual CountersTable&
	getCountersTable() = 0;

	/**
	 * Send character data from a node to the result tree.
	 *
	 * @param node The node to send.
	 */
	virtual void
	characters(const XalanNode&		node) = 0;

	/**
	 * Send character data from an XObject to the result tree.
	 *
	 * @param node The xobject to send.
	 */
	virtual void
	characters(const XObjectPtr&	xobject) = 0;

	/**
	 * Send raw character data from a node to the result tree.
	 *
	 * @param node The node to send.
	 * @param length number of characters to read from the array
	 */
	virtual void
	charactersRaw(const XalanNode&	node) = 0;

	/**
	 * Send raw character data from an XObject to the result tree.
	 *
	 * @param node The xobject to send.
	 */
	virtual void
	charactersRaw(const XObjectPtr&		xobject) = 0;


	// These interfaces are inherited from XPathExecutionContext...

	virtual void
	reset() = 0;

	virtual XalanNode*
	getCurrentNode() const = 0;

	virtual void
	setCurrentNode(XalanNode*	theCurrentNode) = 0;

	virtual XObjectPtr
	createNodeSet(XalanNode&	theNode) = 0;

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const = 0;

	virtual const NodeRefListBase&
	getContextNodeList() const = 0;

	virtual void	
	setContextNodeList(const NodeRefListBase&	theList) = 0;

	virtual size_type
	getContextNodeListLength() const = 0;

	virtual size_type
	getContextNodeListPosition(const XalanNode&		contextNode) const = 0;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const = 0;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const = 0;

	virtual const XObjectPtr
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const Locator*					locator) = 0;

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const = 0;

	virtual MutableNodeRefList*
	borrowMutableNodeRefList() = 0;

	virtual bool
	returnMutableNodeRefList(MutableNodeRefList*	theList) = 0;

	virtual MutableNodeRefList*
	createMutableNodeRefList() const = 0;

	virtual XalanDOMString&
	getCachedString() = 0;

	virtual bool
	releaseCachedString(XalanDOMString&		theString) = 0;

	virtual void
	getNodeSetByKey(
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			MutableNodeRefList&		nodelist) = 0;

	virtual const XObjectPtr
	getVariable(
			const XalanQName&	name,
			const Locator*		locator = 0) = 0;

	virtual const PrefixResolver*
	getPrefixResolver() const = 0;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver) = 0;

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const = 0;

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const = 0;

	virtual const XalanDOMString&
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const = 0;

	virtual bool
	shouldStripSourceNode(const XalanNode&	node) = 0;

	virtual bool
	getThrowFoundIndex() const = 0;

	virtual void
	setThrowFoundIndex(bool 	fThrow) = 0;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const = 0;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument) = 0;


	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanQName&	qname) = 0;

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const = 0;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const = 0;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;
};



#endif	// STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680
