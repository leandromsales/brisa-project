import brisa
brisa.__enable_logging__ = False

import logging
from terra.core.log import setLevel
setLevel(logging.DEBUG)

from terra.core.manager import Manager
from terra.ui.base import EdjeWidget
import ecore.x as X

log = logging.getLogger('plugins.shared')

mger = Manager()
OptionsController = mger.get_class("OptionsController")
BaseAudioPlayerController = mger.get_class("Controller/Media/Audio")
BaseVideoPlayerController = mger.get_class("Controller/Media/Video")
BaseImagePlayerController = mger.get_class("Controller/Media/Image")

class VideoPlayerScreen(EdjeWidget):
    def __init__(self, model, parent, xid, theme=None):
        EdjeWidget.__init__(self, parent.evas, "player_video",
                            parent, theme)

        self.parent_view = parent
        self.parent_view.signal_emit("media_area,video", "")
        self.parent_view.part_swallow("player_view/video_throbber",
                                      self.parent_view.throbber)
        self.callback_stop = None
        self.callback_prev = None
        self.callback_next = None
        self.callback_pause = None

        self._setup_x_window(xid)
        self.parent_view.signal_callback_add("transition,out", "*",
                                             self.transition_out_cb)
        self.setup_model(model)

    def _setup_x_window(self, xid):
        X.init()
        self.parent_xid = X.Window_from_xid(xid)
        self._setup_background()
        self.key_cb = X.on_key_down_add(self._key_down_x_wrapper)

    def _setup_background(self):
        self.background = self.parent_view.canvas.Rectangle()
        self.background.color_set(0, 0, 0, 255)
        self.background.geometry_set(*self.parent_xid.geometry_get())
        self.background.hide()

    def setup_model(self, model=None):
        if model:
            self.model = model
            self.parent_view.title_set(self.model.title)

    def transition_out_cb(self, obj, emission, source):
        if self.callback_stop:
            self.callback_stop()

        self.parent_view.video_window = None
        self.parent_view.signal_callback_del("transition,out", "*",
                                             self.transition_out_cb)
        self.parent_view.throbber.stop()
        self.parent_view.part_unswallow(self.parent_view.throbber)
        self.parent_view = None
        self.callback_stop = None
        self.callback_prev = None
        self.callback_next = None
        self.callback_pause = None

    def _key_down_x_wrapper(self, event):
        if self.video and event.window == self.video.input_window:
            self.handle_key_down(event)
        return True

    def handle_key_down(self, event):
        # Handle keys while focus is on video area
        if event.keyname == "F7":
            self.parent_view.raise_volume(5)
        elif event.keyname == "F8":
            self.parent_view.lower_volume(5)
        elif event.keyname == "Left":
            if self.callback_prev:
                self.callback_prev()
        elif event.keyname == "Right":
            if self.callback_next:
                self.callback_next()
        elif event.keyname == "Return":
            if self.callback_pause:
                self.callback_pause()
        else:
            self._parent_widget.handle_key_down(event)
        return True

    def delete(self):
        self.model = None
        self.key_cb.delete()
        EdjeWidget.delete(self)


class AudioPlayerController(BaseAudioPlayerController):
    terra_type = "Controller/Media/Audio/Shared/UPnP"

    def __init__(self, model, canvas, parent):
        BaseAudioPlayerController.__init__(self, model, canvas, parent)

    def setup_model(self, view=True):
        self.setup_view()
        self.view.setup_volume(self._volume)
        self.current = self.model.parent.current
        self.set_uri(self.model.uri)
        self.model.do_load()
        self.play()

    def play(self):
        log.info('Starting play')
        if not self.model.is_remote_renderer:
            BaseAudioPlayerController.play(self)
        else:
            self.model.control_point.av_play(uri=self.model.uri)
            self.view.throbber_stop()
            self.view.set_playing(True)
            self.inform_playing()
        log.debug('Exiting play')

    def pause(self):
        log.info('Starting pause')
        if not self.model.is_remote_renderer:
            BaseAudioPlayerController.pause(self)
        else:
            self.model.control_point.av_pause()
            self.view.set_playing(False)
            self.inform_paused()
        log.debug('Exiting pause')


class VideoPlayerController(BaseVideoPlayerController):
    terra_type = "Controller/Media/Video/Shared/UPnP"

    def __init__(self, model, canvas, parent):
        BaseVideoPlayerController.__init__(self, model, canvas, parent)
        # disable repeat and shuffle for video
        self._repeat = False
        self._shuffle = False
        self.first_load = True
        self.view.callback_theme_changed = self.theme_changed
        self._setup_video_screen()
        self.setup_view()
        self.view.throbber_start()
        self.view.signal_callback_add("transition,in,finished", "*",
                                      self.transition_in_finished_cb)

    def _setup_video_screen(self):
        ee = self.parent.parent.ecore_evas # XXX
        self.video_screen = VideoPlayerScreen(self.model, self.view, ee.window)
        self.video_screen.callback_stop = self.stop
        self.video_screen.callback_prev = self.prev
        self.video_screen.callback_next = self.next
        self.video_screen.callback_pause = self.handle_play_pause


    def setup_model(self, view=True):
        log.info('Setup model')
        self.setup_view()
        self.view.setup_volume(self._volume)
        self.current = self.model.parent.current
        self.set_uri(self.model.uri)
        self.model.do_load()
        self.play()

    def play(self):
        log.info('Starting play')
        if not self.model.is_remote_renderer:
            BaseVideoPlayerController.play(self)
        else:
            self.model.control_point.av_play(uri=self.model.uri)
            self.view.throbber_stop()
            self.view.set_playing(True)
            self.inform_playing()
        log.debug('Exiting play')

    def pause(self):
        log.info('Starting pause')
        if not self.model.is_remote_renderer:
            BaseVideoPlayerController.pause(self)
        else:
            self.model.control_point.av_pause()
            self.view.set_playing(False)
            self.inform_paused()
        log.debug('Exiting pause')

    def transition_in_finished_cb(self, obj, emission, source):
        self.view.evas.render() # XXX: load_atabake() may take too long
                                # XXX: force redraw to clear last frame
                                # XXX: and avoid idler_add()
        self.load_atabake()
        self.setup_interface()
        self.setup_model()
        self.view.signal_callback_del("transition,in,finished", "*",
                                      self.transition_in_finished_cb)

    def options(self):
        self.view.parent.signal_emit("events,block", "")

        self.options_model.position = self.get_position()

        # Define what to do when options close
        if self.state == self.STATE_PLAYING:
            self.pause()
            def leave_cb():
                self.pause()
        else:
            def leave_cb():
                pass

        def cb(controller, *ignored):
            def cb_finished(*ignored):
                self.view.parent.signal_emit("events,unblock", "")
            self.parent.use_options(controller, cb_finished)

        oc = OptionsController(self.options_model, self.evas, self.parent,
                               self, end_callback=cb, leave_callback=leave_cb)



class ImagePlayerController(BaseImagePlayerController):
    terra_type = "Controller/Media/Image/Shared/UPnP"

    def __init__(self, model, canvas, parent):
        BaseImagePlayerController.__init__(self, model, canvas, parent)
