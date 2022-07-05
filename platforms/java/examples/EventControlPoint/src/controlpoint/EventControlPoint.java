package controlpoint;

import java.util.ArrayList;

import upnp.controlpoint.core.BrisaControlPoint;
import upnp.controlpoint.event.BrisaEventProxy;
import upnp.controlpoint.model.BrisaCPDevice;
import upnp.controlpoint.model.BrisaCPService;
import upnp.event.BrisaControlPointAdapter;
import upnp.event.BrisaControlPointEvent;

/**
 * 
 * Application example;
 * 
 * @author Thiago Ribeiro
 * @author Wendell Silva
 *
 */
public class EventControlPoint {
	
	private BrisaControlPoint cp;
	
	//=========================================
	public static void main(String[] args) {
		
		EventControlPoint e = new EventControlPoint();
		e.addListener();
		
		e.start();
		e.discover();
	}
	//========================================
	
	public void addListener() {
		
		cp.addListener(new BrisaControlPointAdapter() {
			@Override
			public void searchComplete(BrisaControlPointEvent e) {
				//searchComplete(e);
				
				ArrayList<BrisaCPDevice> devices = cp.getArrayDevices();
				
				for(BrisaCPDevice d: devices) {
					for(BrisaCPService s: d.serviceList) {
						BrisaEventProxy subscription = cp.getSubscriptionProxy(d, s);
						subscription.subscribe(60);
					}
				}
			}
		});
		
	}
	
	public void start() {
		cp.start();
	}
	
	public void discover() {
		cp.discover();
	}

}
