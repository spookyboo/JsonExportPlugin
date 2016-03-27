# Json Export Plugin
Json Export Plugin

HLMS Editor plugins are used to import and/or export Hlms materials.  
The Json Export Plugin is a plugin for the HLMS Editor and exports all materials in the material browser to two json files (one for pbs and one for unlit).
This means that the plugin exports all materials that are part of a project.
This Github repository contains Visual Studio JsonExport.sln / JsonExport.vcxproj files for convenience (do not forget to change the properties to the correct include files, 
because it makes use of both HLMS Editor and Ogre3d include files).
The Json Export plugin makes use of the generic plugin mechanism of Ogre3D.

**Plugin creation:**  
You can use the Json Export plugin code to create your own plugins and import/export materials from/to different sources. It works as a generic Ogre3d plugin, which means that
you have to create a <yourname>Plugin.h / <yourname>Plugin.cpp to implement custom import/export code and a <yourname>Dll.cpp that acts as an entrypoint.
Your custom plugin extends from HlmsEditorPlugin (defined in hlms_editor_plugin.h), which means that it not only has to implement the Ogre3d plugin methods, 
such as getName(), install(), initialise(), shutdown() and uninstall(), but also some functions that are needed to communicate with the HLMS Editor.
The HLMS Editor recognizes the plugin by its name. Function getName() must always return the value "HlmsEditorPlugin".  
  
The HLMS Editor and the plugin communicate by means of the HlmsEditorPluginData object, which is defined in hlms_editor_plugin.h

**Installation:**  
Just add your plugin to the plugins.cfg file (under HLMSEditor/bin) and that's all; the HLMS Editor recognized it if you have followed the rules above.