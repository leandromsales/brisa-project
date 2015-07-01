# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

from datetime import datetime
from brisa import log

from brisa.services.cds.plugin import Plugin
from brisa.utils import properties

from youtube_api import YouTubeClient
from youtube_dl import get_real_video_url

from brisa.upnp.didl.didl_lite import VideoBroadcast
from brisa import config

youtube_video_url = config.get_parameter('youtube', 'videourl')


class YouTubeItem(VideoBroadcast):

    protocolInfo = 'http-get:*:video/flv:*'

    def __init__(self, id, parent_container_id, namespace, title, description,
                 duration, date, author, rating):
        VideoBroadcast.__init__(self, id,
                                parent_container_id=parent_container_id,
                                namespace=namespace,
                                date=date,
                                author=author,
                                rating=rating,
                                duration=duration,
                                title=title,
                                name=title,
                                description=description)
        # TODO: put more youtube rest attributes
        self._uri = get_real_video_url("%s%s" % (youtube_video_url, id))

    def _gen_uri(self):
        return self._uri


class YouTubePlugin(Plugin):
    id = "7"
    name = 'youtube'
    usage = config.get_parameter_bool('youtube', 'usage')
    videos = {}

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)

    def _register(self):
        self.ytcontainer = self.plugin_manager.root_plugin.\
                                        add_container("YouTube", self.id, "5",
                                                      self)

    def load(self):
        self._register()
        yt = YouTubeClient('ngR1Q8w0OEk')
        username = config.get_parameter('youtube', 'username')
        for video in yt.list_by_user(username):
            video_info = yt.get_details(video['id'])
            date = datetime.fromtimestamp(int(video_info['upload_time'])).\
                                                        strftime("%Y-%m-%d")
            self.add_item(video['id'], self.ytcontainer.id,
                          video_info['title'], video_info['description'],
                          video_info['length_seconds'], date,
                          video_info['author'], video_info['rating_avg'])

    def add_item(self, video_id, parent_id, title, description, duration,
                 date, author, rating):
        item = YouTubeItem(video_id, parent_id, self.name, title, description,
                           duration, date, author, rating)
        self.videos[video_id] = item

    def browse(self, id, browse_flag, filter, starting_index,
               requested_count, sort_criteria):
        if browse_flag == 'BrowseMetadata' and id != self.id:
            return [self.videos[id]]
        else:
            return self.videos.values()
