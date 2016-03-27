/*
  -----------------------------------------------------------------------------
  This source file is part of OGRE
  (Object-oriented Graphics Rendering Engine)
  For the latest info, see http://www.ogre3d.org/

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

#include "OgreRoot.h"
#include "JsonExportPlugin.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsPbsDatablock.h"
#include "OgreHlmsUnlit.h"
#include "OgreHlmsUnlitDatablock.h"
#include "OgreHlmsJson.h"
#include "OgreHlmsManager.h"
#include "OgreLogManager.h"
#include "OgreItem.h"
#include <iostream>
#include <fstream>

namespace Ogre
{
	const String sImportMenuText = "";
	const String sExportMenuText = "JsonExport: Export material browser to Json";
	//---------------------------------------------------------------------
	JsonExportPlugin::JsonExportPlugin()
    {
    }
    //---------------------------------------------------------------------
    const String& JsonExportPlugin::getName() const
    {
        return GENERAL_HLMS_PLUGIN_NAME;
    }
    //---------------------------------------------------------------------
    void JsonExportPlugin::install()
    {
    }
    //---------------------------------------------------------------------
    void JsonExportPlugin::initialise()
    {
        // nothing to do
	}
    //---------------------------------------------------------------------
    void JsonExportPlugin::shutdown()
    {
        // nothing to do
    }
    //---------------------------------------------------------------------
    void JsonExportPlugin::uninstall()
    {
    }
	//---------------------------------------------------------------------
	bool JsonExportPlugin::isImport (void) const
	{
		return false;
	}
	//---------------------------------------------------------------------
	bool JsonExportPlugin::isExport (void) const
	{
		return true;
	}
	//---------------------------------------------------------------------
	const String& JsonExportPlugin::getImportMenuText (void) const
	{
		return sImportMenuText;
	}
	//---------------------------------------------------------------------
	const String& JsonExportPlugin::getExportMenuText (void) const
	{
		return sExportMenuText;
	}
	//---------------------------------------------------------------------
	bool JsonExportPlugin::executeImport (HlmsEditorPluginData* data)
	{
		// nothing to do
		return true;
	}
	//---------------------------------------------------------------------
	bool JsonExportPlugin::executeExport (HlmsEditorPluginData* data)
	{
		// Error in case no materials available
		if (data->mInMaterialFileNameVector.size() == 0)
		{
			data->mOutSuccessText = "No materials to export";
			return true;
		}

		// First, delete all datablocks before loading the new ones
		destroyAllDatablocks(data);

		// Iterate through the json files of the material browser and load them into Ogre
		std::vector<String> materials = data->mInMaterialFileNameVector;
		std::vector<String>::iterator it;
		std::vector<String>::iterator itStart = materials.begin();
		std::vector<String>::iterator itEnd = materials.end();
		String fileName;
		for (it = itStart; it != itEnd; ++it)
		{
			// Load the materials
			fileName = *it;
			if (fileName.empty())
			{
				data->mOutErrorText = "Trying to process a non-existing material filename";
				return false;
			}

			if (!loadMaterial(fileName))
			{
				data->mOutErrorText = "Error while processing the materials";
				return false;
			}
		}

		// Combine all currently created materials into one Json file
		Root* root = Root::getSingletonPtr();
		HlmsManager* hlmsManager = root->getHlmsManager();
		String exportPbsFileName = data->mInProjectName + ".pbs.material.json";
		String exportUnlitFileName = data->mInProjectName + ".unlit.material.json";
		data->mOutExportReference = exportPbsFileName + " + " + exportUnlitFileName;
		hlmsManager->saveMaterials(HLMS_PBS, data->mInProjectPath + exportPbsFileName);
		hlmsManager->saveMaterials(HLMS_UNLIT, data->mInProjectPath + exportUnlitFileName);
		data->mOutSuccessText = "Exporting materials to " + data->mOutExportReference;
		return true;
	}

	//---------------------------------------------------------------------
	bool JsonExportPlugin::loadMaterial(const String& fileName)
	{
		// Read the json file as text file and feed it to the HlmsManager::loadMaterials() function
		// Note, that the resources (textures, etc.) must be present (in resource loacation)

		Root* root = Root::getSingletonPtr();
		HlmsManager* hlmsManager = root->getHlmsManager();
		HlmsJson hlmsJson(hlmsManager);

		// Read the content of the file into a string/char*
		std::ifstream inFile;
		inFile.open(fileName);

		std::stringstream strStream;
		strStream << inFile.rdbuf();
		String jsonAsString = strStream.str();

		std::cout << jsonAsString << std::endl;
		inFile.close();
		const char* jsonAsChar = jsonAsString.c_str();

		try
		{
			// Load the datablocks (which also creates them)
			hlmsJson.loadMaterials(fileName, jsonAsChar); // The fileName is only used for logging and has no purpose
		}
		
		catch (Exception e)
		{
			LogManager::getSingleton().logMessage("JsonExportPlugin::loadMaterial(); Error while processing the materials\n");
			return false;
		}
		
		return true;
	}


	//****************************************************************************/
	void JsonExportPlugin::destroyAllDatablocks(HlmsEditorPluginData* data)
	{
		// Get the datablock from the current item and remove it
		Item* item = data->mInItem;
		HlmsDatablock* itemDatablock = data->mInOutCurrenDatablock;
		Root* root = Root::getSingletonPtr();
		HlmsManager* hlmsManager = root->getHlmsManager();
		HlmsPbs* hlmsPbs = static_cast<HlmsPbs*>(hlmsManager->getHlms(HLMS_PBS));
		HlmsUnlit* hlmsUnlit = static_cast<HlmsUnlit*>(hlmsManager->getHlms(HLMS_UNLIT));

		if (itemDatablock != hlmsUnlit->getDefaultDatablock())
			item->setDatablock(hlmsUnlit->getDefaultDatablock());
		else
			if (itemDatablock != hlmsPbs->getDefaultDatablock())
				item->setDatablock(hlmsPbs->getDefaultDatablock());
			else
				item->setDatablock("[Default]");

		// Iterate through all pbs datablocks and remove them
		Hlms::HlmsDatablockMap::const_iterator itorPbs = hlmsPbs->getDatablockMap().begin();
		Hlms::HlmsDatablockMap::const_iterator endPbs = hlmsPbs->getDatablockMap().end();
		HlmsPbsDatablock* pbsDatablock;
		while (itorPbs != endPbs)
		{
			pbsDatablock = static_cast<HlmsPbsDatablock*>(itorPbs->second.datablock);
			if (pbsDatablock != hlmsPbs->getDefaultDatablock() &&
				pbsDatablock != hlmsUnlit->getDefaultDatablock())
			{
				hlmsPbs->destroyDatablock(pbsDatablock->getName());
				itorPbs = hlmsPbs->getDatablockMap().begin(); // Start from the beginning again
			}
			else
				++itorPbs;
		}

		// Iterate through all unlit datablocks and remove them
		Hlms::HlmsDatablockMap::const_iterator itorUnlit = hlmsUnlit->getDatablockMap().begin();
		Hlms::HlmsDatablockMap::const_iterator endUnlit = hlmsUnlit->getDatablockMap().end();
		HlmsUnlitDatablock* unlitDatablock;
		while (itorUnlit != endUnlit)
		{
			unlitDatablock = static_cast<HlmsUnlitDatablock*>(itorUnlit->second.datablock);
			if (unlitDatablock != hlmsPbs->getDefaultDatablock() &&
				unlitDatablock != hlmsUnlit->getDefaultDatablock())
			{
				hlmsUnlit->destroyDatablock(unlitDatablock->getName());
				itorUnlit = hlmsUnlit->getDatablockMap().begin(); // Start from the beginning again
			}
			else
				++itorUnlit;
		}
	}
}
