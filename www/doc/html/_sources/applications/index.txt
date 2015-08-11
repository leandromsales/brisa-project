.. _applications-index:

##############
 Applications
##############

==============
 DBus Support
==============

Both BRisa Media Server and BRisa Media Renderer applications support DBus,
allowing them to integrate with a wide range of media systems. In this section
we specify both interfaces.


BRisa Media Server
==================

DBus specification
------------------

    * Object path: ``/br/edu/ufcg/embedded/brisa/MediaServer``
    * Interface: ``br.edu.ufcg.embedded.brisa.MediaServer``


Available Methods
-----------------

.. note::

   A watched folder is a folder whose files are being watched, that is, are
   served by the media server.

.. note::

   * DMS = DLNA Media Server
   * CDS = Content Directory Subsystem or Service
   * CM = Connection Manager service


.. function:: dms_halt():

    Halts the media server.

.. function:: dms_get_server_info():

    Returns a 7-tuple containing information about the device. The
    format is ``(device version, brisa framework version, application
    version, server name, xbox compatibility, logging level, logging
    output)``.

.. function:: dms_reload_config():

   Reloads the configuration from the database.

.. function:: dms_save_config():

   Saves the runtime configuration.

.. function:: dms_set_xbox_compatibility(enable=True):

   Enables or disables the XBox compatibility.

.. function:: dms_cds_list_watched_audio_folders():

   Returns a list with the watched audio folders.

.. function:: dms_cds_list_watched_video_folders():

   Returns a list with the watched video folders.

.. function:: dms_cds_list_watched_picture_folders():

   Returns a list of the watched picture folders.

.. function:: dms_cds_remove_watched_audio_folder(folder):

   Removes an audio folder from being watched.

.. function:: dms_cds_remove_watched_video_folder(folder):

   Removes a video folder from being watched.

.. function:: dms_cds_remove_watched_picture_folder(folder):

   Removes a picture folder from being watched.

.. function:: dms_cds_add_watch_audio_folder(folder):

   Add an audio folder to be watched.

.. function:: dms_cds_add_watch_video_folder(folder):

   Add a video folder to be watched.

.. function:: dms_cds_add_watch_picture_folder(folder):

   Add a picture folder to be watched.

.. function:: dms_cds_rescan_folders():

   Rescans all folders for changes (added or removed files).

.. function:: dms_cds_browse(object_id, browse_flag, filter, starting_index, requested_count, sort_criteria):

   Browses the CDS. Returns a DIDL XML string containing the result entries.
   The module **brisa.upnp.didl.didl_lite.Element** can be used for converting
   it into python objects.

.. function:: dms_cds_search(container_id, search_criteria, filter, starting_index, requested_count, sort_criteria):

   Searches a CDS container for entries. Returns a DIDL XML string containing
   the result entries. The module **brisa.upnp.didl.didl_lite.Element** can be used for
   converting it into python objects.

.. function:: dms_cds_get_search_caps():

   Retrieves the search capabilities of the CDS service.

.. function:: dms_cds_get_sort_caps():

   Retrieves the sort capabilities of the CDS service.

.. function:: dms_cds_get_system_update_id():

   Retrieves the CDS system update ID.

.. function:: dms_cm_get_protocol_info():

   Retrieves the CM protocol info.

.. function:: dms_cm_get_current_connection_ids():

   Retrieves CM current connection IDs.

.. function:: dms_cm_get_current_connection_info():

   Retrieves CM current connection info.


BRisa Media Renderer
====================

DBus specification
------------------

    * Object path: ``/br/edu/ufcg/embedded/brisa/MediaRenderer``
    * Interface: ``br.edu.ufcg.embedded.brisa.MediaRenderer``


Available Methods
-----------------

.. note::

    * DMR = DLNA Media Renderer
    * AVT = AVTransport service, where AV stands for Audio/Video
    * RC = Render Control service
    * CM = Connnection Manager service

.. function:: dmr_halt():

    Halts the media renderer.

.. function:: dmr_get_renderer_info():

    Returns a 6-tuple containing information about the device. The
    format is ``(device version, brisa framework version, application
    version, renderer name, logging level, logging output)``.

.. function:: dmr_avt_set_av_transport_uri(instance_id, current_uri, current_uri_metadata):

   Sets the AVT work URI (the one used for play(), pause(), stop(), and so on).

.. function:: dmr_avt_get_media_info():

   Retrieves AVT media info.

.. function:: dmr_avt_get_media_info_ext():

   Retrieves AVT extended media info.

.. function:: dmr_avt_get_transport_info():

   Retrieves AVT transport info.

.. function:: dmr_avt_get_position_info():

   Retrieves AVT position info.

.. function:: dmr_avt_get_device_caps():

   Retrieves AVT device capabilities.

.. function:: dmr_avt_get_transport_settings():

   Retrieves AVT transport settings.

.. function:: dmr_avt_play():

   Starts the playback of the current URI.

.. function:: dmr_avt_stop():

   Stops the playback.

.. function:: dmr_avt_pause():

   Pauses the playback.

.. function:: dmr_avt_seek(instance_id, unit, target):

   Seeks the playback.

.. function:: dmr_avt_next():

   Plays next track (not implemented).

.. function:: dmr_avt_previous():

   Plays previous track (not implemented).

.. function:: dmr_rc_list_presets():

   Returns a list of RC presets.

.. function:: dmr_rc_select_preset(preset):

   Select a preset for the RC.

.. function:: dmr_rc_get_volume(instance_id, channel):

   Retrieves the volume of a channel.

.. function:: dmr_rc_set_volume(instance_id, channel, desired_volume):

   Sets the volume of a channel.

.. function:: dmr_cm_get_protocol_info():

   Retrieves CM protocol info.

.. function:: dmr_cm_get_current_connection_info():

   Retrieves CM current connection info.

.. function:: dmr_cm_get_current_connection_ids():

   Retrieves CM current connection IDs.

========================================
 Writing plugins for BRisa Media Server
========================================


Introduction
============

BRisa Media Server can be easily extended to serve more content with plugins. We
have a few plugins already implemented which can serve as code example:

 * Flickr: enables access to Flickr pictures, such as user pictures or even featured ones.
 * Youtube: enables access to Youtube user and featured videos.


Examples: :download:`Media Library Plugin <../example_code/media_library_implementation.py>`.


Organization
============

Concerning files, your plugin must be placed in
``$PREFIX/brisa_media_server/plugins/yourplugin``, where $PREFIX is usually
``/usr/lib/python2.5/site-packages`` or
``/usr/local/lib/python2.6/dist-packages`` and so on. On your ``__init__.py`` file
you must import your plugin individual module, so that your plugin class is
visible as subclass of our plugin interface (future plan is to have an automatic
plugin install mechanism).

BRisa Media Server basic directory structure is composed only by a root folder,
which belongs to what we call ``RootPlugin``. When plugins are being loaded,
they can add folders to the root folder or register themselves on other folders.

Interface
=========

Plugins must be implemented using the ``brisa.core.plugin.PluginInterface``
interface.

Attributes that must be set during construction:

 * ``name``: plugin name
 * ``usage``: True if your plugin is supposed to load, False otherwise. This is
              usually loaded from a configuration file.
 * ``has_browse_filter``: can be ``True`` or ``False``. If your plugin
                          implements the browse() function with slicing/sorting
                          capabilities (i.e. it uses ``starting_index``,
                          ``requested_count``, ``sort_criteria`` parameters),
                          this attribute must be ``True``.

Methods that must be implemented (part of the interface):

.. function:: PluginInterface.load():

   Loads the plugin.

.. function:: PluginInterface.unload():

   Unloads the plugin.

.. function:: PluginInterface.browse(object_id, browse_flag, filter, starting_index, requested_count, sort_criteria):

   Browses plugin folders or items. Must return a list of DIDL objects
   (brisa.upnp.didl.didl_lite) representing the browse result.

.. function:: PluginInterface.search(object_id, search_flag, filter, starting_index, requested_count, sort_criteria):

   Searches the plugin folder for items.


If your plugin folder was correctly copied into
``brisa_media_server/plugins/your_plugin`` and on your file
``/your_plugin/__init__.py`` you import the module that contains your plugin
class implementation, then the media server should load it automatically. For
examples on that, please refer to our media_library example, currently on our
`SVN repository <https://garage.maemo.org/svn/brisa>` under ``trunk/app/media-server/src/plugins/media_library``.

Once your plugin is created, python-brisa automatically sets the
``plugin_manager`` attribute of your plugin, which points to our PluginManager.
You will be using the PluginManager for retrieving the RootPlugin, which
contains the root directory, for adding your plugin's own custom folders.


For retrieving the root plugin, consider the following code::

    from brisa.core.plugin import PluginInterface


    class MyPlugin(PluginInterface):

       (implement as the description we gave above)

       def load(self):
          # Retrieve the root plugin
          root_plugin = self.plugin_manager.root_plugin

          audio_container = root_plugin.get_container('Audio')

          if not audio_container:
                # Does not exist yet, add it
                audio_container = root_plugin.add_container('Audio')

          (...)

Specific implementation details of browse and search methods are better
visualized on the media library example, as it uses everything that a plugin can
do. In a nutshell, you should keep a dictionary of containers you added, keys
being the container id attribute and values the container itself. When browsed,
you query on this dictionary, and do the appropriate action that may vary from
a plugin to another.

For instance, on a Flickr plugin we do not have actual files, but links to the
web, so, you can cache the links on your containers (expensive) or just fetch
the links when the browse() function tells it wants the contents of a container.

Plugins can also serve local files, which is also illustrated on the media
library example.
