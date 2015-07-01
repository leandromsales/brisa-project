package controlpoint;

import java.util.Observable;
import java.util.Observer;

import upnp.controlpoint.core.BrisaControlPoint;
import upnp.controlpoint.event.BrisaMulticastEventReceiver;

public class MulticastControlPoint implements Observer {
	
	BrisaControlPoint controlPoint;
	
	public MulticastControlPoint() {
		controlPoint = new BrisaControlPoint();
	}
	
	public void startAndDiscover() {
		controlPoint.start();
		
		controlPoint.subscribeMulticastEvents();
		
		controlPoint.discover();
	}

	@Override
	public void update(Observable o, Object arg) {
		
		if (o instanceof BrisaMulticastEventReceiver) {
			System.out.println("\n=======================================");
			System.out.println("Multicast event received: \n");
			System.out.println("Message: " + arg.toString());
		}
		
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		MulticastControlPoint cp = new MulticastControlPoint();
		cp.startAndDiscover();
	}

}
