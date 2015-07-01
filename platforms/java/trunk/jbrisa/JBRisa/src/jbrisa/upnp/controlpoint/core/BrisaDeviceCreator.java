package jbrisa.upnp.controlpoint.core;

import java.util.ArrayList;
import java.util.Observable;

import jbrisa.upnp.controlpoint.model.BrisaCPAction;
import jbrisa.upnp.controlpoint.model.BrisaCPAllowedValueRange;
import jbrisa.upnp.controlpoint.model.BrisaCPArgument;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPService;
import jbrisa.upnp.controlpoint.model.BrisaCPStateVariable;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;

import org.jdom.Element;
import org.jdom.Namespace;


/**
 * 
 * Class used to create devices objects from strings sent by the devices 
 * in a ssdp search message. If the string is invalid or with wrong arguments, 
 * so it's ignored;
 * 
 * @author Thiago Ribeiro
 * 
 */
public class BrisaDeviceCreator extends Observable {

	private BrisaControlPoint b;

	public BrisaDeviceCreator(BrisaControlPoint b) {
		
		this.b = b;
	}
	
	public BrisaCPDevice createDevice(String deviceMSearchAnswer) {
		
		BrisaCPDevice currentDevice = new BrisaCPDevice();

		//GET THE LOCATION AND MAX AGE FROM ANSWER STRING;
		currentDevice.location = XmlManager.getLocationFromSSDP(deviceMSearchAnswer);
		currentDevice.maxAge = XmlManager.getMaxAgeFromSSDP(deviceMSearchAnswer);
		currentDevice.urlBase = XmlManager.getUrlBaseFromLocation(currentDevice.location);

		//IF LOCATION IS VALID;
		if(currentDevice.location != null)
		{
			if(!deviceAlreadyExists(currentDevice.location))
			{
				//LOAD THE LOCATION XML;
				Element deviceXml = XmlManager.getDeviceXmlElement(currentDevice.location);
				
				//IF THE XML EXISTS IN THE LOCATION;
				if (deviceXml != null) {
					
					//FILL ALL DEVICE'S ATTRIBUTES FROM LOADED XML;
					currentDevice.deviceType = XmlManager.getXmlNodeValue(deviceXml, "deviceType");
					currentDevice.friendlyName = XmlManager.getXmlNodeValue(deviceXml, "friendlyName");
					currentDevice.manufacturer = XmlManager.getXmlNodeValue(deviceXml, "manufacturer");
					currentDevice.manufacturerURL = XmlManager.getXmlNodeValue(deviceXml, "manufacturerURL");
					currentDevice.modelDescription = XmlManager.getXmlNodeValue(deviceXml, "modelDescription");
					currentDevice.modelName = XmlManager.getXmlNodeValue(deviceXml, "modelName");
					currentDevice.modelNumber = XmlManager.getXmlNodeValue(deviceXml, "modelNumber");
					currentDevice.modelURL = XmlManager.getXmlNodeValue(deviceXml, "modelURL");
					currentDevice.serialNumber = XmlManager.getXmlNodeValue(deviceXml, "serialNumber");
					currentDevice.UDN = XmlManager.getXmlNodeValue(deviceXml, "UDN");
					currentDevice.UPC = XmlManager.getXmlNodeValue(deviceXml, "UPC");
					currentDevice.presentationURL = XmlManager.getXmlNodeValue(deviceXml, "presentationURL");
					currentDevice.iconList = XmlManager.getIconList(deviceXml);
					currentDevice.serviceList = XmlManager.getServiceList(deviceXml, currentDevice.location);
					currentDevice.deviceList = XmlManager.getDeviceList(deviceXml, currentDevice.location);

					currentDevice.startTimer();
					BrisaLogger.logger.info("Device created. Max-age timer started.");
					
					loadServicesFromDevice(currentDevice);
					
					return currentDevice;
					/*
					if (currentDevice.serviceList != null) 
					{
						currentDevice.startTimer();
						BrisaLogger.logger.info("Device created. Max-age timer started.");
						
						loadServicesFromDevice(currentDevice);
						
						return currentDevice;
					}
					*/
				}else{
					return null;
				}
			}
		}
		else{
			return null;
		}
		
		return null;
	}
	
	private boolean deviceAlreadyExists(String location) {
		
		Boolean found = false;
		
		for (BrisaCPDevice device : b.getArrayDevices()) {
			if(device.location.equals(location))
			{
				found = true;
				break;
			}
		}
		
		return found;
	}
	
	/**
	 * 
	 * Method that fill the ActionList and the ServiceStateTable from all
	 * services of a selected device.
	 * 
	 * @param device
	 *            The device that will have their services filled;
	 * @return
	 */
	private void loadServicesFromDevice(BrisaCPDevice device) {

		if(device.serviceList == null)
			device.serviceList = new ArrayList<BrisaCPService>();
		
		for (BrisaCPService service : device.serviceList) {

			BrisaLogger.logger.info("Service url from device: "
					+ device.friendlyName + " is " + service.SCPDURL);

			service.urlBase = device.urlBase;
			
			// GET THE SCPDURL FROM THE CURRENT SERVICE;
			Element rootService = XmlManager.getScdpXml(service.SCPDURL);

			// INIT THE ACTION LIST FROM THE SERVICE;
			service.actionList = new ArrayList<BrisaCPAction>();

			// IF THE XML VAS LOADED SUCESSFULLY;
			if (rootService != null) {

				for (Object auxAction : rootService.getChild("actionList",
						rootService.getNamespace()).getChildren()) {

					service.actionList.add(getAction(auxAction,
							rootService.getNamespace()));
				}

				service.serviceStateTable = new ArrayList<BrisaCPStateVariable>();

				for (Object auxStateVariable : rootService.getChild(
						"serviceStateTable", rootService.getNamespace())
						.getChildren()) {

					// ADD STATE VARIABLE TO SERVICE STATE TABLE;
					service.serviceStateTable.add(getStateVariable(
							auxStateVariable, rootService.getNamespace()));
				}
			}
		}
	}
	
	private BrisaCPAction getAction(Object auxAction, Namespace namespace) {

		// NEW ACTION
		BrisaCPAction newAction = new BrisaCPAction();
		Element actionXml = (Element) auxAction;

		// NEW ACTION NAME
		newAction.name = XmlManager.getXmlNodeValue(actionXml, "name");

		// NEW ACTION ARGUMENT LIST
		newAction.argumentList = new ArrayList<BrisaCPArgument>();

		try {

			for (Object auxArgument : actionXml.getChild("argumentList",
					namespace).getChildren()) {

				Element argumentXml = (Element) auxArgument;

				BrisaCPArgument argument = new BrisaCPArgument();
				argument.direction = XmlManager.getXmlNodeValue(argumentXml,
						"direction");
				argument.name = XmlManager.getXmlNodeValue(argumentXml, "name");
				argument.relatedStateVariable = XmlManager.getXmlNodeValue(
						argumentXml, "relatedStateVariable");
				argument.retval = XmlManager.getXmlNodeValue(argumentXml,
						"retval");

				newAction.argumentList.add(argument);
			}

		} catch (Exception e) {
			BrisaLogger.logger.warning("No argument list found for the action "
					+ newAction.name);
		}

		return newAction;
	}

	private BrisaCPStateVariable getStateVariable(Object auxStateVariable,
			Namespace namespace) {

		// NEW STATE VARIABLE
		BrisaCPStateVariable newStateVariable = new BrisaCPStateVariable();
		Element stateVariableXml = (Element) auxStateVariable;

		newStateVariable.sendEvents = stateVariableXml
				.getAttributeValue("sendEvents");
		newStateVariable.multicast = stateVariableXml
				.getAttributeValue("multicast");

		// NEW STATE VARIABLE ATTRIBUTES
		newStateVariable.name = XmlManager.getXmlNodeValue(stateVariableXml,
				"name");
		newStateVariable.defaultValue = XmlManager.getXmlNodeValue(
				stateVariableXml, "defaultValue");
		newStateVariable.dataType = XmlManager.getXmlNodeValue(
				stateVariableXml, "dataType");

		Element allowedValueRangeXml = stateVariableXml.getChild(
				"allowedValueRange", namespace);

		if (allowedValueRangeXml != null) {
			// NEW STATE VARIABLE ALLOWED VALUE RANGE
			BrisaCPAllowedValueRange allowedValueRange = new BrisaCPAllowedValueRange();
			allowedValueRange.maximum = Float.parseFloat(XmlManager
					.getXmlNodeValue(allowedValueRangeXml, "maximum"));
			allowedValueRange.minimum = Float.parseFloat(XmlManager
					.getXmlNodeValue(allowedValueRangeXml, "minimum"));
			allowedValueRange.step = Float.parseFloat(XmlManager
					.getXmlNodeValue(allowedValueRangeXml, "step"));

			newStateVariable.allowedValueRange = allowedValueRange;
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

			newStateVariable.allowedValueList = arrayValues;
		}

		return newStateVariable;
	}


}
