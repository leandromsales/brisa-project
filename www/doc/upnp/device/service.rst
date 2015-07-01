:mod:`service` --- Service classes
==============================================

.. module: service
    :synopsis: Service related classes and examples

Implementing a simple service.
-------------------------------
There is two ways of implement a service: using your own scpd.xml file or
programatically describes it.

Implementing a service with a scpd.xml file.
---------------------------------------------
Write your scpd.xml file.

.. literalinclude:: ../../example_code/myservice-scpd.xml

Now, create your service class and inherits from Service class. You will need
to specify the service name, the service type, the scpd.xml file and implement
a function to run a service action. The function name must begin with "soap" and
ends with the action name::

	from brisa.upnp.device.service import Service
	
	service_name = 'MyService'
	service_type = 'urn:schemas-upnp-org:service:MyService:1'
	
	class MyService(Service):

	    def __init__(self):
	        Service.__init__(self, service_name, service_type, url_base='', scpd_xml_filepath='/path/to/file/myservice-scpd.xml'))
	
	    def soap_MyMethod(self, *args, **kwargs):
            # Pay attention to the case sensitive arguments used here
            # and in the xml file you create for the service
            inArg = kwargs['TextIn']
            return {'TextOut': inArg + "Out!!"}

Implementing a service without a scpd.xml file.
------------------------------------------------
You will need to specify your service definition programatically. Don't forget
to specify at least one state variable and to set the "run_function" at each action.
The "run_function" can have any name at this situation; it doesn't need to have the
action name::

	from brisa.upnp.device.service import Service
	from brisa.upnp.device.action import Action, Argument
	from brisa.upnp.device.service import StateVariable
	
	service_name = 'MyService'
	service_type = 'urn:schemas-upnp-org:service:MyService:1'
	
	def MyMethod(*args, **kwargs):
	    # Pay attention to the case sensitive arguments used here
	    # and in the xml file you create for the service
	    inArg = kwargs['TextIn']
	    return {'TextOut': inArg + "Out!!"}
	
	class MyService(Service):
	 
	    def __init__(self):
	        Service.__init__(self, service_name, service_type, '')
	
	        varIn = StateVariable(service=self, name="A_ARG_TYPE_Textin",
	                              send_events=True, data_type="string")
	        varOut = StateVariable(service=self, name="A_ARG_TYPE_Textout",
	                               send_events=True, data_type="string")
	        self.add_state_variable(varIn)
	        self.add_state_variable(varOut)
	
	        argIn = Argument(arg_name="TextIn", arg_direction=Argument.IN, arg_state_var=varIn)
	        argOut = Argument(arg_name="TextOut", arg_direction=Argument.OUT, arg_state_var=varOut)
	
	        actionMyMethod = Action(service=self, name="MyMethod", arguments=[argIn, argOut])
	        actionMyMethod.run_function = MyMethod
	        self.add_action(actionMyMethod)