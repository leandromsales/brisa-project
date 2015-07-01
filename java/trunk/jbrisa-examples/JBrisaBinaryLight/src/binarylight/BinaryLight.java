package binarylight;

import upnp.device.core.BrisaDevice;

public class BinaryLight extends BrisaDevice {
	
	private SwitchPower switchPower;
	
	public BinaryLight(String deviceType, String friendlyName,
			String manufacturer, String manufacturerURL,
			String modelDescription, String modelName, String modelNumber,
			String modelURL, String serialNumber, String udn, String upc,
			String presentationURL, int port) {
		
		super(deviceType, friendlyName, manufacturer, manufacturerURL,
				modelDescription, modelName, modelNumber, modelURL, serialNumber, udn,
				upc, presentationURL, port);
		
		this.switchPower = new SwitchPower();
		switchPower.setDescriptonFile("xml/SwitchPower-scpd.xml");
		
		this.addService(switchPower);
		
	}



}
