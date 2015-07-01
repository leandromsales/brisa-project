package jbrisa.webserver;

import java.util.Observer;

import jbrisa.upnp.device.core.BrisaDevice;


public interface BrisaWebServer {
	
	public void addNewObserver(Observer o);
	
	public void start();
	
	public void stop();
	
	public void setPort(int port);
	
	public int getPort();
	
	public boolean isRunning();
	
	//TODO pensar numa forma melhor de comunicar brisadevice com webserver;
	public void setBrisaDevice(BrisaDevice brisaDevice);

}
