package jbrisa.upnp.device.core;

import java.io.File;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

import jbrisa.upnp.device.event.BrisaEventSender;
import jbrisa.upnp.device.event.BrisaEventSubscription;
import jbrisa.upnp.device.model.BrisaAction;
import jbrisa.upnp.device.model.BrisaArgument;
import jbrisa.upnp.device.model.BrisaStateVariable;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.Namespace;
import org.jdom.input.SAXBuilder;


/**
 * Service do BRisa
 * 
 * @author wendell
 * 
 */
public class BrisaService implements Observer {

	private String udn;
	private String serviceId;
	private String serviceType;
	private String scpdURL;
	private String eventSubURL;
	private String controlURL;
	private ArrayList<BrisaAction> actionList;
	private ArrayList<BrisaStateVariable> stateVariableList;
	private Integer nextMulticastSeq;
	private Integer initialSeq;
	private ArrayList<BrisaEventSubscription> eventSubscriptions;

	private Document scdpDocument;
	private String scpdPath;
	
	public BrisaService(String serviceId, String serviceType) {
		
		this.initialSeq = 0;
		this.nextMulticastSeq = initialSeq;
		this.serviceId = serviceId;
		this.serviceType = serviceType;
		this.eventSubscriptions = new ArrayList<BrisaEventSubscription>();
		//this.eventSubURL = eventSubURL;
		//this.controlURL = controlURL;
		
		//adicionando / ao scpdUrl;
		/*
		if (!auxscdpURL.startsWith("/")) {
			this.scpdURL = "/" + auxscdpURL;
		}else{
			this.scpdURL = auxscdpURL;
		}
		*/
		
		String idaux = serviceId.trim().replace(" ", "");
		
		this.eventSubURL = "/"+idaux+"/eventSub";
		this.controlURL = "/"+idaux+"/control";
		this.scpdURL = "/"+serviceId+"-scpd.xml";
	}

	public BrisaService(String serviceType) {

		this.initialSeq = 0;
		this.nextMulticastSeq = initialSeq;

		this.serviceType = serviceType;
	}
	
	public void setDescriptonFile(Document scdpDocument) {
		this.scdpDocument = scdpDocument;
		parseDescriptionFile();
	}
	
	public void setDescriptonFile(String scdpString) {
		this.scpdPath = scdpString;
		parseDescriptionFile();
	}
	
	public void addAction(String name) {
		this.addAction(new BrisaAction(name));
	}

	public void addAction(BrisaAction action) {
		// must not add actions named as an already added action
		if (getAction(action.getName()) == null)
			this.actionList.add(action);
	}

	public BrisaAction getAction(String name) {
		for (BrisaAction action : this.actionList) {
			if (action.getName().equalsIgnoreCase(name)) {
				return action;
			}
		}
		return null;
	}

	public void clearActionList() {
		this.actionList.clear();
	}

	public void addStateVariable(BrisaStateVariable stateVariable) {
		this.stateVariableList.add(stateVariable);
		if (stateVariable.multicastEvents()) {
			stateVariable.setNextMulticastSeq(this.nextMulticastSeq);
			//stateVariable.setMulticastUsn(this.serviceType);
			//stateVariable.setMulticastUdn(this.udn);
			//stateVariable.setMulticastSvcid(this.serviceId);
		}
	}

	public BrisaStateVariable getStateVariable(String name) {
		for (BrisaStateVariable v : this.stateVariableList) {
			if (v.getName().equals(name)) {
				return v;
			}
		}
		return null;
	}

	public ArrayList<BrisaStateVariable> getStateVariableList() {
		// TODO Auto-generated method stub
		return stateVariableList;
	}

	public ArrayList<BrisaEventSubscription> getEventSubscriptions() {
		return eventSubscriptions;
	}
	
	public void addEventSubscription(BrisaEventSubscription eventSub) {
		
		//checking if the subscription already exists;
		Boolean found = false;
		
		for (BrisaEventSubscription eventSubAux : getEventSubscriptions()) {
			
			for (String callback1 : eventSubAux.getCallbackUrls()) {
				
				for (String callback2 : eventSub.getCallbackUrls()) {
					
					if(callback1.equals(callback2))
					{
						eventSubAux.renew(eventSub.getTimeout());
						found = true;
					}
				}
			}
		}
		//if doesnt exists in services subscription list;
		if(!found)
			this.eventSubscriptions.add(eventSub);
	}
	
	//================================================================== 
	//PRIVATE METHODS===================================================
	//==================================================================
	
	/**
	 * Parse do aquivo SCDP
	 */
	private void parseDescriptionFile() {
		
		// GET THE SCPDURL FROM THE CURRENT SERVICE;
		Element rootService;

		if (scdpDocument != null) {
			rootService = scdpDocument.getRootElement();
		} else {
			
			String scdpFilePath = this.scpdPath;
			if (scdpFilePath.startsWith("/")) {
				scdpFilePath = scdpFilePath.replaceFirst("/", "");
			}
			scdpDocument = this.getServiceXmlElement(scdpFilePath);
			rootService = scdpDocument.getRootElement();
		}

		// INIT THE ACTION LIST FROM THE SERVICE;
		this.actionList = new ArrayList<BrisaAction>();

		// IF THE XML VAS LOADED SUCESSFULLY;
		if (rootService != null) {

			for (Object auxAction : rootService.getChild("actionList",
					rootService.getNamespace()).getChildren()) {

				this.actionList.add(getActionBySSDPUrl(auxAction,
						rootService.getNamespace()));
			}

			this.stateVariableList = new ArrayList<BrisaStateVariable>();

			for (Object auxStateVariable : rootService.getChild(
					"serviceStateTable", rootService.getNamespace())
					.getChildren()) {

				// ADD STATE VARIABLE TO SERVICE STATE TABLE;
				this.stateVariableList.add(getStateVariableBySSDPUrl(
						auxStateVariable, rootService.getNamespace()));
			}
			
			addListenerToStateVariableList();
		}
	}

	private Document getServiceXmlElement(String location) {

		SAXBuilder builder = new SAXBuilder();

		try {
			Document documento = builder.build(new File(location));

			BrisaLogger.logger.info("Xml service from " + location
					+ " loaded sucessfully");

			return documento;

		} catch (Exception e) {
			BrisaLogger.logger
					.warning("Xml from device location wasn`t loaded sucessfully. "
							+ "Service won`t be created. ");
			BrisaLogger.logger.warning(e.getMessage());
			e.printStackTrace();
		}
		return null;
	}

	private BrisaAction getActionBySSDPUrl(Object auxAction, Namespace namespace) {

		// NEW ACTION
		BrisaAction newAction = new BrisaAction();
		Element actionXml = (Element) auxAction;

		// NEW ACTION NAME
		newAction.setName(XmlManager.getXmlNodeValue(actionXml, "name"));

		// NEW ACTION ARGUMENT LIST
		newAction.setArgumentList(new ArrayList<BrisaArgument>());

		try {

			for (Object auxArgument : actionXml.getChild("argumentList",
					namespace).getChildren()) {

				Element argumentXml = (Element) auxArgument;

				BrisaArgument argument = new BrisaArgument();
				argument.setDirection(XmlManager.getXmlNodeValue(argumentXml,
						"direction"));
				argument.setName(XmlManager
						.getXmlNodeValue(argumentXml, "name"));

				argument.setRelatedStateVariable(XmlManager.getXmlNodeValue(
						argumentXml, "relatedStateVariable"));

				argument.setValue(XmlManager.getXmlNodeValue(argumentXml,
						"retval"));

				newAction.getArgumentList().add(argument);
			}

		} catch (Exception e) {
			BrisaLogger.logger.warning("No argument list found for the action "
					+ newAction.getName());
		}

		return newAction;
	}

	private BrisaStateVariable getStateVariableBySSDPUrl(
			Object auxStateVariable, Namespace namespace) {

		// NEW STATE VARIABLE
		BrisaStateVariable newStateVariable = new BrisaStateVariable();
		Element stateVariableXml = (Element) auxStateVariable;

		newStateVariable.setStrSendEvents(stateVariableXml
				.getAttributeValue("sendEvents"));
		newStateVariable.setStrMulticast(stateVariableXml
				.getAttributeValue("multicast"));

		// NEW STATE VARIABLE ATTRIBUTES
		newStateVariable.setName(XmlManager.getXmlNodeValue(stateVariableXml,
				"name"));
		newStateVariable.setDefaultValue(XmlManager.getXmlNodeValue(
				stateVariableXml, "defaultValue"));
		newStateVariable.setDataType(XmlManager.getXmlNodeValue(
				stateVariableXml, "dataType"));

		Element allowedValueRangeXml = stateVariableXml.getChild(
				"allowedValueRange", namespace);

		//
		if (allowedValueRangeXml != null) {
			newStateVariable.setMaximum(XmlManager.getXmlNodeValue(
					allowedValueRangeXml, "maximum"));
			newStateVariable.setMinimum(XmlManager.getXmlNodeValue(
					allowedValueRangeXml, "minimum"));
			newStateVariable.setStep(XmlManager.getXmlNodeValue(
					allowedValueRangeXml, "step"));
		}

		// NEW STATE VARIABLE ALLOWED VALUES
		ArrayList<String> arrayValues = new ArrayList<String>();
		Element allowedValueList = stateVariableXml.getChild(
				"allowedValueList", namespace);

		if (allowedValueList != null) {
			for (Object auxAllowedValue : allowedValueList.getChildren()) {

				String allowedValue = ((Element) auxAllowedValue).getValue();
				arrayValues.add(allowedValue);
			}
			newStateVariable.setAllowedValueList(arrayValues);
		}

		return newStateVariable;
	}
	
	private void addListenerToStateVariableList(){
		
		for (BrisaStateVariable v : stateVariableList) {
			if (v.multicastEvents()) {
				v.addObserver(this);
			}
		}
	}
	
	/**
	 * States Variables sending events
	 */
	@Override
	public void update(Observable o, Object arg) {

		if (o instanceof BrisaStateVariable) {
			BrisaStateVariable v = (BrisaStateVariable) o;

			if (v.multicastEvents()) {

				BrisaLogger.logger.info("Creating Multicast Message");
				
				String bodyMessage = "<?xml version=\"1.0\"?><e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">";

				bodyMessage += "<e:property>";
				bodyMessage += "<" + v.getName() + ">"
						+ v.getValue() + 
						"</"+ v.getName() + ">";
				bodyMessage += "</e:property>";
				bodyMessage += "</e:propertyset>";

				String message = "NOTIFY * HTTP/1.0\r\n"
						+ "HOST: 239.255.255.246:7900\r\n"
						+ "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n" 
						+ "USN: "+this.serviceType+"\r\n" 
						+ "SVCID: "+this.serviceId+"\r\n"
						+ "NT: upnp:event\r\n"
						+ "NTS: upnp:propchange\r\n" 
						+ "SEQ: "+v.getNextMulticastSeq()+"\r\n"
						+ "LVL: upnp:/general\r\n" 
						+ "BOOTID.UPNP.ORG: "+0+"\r\n"
						+ "CONTENT-LENGTH: "+bodyMessage.length() + "\r\n"
						+ "\r\n";

				message += bodyMessage;
				
				BrisaEventSender.sendMulticastMessage(message);
				
				BrisaLogger.logger.info("Multicast Message Sent");

			}
			
			if(v.unicastEvents())
			{
				for (BrisaEventSubscription sub : this.getEventSubscriptions()) {
					
					for (String callback : sub.getCallbackUrls()) {
						
						if(!sub.hasExpired())
						{
							String bodyMessage = "<?xml version=\"1.0\"?><e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">";

							bodyMessage += "<e:property>";
							bodyMessage += "<" + v.getName() + ">"
									+ v.getValue() + 
									"</"+ v.getName() + ">";
							bodyMessage += "</e:property>";
							bodyMessage += "</e:propertyset>";

							String message = "NOTIFY "+ callback +" HTTP/1.1\r\n"
									+ "HOST: "+ callback +"\r\n"
									+ "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
									+ "NT: upnp:event\r\n"
									+ "NTS: upnp:propchange\r\n"
									+ "SID: "+sub.getSid()+"\r\n" 
									+ "SEQ: "+sub.getNextSeq()+"\r\n"
									+ "CONTENT-LENGTH: "+bodyMessage.length() + "\r\n" 
									+ "\r\n";

							message += bodyMessage;
							
							String ip = callback.replace("http://", "").split(":")[0];
							int port = Integer.parseInt(callback.replace("http://", "").split(":")[1]);
							
							BrisaEventSender.sendUnicastMessage(message, ip, port);

						}
					}
				}
			}
		}
	}
	
	//getters and setters
	
	public String getScpdURL() {
		return scpdURL;
	}
	
	public ArrayList<BrisaAction> getActionList() {
		return actionList;
	}
	
	public Document getScdpDocument() {
		return scdpDocument;
	}
	
	public String getUdn() {
		return udn;
	}

	public void setUdn(String udn) {
		this.udn = udn;
	}

	public String getServiceId() {
		return serviceId;
	}

	public void setServiceId(String serviceId) {
		this.serviceId = serviceId;
	}

	public String getServiceType() {
		return serviceType;
	}

	public void setServiceType(String serviceType) {
		this.serviceType = serviceType;
	}

	public String getEventSubURL() {
		return eventSubURL;
	}

	public void setEventSubURL(String eventSubURL) {
		this.eventSubURL = eventSubURL;
	}

	public String getControlURL() {
		return controlURL;
	}

	public void setControlURL(String controlURL) {
		this.controlURL = controlURL;
	}
}
