package jbrisa.examples.controlpoint;

import java.util.logging.Level;

import jbrisa.upnp.controlpoint.core.BrisaControlPoint;
import jbrisa.utils.BrisaLogger;

/**
 * 
 * Simple control point example;
 * 
 * This UPnP Control Point prints UPnP devices list.
 * 
 * @author Thiago Ribeiro
 * @author Wendell Silva
 *
 */
public class SimpleControlPoint {
	
	public static void main(String[] args) {
	
		BrisaControlPoint cp = new BrisaControlPoint();
	
		BrisaLogger.logger.setLevel(Level.OFF);

		cp.start();
		System.out.println("Control Point Address: " + cp.getUrlBase());
		cp.discover();
		
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println("Devices found (" + cp.getArrayDevices().size() + "):");
		cp.printDevices();
	}
}
