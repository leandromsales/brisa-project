package jbrisa.upnp.controlpoint.model;

import java.util.ArrayList;
import java.util.Observable;
import java.util.Timer;
import java.util.TimerTask;

import jbrisa.utils._Constants;


public class BrisaCPDevice extends Observable {
	
	public Integer maxAge = _Constants.DEFAULT_MAX_AGE;
	public String location;
	
	public String deviceType;
	public String friendlyName;
	public String manufacturer;
	public String manufacturerURL;
	public String modelDescription;
	public String modelName;
	public String modelNumber;
	public String modelURL;
	public String serialNumber;
	public String UDN;
	public String UPC;
	public String presentationURL;
	public String urlBase;
	
	public ArrayList<BrisaCPIcon> iconList;
	public ArrayList<BrisaCPService> serviceList;
 	public ArrayList<BrisaCPDevice> deviceList;
 	
 	private Timer timer;
 	
 	public BrisaCPDevice() {
 		
	}
 	
 	public void startTimer() {
 		
 		timer = new Timer();
 		timer.schedule(new Task(), maxAge*1000);
	}
 	
 	public void stopTimer(){
 		
 		timer.cancel();
 	}
 	
 	class Task extends TimerTask {
 		@Override
 		public void run() {
 			
 			//notificar que timer expirou e device esta expirado;
 			System.out.println("TIMER EXPIRADO");
 			
 			setChanged();
 			notifyObservers();
 			
 			timer.cancel();
 		}
 	}

	public Integer getMaxAge() {
		return maxAge;
	}

	public void setMaxAge(Integer maxAge) {
		this.maxAge = maxAge;
	}

	public String getLocation() {
		return location;
	}

	public void setLocation(String location) {
		this.location = location;
	}

	public String getDeviceType() {
		return deviceType;
	}

	public void setDeviceType(String deviceType) {
		this.deviceType = deviceType;
	}

	public String getFriendlyName() {
		return friendlyName;
	}

	public void setFriendlyName(String friendlyName) {
		this.friendlyName = friendlyName;
	}

	public String getManufacturer() {
		return manufacturer;
	}

	public void setManufacturer(String manufacturer) {
		this.manufacturer = manufacturer;
	}

	public String getManufacturerURL() {
		return manufacturerURL;
	}

	public void setManufacturerURL(String manufacturerURL) {
		this.manufacturerURL = manufacturerURL;
	}

	public String getModelDescription() {
		return modelDescription;
	}

	public void setModelDescription(String modelDescription) {
		this.modelDescription = modelDescription;
	}

	public String getModelName() {
		return modelName;
	}

	public void setModelName(String modelName) {
		this.modelName = modelName;
	}

	public String getModelNumber() {
		return modelNumber;
	}

	public void setModelNumber(String modelNumber) {
		this.modelNumber = modelNumber;
	}

	public String getModelURL() {
		return modelURL;
	}

	public void setModelURL(String modelURL) {
		this.modelURL = modelURL;
	}

	public String getSerialNumber() {
		return serialNumber;
	}

	public void setSerialNumber(String serialNumber) {
		this.serialNumber = serialNumber;
	}

	public String getUDN() {
		return UDN;
	}

	public void setUDN(String uDN) {
		UDN = uDN;
	}

	public String getUPC() {
		return UPC;
	}

	public void setUPC(String uPC) {
		UPC = uPC;
	}

	public String getPresentationURL() {
		return presentationURL;
	}

	public void setPresentationURL(String presentationURL) {
		this.presentationURL = presentationURL;
	}

	public String getUrlBase() {
		return urlBase;
	}

	public void setUrlBase(String urlBase) {
		this.urlBase = urlBase;
	}

	public ArrayList<BrisaCPIcon> getIconList() {
		return iconList;
	}

	public void setIconList(ArrayList<BrisaCPIcon> iconList) {
		this.iconList = iconList;
	}

	public ArrayList<BrisaCPService> getServiceList() {
		return serviceList;
	}

	public void setServiceList(ArrayList<BrisaCPService> serviceList) {
		this.serviceList = serviceList;
	}

	public ArrayList<BrisaCPDevice> getDeviceList() {
		return deviceList;
	}

	public void setDeviceList(ArrayList<BrisaCPDevice> deviceList) {
		this.deviceList = deviceList;
	}
 	
 	
 	
 	

}
