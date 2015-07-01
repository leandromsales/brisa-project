# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright 2007-2008, Brisa Team <brisa-develop@garage.maemo.org>

""" Device-side event related classes.
"""
import uuid

from xml.etree import ElementTree
from datetime import datetime

from brisa.core import log, reactor, webserver
from brisa.core.network import http_call
from brisa.core.threaded_call import run_async_call, ThreadedCall
from brisa.utils.looping_call import LoopingCall


class EventManager(webserver.CustomResource):
    """ Listen for event subscribe and unsubscribe at device services.
    It also manager the list of the control points interested at service
    eventing.
    """

    def __init__(self, service):
        webserver.CustomResource.__init__(self)
        #FIXME - This list is not thread safe
        self.subscribers = []
        self.publisher = service

    def get_render(self, uri, params):
        return self

    def render(self, uri, request, response):
        compressed_headers = {}
        for k, v in request.headers.items():
            if not v:
                v = ""
            compressed_headers[k.lower()] = v.lower().strip()

        if request.method.upper() == "SUBSCRIBE":
            if not 'sid' in compressed_headers:
                return self._subscribe(uri, request,
                            response, compressed_headers)
            else:
                return self._renew(uri, request, response, compressed_headers)
        elif request.method.upper() == "UNSUBSCRIBE":
            return self._unsubscribe(uri, request,
                            response, compressed_headers)
        else:
            #TODO: error???
            return None

    def _subscribe(self, uri, request, response, compressed_headers):
        request_status = self._validate_subscribe_request(request,
                                                          compressed_headers)

        if request_status == 200:
            timeout = int(compressed_headers['timeout'].split("-")[-1])
            subscriber = Subscriber(timeout,
                                compressed_headers['callback'][1:-1],
                                request.server_protocol)
            response_body = self._get_subscribe_response(request,
                                                         response, subscriber)
            self.subscribers.append(subscriber)
            EventMessage(subscriber, self.publisher._eventing_variables, 1, "")
            t_call = ThreadedCall(self._auto_remove_subscriber, None, None,
                                  None, None, timeout * 1.1,
                                  "uuid:" + str(subscriber.subscription_id))
            t_call.start()
            return response_body
        else:
            return self._build_error(request_status, request, response)

    def _validate_subscribe_request(self, request, compressed_headers):
        #TODO: verify if the callback url is a valid one
        if not 'callback' in compressed_headers:
            return 412

        if (not 'nt' in compressed_headers) or \
           (compressed_headers['nt'] != "upnp:event"):
            return 412

        #TODO: Put the verification of error 5xx

        # No errors
        return 200

    def _get_subscribe_response(self, request, response_obj, subscriber):
        response_obj.status = 200

        #TODO: date
        response_obj.headers["DATE"] = ''
        response_obj.headers["SERVER"] = 'BRisa UPnP Framework'
        response_obj.headers["SID"] = 'uuid:' + str(subscriber.subscription_id)
        response_obj.headers["CONTENT-LENGTH"] = 0
        response_obj.headers["TIMEOUT"] = 'Second-' \
                        + str(subscriber.subscription_duration)
        response_obj.body = ""

        return response_obj.body

    def _renew(self, uri, request, response, compressed_headers):
        request_status = self._validate_renew_request(request,
                                                      compressed_headers)

        if request_status == 200:
            subs = self._find_subscriber(compressed_headers['sid'])
            timeout = compressed_headers['timeout']
            subs.subscription_duration = int(timeout.split("-")[-1])
            subs.timestamp = datetime.now()

            return self._get_subscribe_response(request, response, subs)
        else:
            return self._build_error(request_status, request, response)

    def _validate_renew_request(self, request, compressed_headers):
        if 'callback' in compressed_headers or 'nt' in compressed_headers:
            return 400

        if not self._find_subscriber(compressed_headers['sid']):
            return 412

        #TODO: Put the verification of error 5xx

        # No errors
        return 200

    def _auto_remove_subscriber(self, sid):
        subscriber = self._find_subscriber(sid)
        if not subscriber:
            #Already unsubscribe
            return

        time_delta = datetime.now() - subscriber.timestamp
        if time_delta.seconds > subscriber.subscription_duration:
            self.subscribers.remove(subscriber)
        else:
            subscriber.timestamp = datetime.now()
            t_call = ThreadedCall(self._auto_remove_subscriber, None, None,
                 None, None, subscriber.subscription_duration * 1.1,
                 sid)
            t_call.start()

    def _unsubscribe(self, uri, request, response, compressed_headers):
        request_status = self._validate_unsubscribe_request(request,
                                                            compressed_headers)

        if request_status == 200:
            # Remove the subscriber
            subs = self._find_subscriber(compressed_headers['sid'])
            if subs:
                subs.stop()
                self.subscribers.remove(subs)

            response.status = 200
            response.body = ""
            return response.body
        else:
            return self._build_error(request_status, request, response)

    def _validate_unsubscribe_request(self, request, compressed_headers):

        if not 'sid' in compressed_headers:
            return 412

        if 'callback' in compressed_headers or 'nt' in compressed_headers:
            return 400

        if not self._find_subscriber(compressed_headers['sid']):
            return 412

        # No errors
        return 200

    def _build_error(self, request, response_obj, status):
        response = soap.build_soap_error(status)

        response_obj.status = 500

        if self.encoding is not None:
            mime_type = 'text/xml; charset="%s"' % self.encoding
        else:
            mime_type = "text/xml"
        response_obj.headers["Content-type"] = mime_type
        response_obj.headers["Content-length"] = str(len(response))
        response_obj.headers["EXT"] = ''
        response_obj.body = response
        return response

    def _find_subscriber(self, sid):
        for subscribe in self.subscribers:
            if str(subscribe.subscription_id) == sid[5:]:
                return subscribe
        return None

    def update_variable(self, name, value):
        for subs in self.subscribers:
            subs.update_variable(name, value)


class Subscriber:

    def __init__(self, subscription_duration, delivery_url, http_version):
        self.subscription_id = uuid.uuid4()
        self.delivery_url = delivery_url
        self.event_key = 0
        self.subscription_duration = subscription_duration
        self.http_version = http_version
        self.eventing_variables = {}
        self.timestamp = datetime.now()
        self.looping_call = LoopingCall(self._send_variables)
        raector.add_after_stop_callback(self.looping_call.stop)
        self.looping_call.start(10, False)

    def event_key_increment(self):
        self.event_key += 1
        if self.event_key > 4294967295:
            self.event_key = 1

    def update_variable(self, name, value):
        if name in self.eventing_variables.keys() and \
            self.eventing_variables[name] != value:

            self._send_variables()
            self.eventing_variables[name] = value
            return

        self.eventing_variables[name] = value

    def _send_variables(self):
        if self.eventing_variables:
            EventMessage(self, self.eventing_variables, 0, "")
            self.eventing_variables = {}

    def stop(self):
        # When called stop() manually, remove the before stop callback
        reactor.rem_after_stop_callback(self.looping_call.stop)
        self.looping_call.stop()


class EventMessage:
    """ Wrapper for an event message.
    """

    def __init__(self, subscriber, variables, event_delay, cargo):
        """ Constructor for the EventMessage class.

        @param subscriber: subscriber that will receive the message
        @param variables: variables of the event
        @param event_delay: delay to wait before sending the event
        @param cargo: callback parameters

        @type subscriber: Subscriber
        @type variables: dict
        @type event_delay: float
        """
        log.debug("event message")
        self.cargo = cargo

        headers = {}
        headers["HOST"] = subscriber.delivery_url
        headers["CONTENT-TYPE"] = 'text/xml'
        headers["NT"] = 'upnp:event'
        headers["NTS"] = 'upnp:propchange'
        headers["SID"] = "uuid:" + str(subscriber.subscription_id)
        headers["SEQ"] = str(subscriber.event_key)
        subscriber.event_key_increment()

        event_body = self._build_message_body(variables)

        headers["CONTENT-LENGTH"] = str(len(event_body))

        run_async_call(http_call, success_callback=self.response,
                       error_callback=self.error, delay=event_delay,
                       method='NOTIFY', url=subscriber.delivery_url,
                       body=event_body, headers=headers)

    def _build_message_body(self, variables):
        property_set = ElementTree.Element("e:propertyset")
        property_set.attrib.update({'xmlns:e':
                                "urn:schemas-upnp-org:event-1-0"})

        type_map = {str: 'xsd:string',
                    unicode: 'xsd:string',
                    int: 'xsd:int',
                    long: 'xsd:int',
                    float: 'xsd:float',
                    bool: 'xsd:boolean'}

        for var_name, var_val in variables.items():
            property = ElementTree.SubElement(property_set, "e:property")

            var_type = type_map[type(var_val)]
            if var_type == 'xsd:string' and type(var_val) == unicode:
                var_val = var_val.encode('utf-8')
            if var_type == 'xsd:int' or var_type == 'xsd:float':
                var_val = str(var_val)
            if var_type == 'xsd:boolean':
                var_val = '1' if var_val else '0'

            e = ElementTree.SubElement(property, var_name)
            e.text = var_val

        preamble = """<?xml version="1.0" encoding="utf-8"?>"""
        return '%s%s' % (preamble, ElementTree.tostring(property_set, 'utf-8'))

    def error(self, cargo, error):
        """ Callback for receiving an error.

        @param cargo: callback parameters passed at construction
        @param error: exception raised

        @type error: Exception

        @rtype: boolean
        """
        log.debug("error", error)
        return True

    def response(self, http_response, cargo):
        """ Callback for receiving the HTTP response on a successful HTTP call.

        @param http_response: response object
        @param cargo: callback parameters passed at construction

        @type http_response: HTTPResponse

        @rtype: boolean
        """
        log.debug("response")
        return True
