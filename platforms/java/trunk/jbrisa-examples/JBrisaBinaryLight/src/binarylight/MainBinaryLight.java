package binarylight;

import java.io.IOException;
import java.util.Scanner;
import java.util.logging.Level;

import utils.BrisaLogger;
import utils.UUIDFactory;

public class MainBinaryLight {
	
	private static final String DEVICE_TYPE = "urn:schemas-upnp-org:device:BinaryLight:1";
	private static final String DEVICE_FRIENDLY_NAME = "JBRisa Light Device";
	private static final String DEVICE_MANUFACTURER = "JBrisa Team.";
	private static final String DEVICE_MANUFACTURER_URL = "https://garage.maemo.org/projects/brisa";
	private static final String DEVICE_MODEL_DESCRIPTION = "An UPnP Binary Light Device";
	private static final String DEVICE_MODEL_NAME = "JBrisa Light Device";
	private static final String DEVICE_MODEL_NUMBER = "1.0";
	private static final String DEVICE_MODEL_URL = "https://garage.maemo.org/projects/brisa";
	private static final String DEVICE_SERIAL_NUMBER = "1.0";
	//private static final String DEVICE_UDN = "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb";
	private static final Integer DEFAULT_PORT = 6789;
	
	/**
	 * @param args
	 * @throws IOException 
	 * @throws JDOMException 
	 */
	public static void main(String[] args) throws IOException {
		
		BrisaLogger.logger.setLevel(Level.OFF);
		
		final BinaryLight device = new BinaryLight(
				DEVICE_TYPE, 
				DEVICE_FRIENDLY_NAME,
	            DEVICE_MANUFACTURER, 
	            DEVICE_MANUFACTURER_URL,
	            DEVICE_MODEL_DESCRIPTION, 
	            DEVICE_MODEL_NAME, 
	            DEVICE_MODEL_NUMBER,
	            DEVICE_MODEL_URL, 
	            DEVICE_SERIAL_NUMBER, 
	            UUIDFactory.getCompleteUiid(), 
	            " ", 
	            " ", 
	            DEFAULT_PORT
	            );
		
		device.start();
		
		@SuppressWarnings("unused")
		Thread t = new Thread(new Runnable() {
			
			@Override
			public void run() {
				
				Scanner scanner = new Scanner(System.in);
				String in = scanner.nextLine();
				
				if(in.equalsIgnoreCase("exit")) {
					device.stop();
					System.exit(0);
				}
				
			}
		});
		
		//t.start();
		
		try {
			Thread.sleep(600000);  //60 seg
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//device.stop();
		//System.exit(0);
		
	}

}
