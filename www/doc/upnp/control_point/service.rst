:mod:`service` --- Service classes
==============================================

.. module: service
    :synopsis: Service related classes and examples

Subscribe for unicast eventing.
--------------------------------
It's very simple to subscribe for service unicast eventing. You need to
select the service object that you want to subscribe and call
"event_subscribe" method.

There is a special parameter at "event_subscribe" method: auto_renew.
If true, the framework will automatically renew the subscription before it 
expires. If false, the program need to call "event_renew" method before the
subscription timeout. The renew callback will be used when auto_renew is true::

	class ControlPoint(ControlPoint):
	
	    def __init__(self):
	        ControlPoint.__init__(self)
	
	    def subscribe(self):
	        service = self._select_service()
	        service.event_subscribe(self.event_host, self._event_subscribe_callback, None, True, self._event_renew_callback)
	        service.subscribe_for_variable("A_ARG_TYPE_Textin", self._event_callback)

	    def _select_service(self):
	    	# Implement your way of find the service that you want
	        return service	
	
	    def _event_subscribe_callback(self, cargo, subscription_id, timeout):
	        print "Event subscribe done!"
	        print 'Subscription ID: ' + str(subscription_id)
	        print 'Timeout: ' + str(timeout)
	
	    def _event_renew_callback(self, cargo, subscription_id, timeout):
	        print "Event renew done!"
	        print 'Subscription ID: ' + str(subscription_id)
	        print 'Timeout: ' + str(timeout)

	    def _event_callback(self, name, value):
	        print "Event message!"
	        print 'State variable:', name
	        print 'Variable value:', value

Unsubscribe for unicast eventing.
----------------------------------
In order to unsubscribe for service unicast eventing, you need to
select the service object that you want to unsubscribe and call
"event_unsubscribe" method::

	    def unsubscribe(self):
	        service = self._select_service()
	        service.event_unsubscribe(self.event_host, self._event_unsubscribe_callback, None)	
	
	    def _event_unsubscribe_callback(self, cargo, old_subscription_id):
	        print "Event unsubscribe done!"
	        print 'Old subscription ID: ' + str(old_subscription_id)
	