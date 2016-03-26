/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __HLMS_EDITOR_PLUGIN_H__
#define __HLMS_EDITOR_PLUGIN_H__

#include "OgreString.h"

namespace Ogre
{
    /** Abstract class for Hlms editor plugins */
    class HlmsEditorPlugin
    {
		public:
            HlmsEditorPlugin (void) {}
            virtual ~HlmsEditorPlugin (void) {}
		
			// Does the plugin perform an import?
			virtual bool isImport (void) const = 0;
			
			// Does the plugin perform an export?
			virtual bool isExport (void) const = 0;

			// Return the text to be presented in the import menu
			virtual const String& getImportMenuText (void) const = 0;

			// Return the text to be presented in the export menu
			virtual const String& getExportMenuText (void) const = 0;

			// Perform the import
			virtual bool executeImport (void) = 0;

			// Perform the export
			virtual bool executeExport (void) = 0;
	};
}

#endif